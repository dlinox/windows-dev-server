@echo off
REM Compilation script with embedded manifest
REM Compiles main.cpp and embeds the admin elevation manifest

echo [*] Compiling resources...
windres resources.rc -o resources.o

if errorlevel 1 (
    echo [!] Error: Could not compile resources
    exit /b 1
)

echo [+] Resources compiled

echo [*] Creating bin folder...
if not exist "bin" mkdir bin

echo [*] Compiling server.exe...
g++ main.cpp resources.o -o bin\server.exe

if errorlevel 1 (
    echo [!] Error: Could not compile the program
    exit /b 1
)

echo [+] Compilation successful
echo [+] bin\server.exe is ready to use
echo.
echo [*] The program will request admin permissions automatically
echo [*] When you run the program, you will see the elevation dialog
pause
