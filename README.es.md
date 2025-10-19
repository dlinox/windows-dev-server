# LNX Server - Servidor Windows local (Nginx + MySQL + PHP)

**Languages:** [🇬🇧 English](README.md) • [🇪🇸 Español](README.es.md)

Stack de desarrollo local para Windows con Nginx, MySQL y múltiples versiones de PHP ejecutándose como servicios de Windows.

## ✨ Características

- ✅ **Gestor unificado** (`server`) para controlar todos los servicios
- ✅ **Múltiples versiones de PHP** corriendo simultáneamente en diferentes puertos
- ✅ **Virtual Hosts** con dominios personalizados (ej: `test.local`)
- ✅ **Servicios de Windows** - Inician automáticamente con el sistema
- ✅ **Logs automáticos** por día con rotación
- ✅ **Reinicio automático** si algún servicio falla

## ⚠️ Nota sobre múltiples versiones

Este sistema está diseñado para ejecutar varias versiones simultáneas de los servicios: no solo PHP, sino también MySQL y (si se desea) múltiples binarios de Nginx — cada uno en su propio puerto/directorio. Apache también se puede integrar, aunque no se documenta aquí.


## 📋 Requisitos

- Windows 10/11
- **MinGW-w64 (g++)** - Para compilar el manager
  - Descargar: https://www.mingw-w64.org/downloads/
  - O usar MSYS2: https://www.msys2.org/
- Visual C++ Redistributable 2015-2022 (x64)
  - Descargar: https://aka.ms/vs/17/release/vc_redist.x64.exe
- WinSW (incluido en `/services/winsw.exe`)

## 📁 Estructura del Proyecto

