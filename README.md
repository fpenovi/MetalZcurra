# VERSION EN ECLIPSE CON SUBDIRECTORIOS

Proyecto subido con archivos de configuracion de eclipse para que el workspace les quede configurado una vez bajado.

### Para evitar problemas de compatibilidad, clonar el repo en:

/home/[usuario]/neonWorkspaces/Taller1


### Link de version de Eclipse a utilizar:
http://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/neon/1a/eclipse-cpp-neon-1a-linux-gtk-x86_64.tar.gz&mirror_id=576

__Instalando...__
* Bajar y descomprimir en escritorio
* Mover a la carpeta opt: $   sudo mv eclipse-neon-cpp /opt
* Crear acceso directo:
  * Ejecutar $  sudo gedit /usr/share/applications/eclipse-neon-cpp.desktop
  * Pegar las lineas: <br />
        [Desktop Entry]
        Encoding=UTF-8 <br />
        Exec=/opt/eclipse-neon-cpp/eclipse <br />
        Icon=/opt/eclipse-neon-cpp/icon.xpm <br />
        Type=Application <br />
        Terminal=false <br />
        Comment=Eclipse Integrated Development Environment <br />
        Name=Eclipse Neon <br />
        GenericName=eclipse-4.6 <br />
        StartupNotify=false <br />
        Categories=Development;IDE;C++; <br />
  
* Guardar gedit y cerrarlo
* Hacer Log Out y volver a Logearse y ya va a aparecer en la busqueda el Eclipse Neon :)

**1) Seleccionando Workspace**

![Imagen de workspace](http://img.ctrlv.in/img/16/10/31/5816b81ab172d.png)

**2) Seleccionando tipo de Ejecutable a compilar**

![Imagen de seleccion](http://img.ctrlv.in/img/16/10/31/5816b89805d8e.png)

_Project -> Build Configuration -> Set Active -> Debug/Release_
**Seleccionar ambos proyectos al mismo tiempo y asegurarse de cambiarle a ambos para que esten en el mismo**

**3) Compilacion**

Boton derecho en _Juego -> Build Project_

**4) Ejecucion**

_Click en la flechita para abajo (del bichito para debugger o del play para ejecucion normal y seleccionar el perfil de ejecución)_

**5) Clean**

_Boton derecho en Ambos proyectos -> Clean Project y si se quiere borrar las carpetas Release y Debug de ambos proyectos_
