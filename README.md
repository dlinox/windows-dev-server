# LNX Server - Local Windows Server (Nginx + MySQL + PHP)

**Languages:** [🇬🇧 English](README.md) • [🇪🇸 Español](README.es.md)

Local development stack for Windows with Nginx, MySQL, and multiple PHP versions running as Windows services.

## ✨ Features

- ✅ **Unified manager** (`server`) to control all services
- ✅ **Multiple PHP versions** running simultaneously on different ports
- ✅ **Virtual Hosts** with custom domains (e.g., `test.local`)
- ✅ **Windows Services** - Start automatically with the system
- ✅ **Automatic logs** per day with rotation
- ✅ **Automatic restart** if any service fails

## ⚠️ Note on Multiple Versions

This system is designed to run multiple simultaneous versions of services: not only PHP, but also MySQL and (if desired) multiple Nginx binaries — each on its own port/directory. Apache can also be integrated, although it is not documented here.

## 📋 Requirements

- Windows 10/11
- **MinGW-w64 (g++)** - To compile the manager
  - Download: https://www.mingw-w64.org/downloads/
  - Or use MSYS2: https://www.msys2.org/
- Visual C++ Redistributable 2015-2022 (x64)
  - Download: https://aka.ms/vs/17/release/vc_redist.x64.exe
- WinSW (included in `/services/winsw.exe`)

## 📁 Project Structure

```
server/
├── manager/              # Service manager (C++)
│   ├── compile.bat      # Compilation script
│   ├── main.cpp         # Source code
│   └── bin/
│       └── server.exe   # Compiled executable
├── nginx/               # Nginx web server (excluded from Git)
│   ├── conf/
│   │   ├── nginx.conf
│   │   └── conf.d/      # Virtual host configurations
│   ├── html/
│   └── nginx.exe
├── mysql/               # MySQL/MariaDB (excluded from Git)
│   └── 8.4.6/
│       ├── bin/
│       └── data/
├── php/                 # PHP versions (excluded from Git)
│   ├── php-7.4.33/
│   ├── php-8.3.26/
│   └── php-8.4.13/
└── services/            # Windows Services
    ├── winsw.exe
    ├── nginx/
    │   └── nginx-service.xml
    ├── mysql/
    │   └── mysql-service.xml
    └── php/
        ├── 74/
        ├── 83/
        └── 84/
            └── php84-service.xml
```

---

## 🚀 Installation

> **📝 Note:** This repository **DOES NOT include** the binaries for Nginx, MySQL, or PHP. It only contains the structure, configurations, and service manager.

### 1. Clone the Repository

```powershell
git clone <repository-url>
cd server
```

---

### 2. Install MySQL

1. **Download:**
   - https://dev.mysql.com/downloads/mysql/ (ZIP Archive for Windows)

2. **Extract:**
   ```powershell
   # Extract to /mysql/8.4.6/ (or corresponding version)
   ```

3. **Initialize database:**
   ```powershell
   cd mysql\8.4.6\bin
   .\mysqld --initialize-insecure --datadir=..\data
   ```
   
   > Default user: `root` (no password)

4. **Create service:**
   ```powershell
   cd ..\..\..\..\services\mysql
   copy ..\winsw.exe mysql-service.exe
   .\mysql-service.exe install
   .\mysql-service.exe start
   ```

---

### 3. Install Nginx

1. **Download:**
   - https://nginx.org/en/download.html (Windows version)

2. **Extract:**
   ```powershell
   # Extract all contents to /nginx/
   ```

3. **Configure:**
   ```powershell
   # Create folder for virtual hosts
   mkdir nginx\conf\conf.d
   ```

4. **Edit `nginx\conf\nginx.conf`:**
   
   Add at the end of the `http` block, before the closing brace `}`:
   ```nginx
   include conf.d/*.conf;
   ```

5. **Create service:**
   ```powershell
   cd services\nginx
   copy ..\winsw.exe nginx-service.exe
   .\nginx-service.exe install
   .\nginx-service.exe start
   ```

---

### 4. Install PHP

1. **Download:**
   - https://windows.php.net/download/ (Thread Safe, x64)

2. **Extract:**
   ```powershell
   # Extract to /php/php-8.4.13/ (or desired version)
   ```

3. **Configure php.ini:**
   ```powershell
   cd php\php-8.4.13
   copy php.ini-development php.ini
   
   # Edit php.ini and enable extensions:
   # extension=curl
   # extension=mysqli
   # extension=mbstring
   # etc.
   ```