```
server/
├── manager/              # Gestor de servicios (C++)
│   ├── compile.bat      # Script de compilación
│   ├── main.cpp         # Código fuente
│   └── bin/
│       └── server.exe   # Ejecutable compilado
├── nginx/               # Servidor web Nginx (excluido de Git)
│   ├── conf/
│   │   ├── nginx.conf
│   │   └── conf.d/      # Configuraciones de virtual hosts
│   ├── html/
│   └── nginx.exe
├── mysql/               # MySQL/MariaDB (excluido de Git)
│   └── 8.4.6/
│       ├── bin/
│       └── data/
├── php/                 # Versiones de PHP (excluido de Git)
│   ├── php-7.4.33/
│   ├── php-8.3.26/
│   └── php-8.4.13/
└── services/            # Servicios de Windows
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

## 🚀 Instalación

> **📝 Nota:** Este repositorio **NO incluye** los binarios de Nginx, MySQL o PHP. Solo contiene la estructura, configuraciones y el gestor de servicios.

### 1. Clonar el repositorio

```powershell
git clone <repository-url>
cd server
```

---

### 2. Instalar MySQL

1. **Descargar:**
   - https://dev.mysql.com/downloads/mysql/ (ZIP Archive para Windows)

2. **Extraer:**
   ```powershell
   # Extraer en /mysql/8.4.6/ (o versión correspondiente)
   ```

3. **Inicializar base de datos:**
   ```powershell
   cd mysql\8.4.6\bin
   .\mysqld --initialize-insecure --datadir=..\data
   ```
   
   > Usuario por defecto: `root` (sin contraseña)

4. **Crear servicio:**
   ```powershell
   cd ..\..\..\..\services\mysql
   copy ..\winsw.exe mysql-service.exe
   .\mysql-service.exe install
   .\mysql-service.exe start
   ```

---

### 3. Instalar Nginx

1. **Descargar:**
   - https://nginx.org/en/download.html (versión Windows)

2. **Extraer:**
   ```powershell
   # Extraer todo el contenido en /nginx/
   ```

3. **Configurar:**
   ```powershell
   # Crear carpeta para virtual hosts
   mkdir nginx\conf\conf.d
   ```

4. **Editar `nginx\conf\nginx.conf`:**
   
   Agregar al final del bloque `http`, antes de la llave de cierre `}`:
   ```nginx
   include conf.d/*.conf;
   ```

5. **Crear servicio:**
   ```powershell
   cd services\nginx
   copy ..\winsw.exe nginx-service.exe
   .\nginx-service.exe install
   .\nginx-service.exe start
   ```

---

### 4. Instalar PHP

1. **Descargar:**
   - https://windows.php.net/download/ (Thread Safe, x64)

2. **Extraer:**
   ```powershell
   # Extraer en /php/php-8.4.13/ (o versión deseada)
   ```

3. **Configurar php.ini:**
   ```powershell
   cd php\php-8.4.13
   copy php.ini-development php.ini
   
   # Editar php.ini y activar extensiones:
   # extension=curl
   # extension=mysqli
   # extension=mbstring
   # etc.
   ```

4. **Crear servicio (ejemplo PHP 8.4):**
   
   Crear `/services/php/84/php84-service.xml`:
   ```xml
   <service>
       <id>php84</id>
       <name>PHP 8.4</name>
       <description>Servidor PHP 8.4 ejecutado como servicio</description>
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

5. **Instalar servicio:**
   ```powershell
   cd services\php\84
   copy ..\..\winsw.exe php84-service.exe
   .\php84-service.exe install
   .\php84-service.exe start
   ```

> **⚠️ Importante:** PHP 8.4+ requiere **Visual C++ Redistributable 2022**: https://aka.ms/vs/17/release/vc_redist.x64.exe

---

### 5. Compilar Manager

```powershell
cd manager
.\compile.bat
```

Esto creará `bin\server.exe`

---

### 6. Agregar Manager al PATH

**PowerShell (Usuario actual):**
```powershell
[Environment]::SetEnvironmentVariable("PATH", $env:PATH + ";C:\lnx\server\manager\bin", "User")
```

**O manualmente:**
1. Abrir "Variables de entorno del sistema"
2. Editar variable `PATH` del usuario
3. Agregar: `C:\lnx\server\manager\bin`
4. **Reiniciar PowerShell**

**Verificar:**
```powershell
server --help
```

---

## 🎯 Uso del Manager

### Comandos básicos

```powershell
# Sintaxis
server <servicio> <acción>

# Acciones disponibles: start, stop, restart, status, install, uninstall
```

### Gestión de servicios

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

### Gestión de hosts

```powershell
# Agregar dominio al archivo hosts de Windows
server host test.local
server host blog.local
server host api.local

# Nota: Requiere permisos de administrador
```

### Puertos por defecto

| Servicio | Puerto |
|----------|--------|
| Nginx    | 80     |
| MySQL    | 3306   |
| PHP 7.1  | 9071   |
| PHP 7.2  | 9072   |
| PHP 7.4  | 9074   |
| PHP 8.3  | 9083   |
| PHP 8.4  | 9084   |

---

## 🌐 Configuración de Virtual Hosts

### Crear un virtual host

1. **Crear configuración:**

`/nginx/conf/conf.d/miproyecto.conf`:
```nginx
server {
    listen       80;
    server_name  miproyecto.local;
    root html/miproyecto;
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

2. **Agregar dominio:**
```powershell
server host miproyecto.local
```

3. **Crear proyecto:**
```powershell
mkdir nginx\html\miproyecto
echo "<?php phpinfo();" > nginx\html\miproyecto\index.php
```

4. **Reiniciar Nginx:**
```powershell
server nginx restart
```

5. **Acceder:** http://miproyecto.local

---

## 📦 Composer - Gestor de dependencias PHP

### Instalación

1. **Descargar e instalar:**
   - https://getcomposer.org/download/
   - Ejecutar `Composer-Setup.exe`
   - Seleccionar cualquier versión de PHP (ej: `C:\lnx\server\php\php-8.4.13\php.exe`)

2. **Verificar:**
   ```powershell
   composer --version
   ```

### Configurar alias para múltiples versiones

**Editar perfil de PowerShell:**
```powershell
notepad $PROFILE
```

**Agregar alias:**
```powershell
# Alias para versiones de PHP
Set-Alias -Name php74 -Value 'C:\lnx\server\php\php-7.4.33\php.exe'
Set-Alias -Name php83 -Value 'C:\lnx\server\php\php-8.3.26\php.exe'
Set-Alias -Name php84 -Value 'C:\lnx\server\php\php-8.4.13\php.exe'

# Alias para composer.phar
function composer-phar { & "C:\ProgramData\ComposerSetup\bin\composer.phar" $args }
Set-Alias -Name cphar -Value composer-phar
```

**Recargar perfil:**
```powershell
. $PROFILE
```

### Uso con diferentes versiones de PHP

```powershell
# Instalar dependencias con PHP 7.4
cd C:\lnx\server\nginx\html\proyecto-viejo
php74 cphar install

# Instalar dependencias con PHP 8.4
cd C:\lnx\server\nginx\html\proyecto-nuevo
php84 cphar install

# Actualizar dependencias
php84 cphar update

# Agregar paquetes
php83 cphar require vendor/package

# Autoload
php83 cphar dump-autoload
```

### Verificar versión

```powershell
php74 --version
php83 --version
php84 --version
```

---

## 💡 Gestión Avanzada

Nota: La filosofía del proyecto es soportar múltiples instalaciones simultáneas de servicios. Esto aplica para PHP, pero también para MySQL y Nginx (por ejemplo, varios servidores Nginx con diferentes configuraciones/puertos). Apache puede integrarse igualmente si se desea, aunque no se documenta aquí.

### Agregar una nueva versión de PHP

Para ejecutar múltiples versiones simultáneamente:

#### 1. Descargar e instalar

```powershell
# Descargar: https://windows.php.net/download/ (Thread Safe, x64)
# Extraer en: /php/php-7.4.33/

cd php\php-7.4.33
copy php.ini-development php.ini
```

#### 2. Crear servicio

```powershell
mkdir services\php\74
cd services\php\74
copy ..\..\winsw.exe php74-service.exe
```

Crear `php74-service.xml`:
```xml
<service>
    <id>php74</id>
    <name>PHP 7.4</name>
    <description>Servidor PHP 7.4 ejecutado como servicio</description>
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

> **⚠️** Cada versión debe usar un puerto único (9074, 9083, 9084, etc.)

#### 3. Instalar e iniciar

```powershell
.\php74-service.exe install
.\php74-service.exe start

# O con el manager:
server php74 install
server php74 start
```

#### 4. Configurar Nginx

Cada proyecto elige su versión por puerto:

```nginx
# Proyecto con PHP 7.4
server {
    server_name  viejo.local;
    location ~ \.php$ {
        fastcgi_pass   127.0.0.1:9074;  # PHP 7.4
        # ...
    }
}

# Proyecto con PHP 8.4
server {
    server_name  nuevo.local;
    location ~ \.php$ {
        fastcgi_pass   127.0.0.1:9084;  # PHP 8.4
        # ...
    }
}
```

#### 5. Reiniciar Nginx

```powershell
server nginx restart
server php74 status
server php84 status
```

### Acceder a MySQL

```powershell
cd mysql\8.4.6\bin
.\mysql -u root

# Cambiar contraseña de root:
ALTER USER 'root'@'localhost' IDENTIFIED BY 'tu_contraseña';
FLUSH PRIVILEGES;
```

### Backup de MySQL

```powershell
# Exportar
cd mysql\8.4.6\bin
.\mysqldump -u root -p nombre_db > backup.sql

# Importar
.\mysql -u root -p nombre_db < backup.sql
```

### Logs

Los logs se generan en:
- `/services/nginx/nginx-service_YYYY-MM-DD.err.log`
- `/services/mysql/mysql-service_YYYY-MM-DD.err.log`
- `/services/php/84/php84-service_YYYY-MM-DD.err.log`

---

## ⚠️ Troubleshooting

### Error: VCRUNTIME140.dll incompatible

PHP 8.4+ requiere Visual C++ Redistributable 2022:
```
https://aka.ms/vs/17/release/vc_redist.x64.exe
```

### Error: "No input file specified"

- Verificar que existe el archivo PHP
- Verificar `fastcgi_param SCRIPT_FILENAME` en configuración Nginx
- Verificar que el servicio PHP está corriendo: `server php84 status`

### Puerto ya en uso

```powershell
netstat -ano | findstr :9084
```

### Servicio no inicia

Revisar logs en `/services/<servicio>/*.err.log`

### Error al compilar manager (g++ no encontrado)

Instalar MinGW-w64:
- https://www.mingw-w64.org/downloads/
- O usar MSYS2: https://www.msys2.org/
- Agregar `bin` de MinGW al PATH del sistema

### Comando 'server' no encontrado

- Verificar ruta en PATH: `C:\lnx\server\manager\bin`
- Reiniciar PowerShell
- Verificar: `$env:PATH -split ';' | Select-String "manager"`

---

## 🤝 Contribuir

1. Fork del proyecto
2. Crear rama: `git checkout -b feature/nueva-funcionalidad`
3. Commit: `git commit -m 'Agregar nueva funcionalidad'`
4. Push: `git push origin feature/nueva-funcionalidad`
5. Pull Request

## 📄 Licencia

Este proyecto está bajo la licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

En resumen: puedes usar, modificar y distribuir este código libremente, incluso con fines comerciales, siempre que incluyas el aviso de copyright y la licencia.

## 🔗 Enlaces útiles

- [Nginx Documentation](https://nginx.org/en/docs/)
- [MySQL Documentation](https://dev.mysql.com/doc/)
- [PHP Documentation](https://www.php.net/docs.php)
- [WinSW Documentation](https://github.com/winsw/winsw)
- [Composer Documentation](https://getcomposer.org/doc/)
