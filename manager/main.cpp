#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <windows.h>
#include <winsvc.h>

using namespace std;

// ANSI color codes
const string COLOR_GREEN = "\033[92m";   // Bright green
const string COLOR_RED = "\033[91m";     // Bright red
const string COLOR_BLUE = "\033[94m";    // Bright blue
const string COLOR_YELLOW = "\033[93m";  // Bright yellow
const string COLOR_RESET = "\033[0m";    // Reset color

// Status indicators
const string EMOJI_ACTIVE = "[ACTIVE]";
const string EMOJI_INACTIVE = "[INACTIVE]";

/**
 * Checks if the program is running with administrator privileges
 */
bool isRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    HANDLE hToken = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &elevation, cbSize, &cbSize)) {
            isAdmin = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }

    return isAdmin == TRUE;
}

/**
 * Restarts the program with administrator privileges
 * Returns: true if restart was successful, false otherwise
 */
bool relaunchAsAdmin(int argc, char* argv[]) {
    // Build the command line with the original arguments
    string exePath = argv[0];
    string cmdLine = "\"" + exePath + "\"";
    
    for (int i = 1; i < argc; i++) {
        cmdLine += " \"" + string(argv[i]) + "\"";
    }

    // Use PowerShell to restart with administrator permissions
    // This will show the elevation dialog
    string psCmd = "PowerShell -Command \"Start-Process '" + exePath + "' -ArgumentList @(";
    for (int i = 1; i < argc; i++) {
        psCmd += "'" + string(argv[i]) + "'";
        if (i < argc - 1) psCmd += ",";
    }
    psCmd += ") -Verb RunAs -Wait\"";

    // Execute the PowerShell command
    int result = system(psCmd.c_str());
    
    return result == 0;
}

/**
 * Enables ANSI sequence support (colors) in Windows console
 */
void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

/**
 * Converts a string to lowercase
 */
string toLower(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return result;
}

/**
 * Executes a system command and returns its output
 */
string executeCommand(const string& command) {
    string result;
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        return result;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    _pclose(pipe);
    return result;
}

/**
 * Gets the service status
 * Returns: "RUNNING", "STOPPED" or "UNKNOWN"
 */
string getServiceStatus(const string& serviceName) {
    string command = "sc query " + serviceName;
    string output = executeCommand(command);

    // If the output contains "RUNNING", the service is active
    if (output.find("RUNNING") != string::npos) {
        return "RUNNING";
    }
    // If the output contains "STOPPED", the service is inactive
    else if (output.find("STOPPED") != string::npos) {
        return "STOPPED";
    }
    // If it contains "does not exist", the service doesn't exist
    else if (output.find("does not exist") != string::npos) {
        return "NOT_FOUND";
    }

    return "UNKNOWN";
}

/**
 * Controls a service (start, stop, restart)
 * Returns: true if the operation was successful, false otherwise
 */
bool controlService(const string& serviceName, const string& action) {
    string command;
    string actionLower = toLower(action);

    if (actionLower == "start") {
        command = "sc start " + serviceName;
    } else if (actionLower == "stop") {
        command = "sc stop " + serviceName;
    } else if (actionLower == "restart") {
        // Stop and then start
        string stopCmd = "sc stop " + serviceName;
        executeCommand(stopCmd);
        // Wait a moment for it to stop
        Sleep(1000);
        command = "sc start " + serviceName;
    } else {
        return false;
    }

    string output = executeCommand(command);

    // Check if the operation was successful
    // The sc commands return "SUCCESS" if they work correctly
    // Also check if it says "Access is denied" (lack of permissions)
    if (output.find("SUCCESS") != string::npos || 
        output.find("service is already running") != string::npos) {
        return true;
    } else if (output.find("Access is denied") != string::npos || 
               output.find("ERROR_ACCESS_DENIED") != string::npos) {
        return false;  // Permission error
    }

    return false;
}

/**
 * Adds a local domain to the hosts file
 * Returns: true if successful, false otherwise
 */
bool addLocalHost(const string& domain) {
    // Path to the hosts file in Windows
    string hostsPath = "C:\\Windows\\System32\\drivers\\etc\\hosts";
    
    // Check that the domain ends with .local
    string domainLower = toLower(domain);
    if (domainLower.length() < 6 || domainLower.substr(domainLower.length() - 6) != ".local") {
        cout << COLOR_RED << "Error: Domain must end with .local" << COLOR_RESET << endl;
        return false;
    }
    
    // Check if the domain already exists in hosts file
    string checkCmd = "PowerShell -Command \"Get-Content '" + hostsPath + "' | Select-String '" + domain + "'\"";
    string existingEntry = executeCommand(checkCmd);
    if (!existingEntry.empty()) {
        cout << COLOR_YELLOW << "Domain '" << domain << "' already exists in hosts file." << COLOR_RESET << endl;
        return true;
    }
    
    // Create the entry
    string entry = "127.0.0.1       " + domain;
    
    // PowerShell command to append to hosts file (not replace)
    string psCmd = "PowerShell -Command \"Add-Content -Path '" + hostsPath + "' -Value '" + entry + "' -Encoding ASCII\"";
    
    int result = system(psCmd.c_str());
    
    if (result == 0) {
        cout << COLOR_GREEN << "Domain '" << domain << "' added successfully to hosts file." << COLOR_RESET << endl;
        return true;
    } else {
        cout << COLOR_RED << "Error: Could not add domain to hosts file." << COLOR_RESET << endl;
        cout << COLOR_YELLOW << "Make sure you have administrator permissions." << COLOR_RESET << endl;
        return false;
    }
}