4. **Create service (PHP 8.4 example):**
   
   Create `/services/php/84/php84-service.xml`:
   ```xml
   <service>
       <id>php84</id>
       <name>PHP 8.4</name>
       <description>PHP 8.4 server running as a service</description>
       <executable>C:\lnx\server\php\php-8.4.13\php-cgi.exe</executable>
       <arguments>-b 127.0.0.1:9084</arguments>
       <workingdirectory>C:\lnx\server\php\php-8.4.13</workingdirectory>
       <startmode>Automatic</startmode>
       <onfailure action="restart" delay="5 sec" />
       <log mode="roll-by-time">
           <pattern>yyyy-MM-dd</pattern>
       </log>
   </service>
   ```

5. **Install service:**
   ```powershell
   cd services\php\84
   copy ..\..\winsw.exe php84-service.exe
   .\php84-service.exe install
   .\php84-service.exe start
   ```

> **⚠️ Important:** PHP 8.4+ requires **Visual C++ Redistributable 2022**: https://aka.ms/vs/17/release/vc_redist.x64.exe

---

### 5. Compile Manager

```powershell
cd manager
.\compile.bat
```

This will create `bin\server.exe`

---

### 6. Add Manager to PATH

**PowerShell (Current user):**
```powershell
[Environment]::SetEnvironmentVariable("PATH", $env:PATH + ";C:\lnx\server\manager\bin", "User")
```

**Or manually:**
1. Open "System Environment Variables"
2. Edit user `PATH` variable
3. Add: `C:\lnx\server\manager\bin`
4. **Restart PowerShell**

**Verify:**
```powershell
server --help
```

---

## 🎯 Manager Usage

### Basic Commands

```powershell
# Syntax
server <service> <action>

# Available actions: start, stop, restart, status, install, uninstall
```

### Service Management

```powershell
# Nginx
server nginx start
server nginx stop
server nginx restart
server nginx status

# MySQL
server mysql start
server mysql stop

# PHP
server php84 start
server php84 status
```

### Host Management

```powershell
# Add domain to Windows hosts file
server host test.local
server host blog.local
server host api.local

# Note: Requires administrator permissions
```

### Default Ports

| Service  | Port |
|----------|------|
| Nginx    | 80   |
| MySQL    | 3306 |
| PHP 7.1  | 9071 |
| PHP 7.2  | 9072 |
| PHP 7.4  | 9074 |
| PHP 8.3  | 9083 |
| PHP 8.4  | 9084 |

---

## 🌐 Virtual Host Configuration

### Create a Virtual Host

1. **Create configuration:**

`/nginx/conf/conf.d/myproject.conf`:
```nginx
server {
    listen       80;
    server_name  myproject.local;
    root html/myproject;
    index index.php;

    location / {
        try_files $uri $uri/ /index.php?$args;
    }

    location ~ \.php$ {
        fastcgi_pass   127.0.0.1:9084;  # PHP 8.4
        fastcgi_index  index.php;
        fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
        include        fastcgi_params;
    }
}
```

2. **Add domain:**
```powershell
server host myproject.local
```

3. **Create project:**
```powershell
mkdir nginx\html\myproject
echo "<?php phpinfo();" > nginx\html\myproject\index.php
```

4. **Restart Nginx:**
```powershell
server nginx restart
```

5. **Access:** http://myproject.local

---

## 📦 Composer - PHP Dependency Manager

### Installation

1. **Download and install:**
   - https://getcomposer.org/download/
   - Run `Composer-Setup.exe`
   - Select any PHP version (e.g., `C:\lnx\server\php\php-8.4.13\php.exe`)

2. **Verify:**
   ```powershell
   composer --version
   ```

### Configure Aliases for Multiple Versions

**Edit PowerShell profile:**
```powershell
notepad $PROFILE
```

**Add aliases:**
```powershell
# Aliases for PHP versions
Set-Alias -Name php74 -Value 'C:\lnx\server\php\php-7.4.33\php.exe'
Set-Alias -Name php83 -Value 'C:\lnx\server\php\php-8.3.26\php.exe'
Set-Alias -Name php84 -Value 'C:\lnx\server\php\php-8.4.13\php.exe'

# Alias for composer.phar
function composer-phar { & "C:\ProgramData\ComposerSetup\bin\composer.phar" $args }
Set-Alias -Name cphar -Value composer-phar
```

**Reload profile:**
```powershell
. $PROFILE
```

### Usage with Different PHP Versions

```powershell
# Install dependencies with PHP 7.4
cd C:\lnx\server\nginx\html\old-project
php74 cphar install

# Install dependencies with PHP 8.4
cd C:\lnx\server\nginx\html\new-project
php84 cphar install

# Update dependencies
php84 cphar update

# Add packages
php83 cphar require vendor/package

# Autoload
php83 cphar dump-autoload
```

