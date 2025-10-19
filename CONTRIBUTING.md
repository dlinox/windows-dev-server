# Contribuir a LNX Server

¡Gracias por tu interés en contribuir a LNX Server! 🎉

Este documento proporciona pautas para contribuir al proyecto.

## 📋 Tabla de Contenidos

- [Código de Conducta](#código-de-conducta)
- [¿Cómo puedo contribuir?](#cómo-puedo-contribuir)
- [Reportar Bugs](#reportar-bugs)
- [Sugerir Mejoras](#sugerir-mejoras)
- [Pull Requests](#pull-requests)
- [Guía de Estilo](#guía-de-estilo)
- [Desarrollo Local](#desarrollo-local)

## 📜 Código de Conducta

Este proyecto se adhiere a un código de conducta. Al participar, se espera que mantengas un ambiente respetuoso y colaborativo.

**Comportamientos esperados:**
- ✅ Ser respetuoso con todos los colaboradores
- ✅ Aceptar críticas constructivas
- ✅ Enfocarse en lo mejor para la comunidad
- ✅ Mostrar empatía hacia otros miembros

**Comportamientos inaceptables:**
- ❌ Lenguaje ofensivo o discriminatorio
- ❌ Ataques personales
- ❌ Acoso público o privado
- ❌ Publicar información privada de otros

## 🤝 ¿Cómo puedo contribuir?

Hay muchas formas de contribuir a LNX Server:

### 1. Reportar Bugs
¿Encontraste un error? Ayúdanos a mejorarlo reportándolo.

### 2. Sugerir Mejoras
¿Tienes una idea para mejorar el proyecto? Compártela.

### 3. Mejorar la Documentación
- Corregir errores tipográficos
- Agregar ejemplos
- Traducir a otros idiomas
- Mejorar explicaciones

### 4. Escribir Código
- Corregir bugs
- Implementar nuevas características
- Optimizar código existente
- Agregar tests

### 5. Ayudar a Otros
- Responder preguntas en Issues
- Ayudar en discusiones
- Compartir el proyecto

## 🐛 Reportar Bugs

Antes de reportar un bug, por favor:

1. **Verifica** que estás usando la última versión
2. **Busca** en los Issues existentes para evitar duplicados
3. **Reproduce** el error en un entorno limpio

### Crear un Bug Report

Incluye la siguiente información:

```markdown
**Descripción del bug**
Una descripción clara y concisa del problema.

**Pasos para reproducir**
1. Ve a '...'
2. Haz clic en '...'
3. Ejecuta '...'
4. Ve el error

**Comportamiento esperado**
Qué esperabas que sucediera.

**Comportamiento actual**
Qué sucedió realmente.

**Capturas de pantalla**
Si aplica, agrega capturas de pantalla.

**Entorno:**
- OS: [ej. Windows 11]
- Versión de LNX Server: [ej. 1.0.0]
- Versión de PHP: [ej. 8.4.13]
- Versión de Nginx: [ej. 1.24.0]
- Versión de MySQL: [ej. 8.4.6]

**Logs**
Pega los logs relevantes de:
- `/services/nginx/nginx-service_YYYY-MM-DD.err.log`
- `/services/mysql/mysql-service_YYYY-MM-DD.err.log`
- `/services/php/XX/phpXX-service_YYYY-MM-DD.err.log`

**Información adicional**
Cualquier otro contexto sobre el problema.
```

## 💡 Sugerir Mejoras

### Antes de Sugerir

1. **Verifica** que la funcionalidad no existe ya
2. **Busca** en Issues para ver si ya fue sugerida
3. **Piensa** si la mejora beneficia a la mayoría de usuarios

### Crear una Sugerencia

```markdown
**¿Tu sugerencia está relacionada con un problema?**
Descripción clara del problema. Ej: "Me frustra cuando [...]"

**Describe la solución que te gustaría**
Descripción clara y concisa de lo que quieres que suceda.

**Describe alternativas consideradas**
Otras soluciones o características que consideraste.

**Contexto adicional**
Agrega contexto, capturas de pantalla, ejemplos, etc.
```

## 🔀 Pull Requests

### Proceso de Pull Request

1. **Fork** el repositorio
2. **Crea** una rama desde `main`:
   ```bash
   git checkout -b feature/mi-nueva-funcionalidad
   ```
3. **Realiza** tus cambios
4. **Compila** y verifica que todo funcione:
   ```powershell
   cd manager
   .\compile.bat
   ```
5. **Commit** tus cambios:
   ```bash
   git commit -m "Agregar: descripción clara del cambio"
   ```
6. **Push** a tu fork:
   ```bash
   git push origin feature/mi-nueva-funcionalidad
   ```
7. **Abre** un Pull Request

### Requisitos para Pull Requests

- ✅ Descripción clara de los cambios
- ✅ Referencia a Issues relacionados (si aplica)
- ✅ El código compila sin errores
- ✅ Sigue la guía de estilo
- ✅ Actualiza documentación si es necesario
- ✅ Commits con mensajes descriptivos

### Plantilla de Pull Request

```markdown
## Descripción
Descripción clara de los cambios realizados.

## Tipo de cambio
- [ ] Bug fix (cambio que corrige un issue)
- [ ] Nueva funcionalidad (cambio que agrega funcionalidad)
- [ ] Breaking change (cambio que rompe compatibilidad)
- [ ] Documentación

## ¿Cómo se ha probado?
Describe las pruebas realizadas.

## Checklist
- [ ] Mi código sigue el estilo del proyecto
- [ ] He realizado una auto-revisión del código
- [ ] He comentado el código en áreas complejas
- [ ] He actualizado la documentación
- [ ] Mis cambios no generan nuevas advertencias
- [ ] El código compila correctamente
- [ ] He probado en un entorno limpio
```

## 📝 Guía de Estilo

### C++ (Manager)

```cpp
// Nombres de variables: camelCase
int serviceCount = 0;
std::string serviceName = "nginx";

// Nombres de funciones: camelCase
void startService(const std::string& name) {
    // Código aquí
}

// Nombres de clases: PascalCase
class ServiceManager {
public:
    void start();
    void stop();
};

// Constantes: UPPER_CASE
const int MAX_SERVICES = 10;

// Indentación: 4 espacios
if (condition) {
    doSomething();
}
```

### PowerShell Scripts

```powershell
# Nombres de variables: PascalCase
$ServiceName = "nginx"

# Nombres de funciones: PascalCase con verbo
function Start-LocalService {
    param(
        [string]$Name
    )
    # Código aquí
}

# Indentación: 4 espacios
if ($condition) {
    Write-Host "Mensaje"
}
```

### XML (Configuración de Servicios)

```xml
<!-- Indentación: 4 espacios -->
<service>
    <id>service-name</id>
    <name>Service Display Name</name>
    <description>Service description</description>
</service>
```

### Mensajes de Commit

Formato recomendado:

```
Tipo: Descripción corta (máximo 50 caracteres)

Descripción detallada del cambio (opcional)
Puede tener múltiples líneas

Resolves #123
```

**Tipos:**
- `Add:` - Agregar nueva funcionalidad
- `Fix:` - Corregir un bug
- `Update:` - Actualizar funcionalidad existente
- `Docs:` - Solo cambios en documentación
- `Style:` - Formato, punto y coma faltante, etc.
- `Refactor:` - Refactorización de código
- `Test:` - Agregar tests
- `Chore:` - Mantenimiento, dependencias, etc.

**Ejemplos:**
```
Add: Soporte para múltiples versiones de MySQL

Fix: Error al iniciar servicio de PHP en Windows 11

Docs: Actualizar instrucciones de instalación de Composer

Refactor: Mejorar gestión de procesos en manager
```

## 🛠️ Desarrollo Local

### Configurar el Entorno

1. **Clonar el repositorio:**
   ```bash
   git clone https://github.com/tu-usuario/lnx-server.git
   cd lnx-server
   ```

2. **Instalar MinGW-w64:**
   - https://www.mingw-w64.org/downloads/
   - O usar MSYS2: https://www.msys2.org/

3. **Compilar el manager:**
   ```powershell
   cd manager
   .\compile.bat
   ```

4. **Probar cambios:**
   ```powershell
   .\bin\server.exe nginx status
   ```

### Estructura del Proyecto

```
server/
├── manager/          # Código fuente del gestor en C++
├── services/         # Configuraciones de servicios
├── nginx/           # Instalación de Nginx (no versionada)
├── mysql/           # Instalación de MySQL (no versionada)
└── php/             # Instalaciones de PHP (no versionadas)
```

### Probar Cambios

Antes de enviar un PR:

1. **Compilar sin errores:**
   ```powershell
   cd manager
   .\compile.bat
   ```

2. **Probar comandos básicos:**
   ```powershell
   .\bin\server.exe nginx status
   .\bin\server.exe mysql status
   .\bin\server.exe php84 status
   ```

3. **Verificar logs** que no haya errores

## 📞 Contacto

Si tienes preguntas sobre cómo contribuir:

- Abre un Issue con la etiqueta `question`
- Revisa la documentación en el README
- Revisa Issues y Discussions existentes

## 🙏 Reconocimientos

¡Todos los colaboradores son reconocidos! Tu nombre aparecerá en:
- La lista de contribuidores de GitHub
- El archivo AUTHORS (si lo creamos)
- Menciones en el CHANGELOG

---

**¡Gracias por contribuir a LNX Server!** 🚀

Mantenido por [Lino Puma](https://github.com/tu-usuario)