/**
 * Shows the help message
 */
void showHelp() {
    cout << COLOR_BLUE << "\nUsage:" << COLOR_RESET << endl;
    cout << "  server <service> <action>" << endl;
    cout << "  server host <domain>" << endl;
    cout << "\n" << COLOR_BLUE << "Available actions:" << COLOR_RESET << endl;
    cout << "  status   - Show service status" << endl;
    cout << "  start    - Start the service" << endl;
    cout << "  stop     - Stop the service" << endl;
    cout << "  restart  - Restart the service" << endl;
    cout << "\n" << COLOR_BLUE << "Host management:" << COLOR_RESET << endl;
    cout << "  host     - Add a local domain to hosts file" << endl;
    cout << "\n" << COLOR_BLUE << "Examples:" << COLOR_RESET << endl;
    cout << "  server nginx status" << endl;
    cout << "  server nginx start" << endl;
    cout << "  server nginx stop" << endl;
    cout << "  server nginx restart" << endl;
    cout << "  server host myapp.local" << endl;
    cout << "  server host api.local" << endl << endl;
}

/**
 * Main function
 */
int main(int argc, char* argv[]) {
    // Enable ANSI color support
    enableVirtualTerminalProcessing();

    // Validate number of arguments
    if (argc < 2 || argc > 3) {
        showHelp();
        return 1;
    }

    string firstArg = argv[1];
    string secondArg = (argc == 3) ? argv[2] : "";
    
    string firstArgLower = toLower(firstArg);

    // Command: server host <domain>
    if (firstArgLower == "host") {
        if (secondArg.empty()) {
            cout << COLOR_RED << "Error: Domain name required." << COLOR_RESET << endl;
            cout << COLOR_BLUE << "Usage: server host <domain>" << COLOR_RESET << endl;
            cout << COLOR_BLUE << "Example: server host myapp.local" << COLOR_RESET << endl;
            return 1;
        }

        // Check that the domain ends with .local BEFORE requesting permissions
        string domainLower = toLower(secondArg);
        if (domainLower.length() < 6 || domainLower.substr(domainLower.length() - 6) != ".local") {
            cout << COLOR_RED << "Error: Domain must end with .local" << COLOR_RESET << endl;
            return 1;
        }

        // Check if administrator permissions are needed
        if (!isRunningAsAdmin()) {
            cout << COLOR_BLUE << "Administrator permissions required for this action..." << COLOR_RESET << endl;
            cout << COLOR_YELLOW << "Requesting privilege elevation..." << COLOR_RESET << endl;
            
            if (relaunchAsAdmin(argc, argv)) {
                // The program restarted with permissions, exit
                return 0;
            } else {
                // The user declined the elevation
                cout << COLOR_RED << "Error: Administrator permission request was denied." << COLOR_RESET << endl;
                return 1;
            }
        }

        // Add the local domain
        if (addLocalHost(secondArg)) {
            return 0;
        } else {
            return 1;
        }
    }

    // Command: server <service> <action>
    string serviceName = firstArg;
    string action = secondArg;

    // If no action is provided, show help
    if (action.empty()) {
        showHelp();
        return 1;
    }

    string actionLower = toLower(action);

    // If it's an action that requires permissions and doesn't have them, restart with permissions
    if ((actionLower == "start" || actionLower == "stop" || actionLower == "restart") && 
        !isRunningAsAdmin()) {
        cout << COLOR_BLUE << "Administrator permissions required for this action..." << COLOR_RESET << endl;
        cout << COLOR_YELLOW << "Requesting privilege elevation..." << COLOR_RESET << endl;
        
        if (relaunchAsAdmin(argc, argv)) {
            // The program restarted with permissions, exit
            return 0;
        } else {
            // The user declined the elevation
            cout << COLOR_RED << "Error: Administrator permission request was denied." << COLOR_RESET << endl;
            return 1;
        }
    }

    // Process the "status" action
    if (actionLower == "status") {
        string status = getServiceStatus(serviceName);

        if (status == "RUNNING") {
            cout << serviceName << "  - " << COLOR_GREEN << EMOJI_ACTIVE << " active" << COLOR_RESET << endl;
            return 0;
        } else if (status == "STOPPED") {
            cout << serviceName << "  - " << COLOR_RED << EMOJI_INACTIVE << " inactive" << COLOR_RESET << endl;
            return 0;
        } else if (status == "NOT_FOUND") {
            cout << COLOR_RED << "Error: Service '" << serviceName << "' does not exist." << COLOR_RESET << endl;
            return 1;
        } else {
            cout << COLOR_YELLOW << "Error: Could not determine service status." << COLOR_RESET << endl;
            return 1;
        }
    }
    // Process control actions (start, stop, restart)
    else if (actionLower == "start" || actionLower == "stop" || actionLower == "restart") {
        // Show the action to be performed
        cout << COLOR_BLUE << "Executing: " << actionLower << " " << serviceName << "..." << COLOR_RESET << endl;

        if (controlService(serviceName, action)) {
            cout << COLOR_GREEN << "Operation completed successfully." << COLOR_RESET << endl;
            return 0;
        } else {
            cout << COLOR_RED << "Error: Could not complete the operation." << COLOR_RESET << endl;
            return 1;
        }
    }
    // Invalid action
    else {
        cout << COLOR_RED << "Error: Invalid action: '" << action << "'" << COLOR_RESET << endl;
        showHelp();
        return 1;
    }

    return 0;
}