### Check Version

```powershell
php74 --version
php83 --version
php84 --version
```

---

## 💡 Advanced Management

Note: The project philosophy is to support multiple simultaneous installations of services. This applies to PHP, but also to MySQL and Nginx (for example, multiple Nginx servers with different configurations/ports). Apache can also be integrated if desired, although it is not documented here.

### Add a New PHP Version

To run multiple versions simultaneously:

#### 1. Download and Install

```powershell
# Download: https://windows.php.net/download/ (Thread Safe, x64)
# Extract to: /php/php-7.4.33/

cd php\php-7.4.33
copy php.ini-development php.ini
```

#### 2. Create Service

```powershell
mkdir services\php\74
cd services\php\74
copy ..\..\winsw.exe php74-service.exe
```

Create `php74-service.xml`:
```xml
<service>
    <id>php74</id>
    <name>PHP 7.4</name>
    <description>PHP 7.4 server running as a service</description>
    <executable>C:\lnx\server\php\php-7.4.33\php-cgi.exe</executable>
    <arguments>-b 127.0.0.1:9074</arguments>
    <workingdirectory>C:\lnx\server\php\php-7.4.33</workingdirectory>
    <startmode>Automatic</startmode>
    <onfailure action="restart" delay="5 sec" />
    <log mode="roll-by-time">
        <pattern>yyyy-MM-dd</pattern>
    </log>
</service>
```

> **⚠️** Each version must use a unique port (9074, 9083, 9084, etc.)

#### 3. Install and Start

```powershell
.\php74-service.exe install
.\php74-service.exe start

# Or with the manager:
server php74 install
server php74 start
```

#### 4. Configure Nginx

Each project chooses its version by port:

```nginx
# Project with PHP 7.4
server {
    server_name  old.local;
    location ~ \.php$ {
        fastcgi_pass   127.0.0.1:9074;  # PHP 7.4
        # ...
    }
}

# Project with PHP 8.4
server {
    server_name  new.local;
    location ~ \.php$ {
        fastcgi_pass   127.0.0.1:9084;  # PHP 8.4
        # ...
    }
}
```

#### 5. Restart Nginx

```powershell
server nginx restart
server php74 status
server php84 status
```

### Access MySQL

```powershell
cd mysql\8.4.6\bin
.\mysql -u root

# Change root password:
ALTER USER 'root'@'localhost' IDENTIFIED BY 'your_password';
FLUSH PRIVILEGES;
```

### MySQL Backup

```powershell
# Export
cd mysql\8.4.6\bin
.\mysqldump -u root -p database_name > backup.sql

# Import
.\mysql -u root -p database_name < backup.sql
```

### Logs

Logs are generated in:
- `/services/nginx/nginx-service_YYYY-MM-DD.err.log`
- `/services/mysql/mysql-service_YYYY-MM-DD.err.log`
- `/services/php/84/php84-service_YYYY-MM-DD.err.log`

---

## ⚠️ Troubleshooting

### Error: Incompatible VCRUNTIME140.dll

PHP 8.4+ requires Visual C++ Redistributable 2022:
```
https://aka.ms/vs/17/release/vc_redist.x64.exe
```

### Error: "No input file specified"

- Verify the PHP file exists
- Check `fastcgi_param SCRIPT_FILENAME` in Nginx configuration
- Verify the PHP service is running: `server php84 status`

### Port Already in Use

```powershell
netstat -ano | findstr :9084
```

### Service Won't Start

Check logs in `/services/<service>/*.err.log`

### Error Compiling Manager (g++ not found)

Install MinGW-w64:
- https://www.mingw-w64.org/downloads/
- Or use MSYS2: https://www.msys2.org/
- Add MinGW `bin` to system PATH

### Command 'server' Not Found

- Verify PATH includes: `C:\lnx\server\manager\bin`
- Restart PowerShell
- Verify: `$env:PATH -split ';' | Select-String "manager"`

---

## 🤝 Contributing

1. Fork the project
2. Create branch: `git checkout -b feature/new-feature`
3. Commit: `git commit -m 'Add new feature'`
4. Push: `git push origin feature/new-feature`
5. Pull Request

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

In summary: you can use, modify, and distribute this code freely, even for commercial purposes, as long as you include the copyright notice and license.

## 🔗 Useful Links

- [Nginx Documentation](https://nginx.org/en/docs/)
- [MySQL Documentation](https://dev.mysql.com/doc/)
- [PHP Documentation](https://www.php.net/docs.php)
- [WinSW Documentation](https://github.com/winsw/winsw)
- [Composer Documentation](https://getcomposer.org/doc/)
