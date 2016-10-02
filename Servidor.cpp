#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unordered_map>
#include <vector>
#include "auxiliares.h"
#include "Mensaje.h"
#include "Log.h"
#define MAX_CLIENTS 6

using namespace std;

struct argthread {
    int clientFD= 0;
    pthread_t* thread= NULL;
    char* user=NULL;
    char* clave= NULL;
};


class NoSePudoCrearServidorException : public runtime_error {

public:
    NoSePudoCrearServidorException() : runtime_error("No se pudo crear el servidor") {
    }

};


class Servidor {

private:
    static int fileDescrpt;
    string nombreArchivoCsv;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t pesoCli_adrr;
    pthread_t threadControl;
    pthread_attr_t attr;
    bool serverOn = false;
    static vector<argthread_t *> conectados;
    static unordered_map<string, string> usuarios;
    static vector<Mensaje> mensajes;
    static pthread_mutex_t mutex_mensajes;
    static pthread_mutex_t mutex_login;
    static Log logger;

    static void *controlInput(void *serverStatus) {

        cout << "ESPERANDO CLIENTES" << endl << "Presione * para salir..." << endl;

        string input;
        while (true) {
            cin >> input;

            if (input.compare("*") == 0) {
                *((bool *) serverStatus) = false;
                cerrarConexiones();
                usleep(100000);
                shutdown(fileDescrpt, SHUT_RDWR);
                return NULL;
            }
        }
    }

    static void cerrarConexiones() {
        size_t numConexiones = conectados.size();

        for (size_t i = 0; i < numConexiones; i++) {
            argthread_t *actual = conectados[i];
            shutdown(actual->clientFD, SHUT_RDWR);
        }
    }

    void cargarUsuarios(string filename) {

        char* linea = NULL;
        size_t len = 0;
        FILE *archivo = fopen(filename.c_str(), "r");

        while (getline(&linea, &len, archivo) != -1) {
            string usuario = strtok(linea, ",");
            string password = strtok(NULL, ",");
            usuarios[usuario] = password;
            free(linea);
            linea = NULL;
        }

        free(linea);
        fclose(archivo);
    }

    static bool esValido(string usuario, string clave) {
        cout << usuario << " " << clave << endl;
        usuario.erase(usuario.length() - 1);
        if (usuarios.find(usuario) == usuarios.end())
            return false;

        return (usuarios[usuario] == clave);
    }

    static bool pedirLogin(FILE *mensajeCliente, argthread_t *arg) {

        size_t len = 0;
        char* user = NULL;
        char* pass = NULL;
        ssize_t bytesLeidos=0;

        // Pido el usuario al cliente
        bytesLeidos=getline(&user, &len, mensajeCliente);
        if (bytesLeidos<=0){
            perror("ERROR --> Al leer nombre de usuario\n");
            return false;
        }
        bytesLeidos=getline(&pass, &len, mensajeCliente);
        if (bytesLeidos<=0){
            perror("ERROR --> Al leer contrasena\n");
            return false;
        }

         int result = pthread_mutex_lock(&mutex_login);
         if (result != 0) perror("Fallo el pthread_mutex_lock en login");

        // Chequeo si el user ya esta conectado
        vector<argthread_t *>::iterator it;
        for (it = conectados.begin(); it != conectados.end();) {
            if (it.operator*()->user != NULL){
                if (strcmp(it.operator*()->user,user) == 0){
                    cout << user << "Ya esta conectado" << endl;
                    free(user);
                    free(pass);
                    result = pthread_mutex_unlock(&mutex_login);
                    if (result != 0) perror("Fallo el pthread_mutex_unlock en login");
                    return false;
                }
            }
            it++;
        }

        string user_s(user);
        string pass_s(pass);

        if (esValido(user_s, pass_s)){
            arg->user = user;
            arg->clave = pass;
            result = pthread_mutex_unlock(&mutex_login);
            if (result != 0) perror("Fallo el pthread_mutex_unlock en login");
            return true;
        }

        cout << "User o pass fallidos" << endl;
        free(user);
        free(pass);
        result = pthread_mutex_unlock(&mutex_login);
        if (result != 0) perror("Fallo el pthread_mutex_unlock en login");
        return false;
    }

    static void mandarUsuarios(int sockNewFileDescrpt) {
        //Consigo las claves del hash de usuarios
        vector<string> keys;
        for (auto kv : usuarios) {
            keys.push_back(kv.first);
        }

        //Los junto en un vector separados por un espacio
        string texto;
        for (int i = 0; i < keys.size(); i++) {
            texto += keys[i] + ",";
        }
        texto += "\n";

        //Transformo string en char*
        char *textoUsuarios = new char[texto.length() + 1];
        strcpy(textoUsuarios, texto.c_str());

        //Mando el vector al cliente
        ssize_t bytesEscritos = write(sockNewFileDescrpt, textoUsuarios, strlen(textoUsuarios));
        delete textoUsuarios;

        if (bytesEscritos < 0)
            perror("ERROR --> No se pudo responder al cliente");
    }

    static void kickearUsuario(argthread_t* arg) {

        vector<argthread_t*>::iterator it;

        int result = pthread_mutex_lock(&mutex_login);
        if (result != 0) perror("Fallo el pthread_mutex_lock en kick");

        for (it = conectados.begin(); it != conectados.end();) {

            if (it.operator*()->user == NULL){
                it = conectados.erase(it);
                result = pthread_mutex_unlock(&mutex_login);
                if (result != 0) perror("Fallo el pthread_mutex_unlock en kick");
                return;
            }

            else if (it.operator*()->user != NULL && arg->user != NULL){
                if (strcmp(it.operator*()->user, arg->user) == 0){
                    free(it.operator*()->user);
                    free(it.operator*()->clave);
                    it = conectados.erase(it);
                    result = pthread_mutex_unlock(&mutex_login);
                    if (result != 0) perror("Fallo el pthread_mutex_unlock en kick");
                    return;
                }
            }

            it++;
        }

        result = pthread_mutex_unlock(&mutex_login);
        if (result != 0) perror("Fallo el pthread_mutex_unlock en kick");
    }

    static void agregarMensaje(char* emisorChar, char *textoInicial, ssize_t largo) {

        if (textoInicial == NULL){
            printf("ERROR");
            exit(1);
        }

        string emisor(emisorChar);
        emisor.erase(emisor.length()-1);

        //me fijo cuanto mide el destinatario
        int l=0;
        for (l; l<=largo; l++){
            if (textoInicial[l] == '$')
                break;
        }

        //sabiendo el largo del destinatario se el largo del mensaje
        char msg[largo-l];
        char dest[l];
        dest[l]='\0';
        //guardo el destinatario
        for (int d=0;d<l;d++) dest[d]=textoInicial[d];

        //guardo el mensaje
        l++;
        for(int m=0; l<=largo; m++){
            msg[m]=textoInicial[l];
            l++;
        }
        string destinatario(dest);
        string mensaje(msg);


        int result; // para el mutex
        if (destinatario == "TODOS") {
            for (auto kv : usuarios) {
                Mensaje mensajeNuevo(emisor,kv.first,mensaje);

                // Lockeo el mutex a mensajes
                result = pthread_mutex_lock(&mutex_mensajes);
                if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
                logger.loggearMsj(emisor,kv.first,mensaje);

                mensajes.push_back(mensajeNuevo);

                // Unlockeo el mutex a mensajes
                result = pthread_mutex_unlock(&mutex_mensajes);
                if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
            }
        }
        else {
         
            Mensaje mensajeNuevo(emisor,destinatario, mensaje);

            // Lockeo el mutex a mensajess
            result = pthread_mutex_lock(&mutex_mensajes);
            if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
            logger.loggearMsj(emisor,destinatario,mensaje);
            mensajes.push_back(mensajeNuevo);

            // Unlockeo el mutex a mensajes
            result = pthread_mutex_unlock(&mutex_mensajes);
            if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
        }
        return;
    }

    static bool enviarMensaje(argthread_t* arg, char* linea, ssize_t* bytesLeidos) {

        if (*bytesLeidos < 0) {
            cout << " SE DESCONECTÓ " << arg->user << endl;
            free(linea);
            return false;
        }

        agregarMensaje(arg->user, linea, *bytesLeidos);
        return true;

    }

    static void recibirMensajes(argthread_t* arg, ssize_t* bytesEscritos, int sockNewFileDescrpt) {

        string receptor(arg->user);
        receptor.erase(receptor.length() - 1);
        vector<Mensaje>::iterator it;

        int result; //para el mutex

        // Lockeo el mutex a mensajes
        result = pthread_mutex_lock(&mutex_mensajes);

        if (result != 0)
            perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

        logger.loggearRecepcion(receptor);

        for (it = mensajes.begin(); it != mensajes.end();) {
            if (it->getNameReceptor() == receptor) {
                string nombreDelEmisor = it->getNameEmisor();
                string mensajeEmisor = it->getMensaje();
                string texto = nombreDelEmisor + " dice: " + mensajeEmisor;
                char *mensaje = new char[texto.length() + 1];
                strcpy(mensaje, texto.c_str());

                //Mando el vector al cliente
                *bytesEscritos = write(sockNewFileDescrpt, mensaje, texto.length());
                delete mensaje;

                if (*bytesEscritos < 0) {
                    perror("ERROR --> Cliente se desconectó inesperadamente");
                    break;
                }
                it = mensajes.erase(it);
            }
            else it++;
        }

        // Unlockeo el mutex a mensajes
        result = pthread_mutex_unlock(&mutex_mensajes);
        if (result != 0)
            perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

        const char* fin = "$\n";

        if (*bytesEscritos >= 0)
            if (write(sockNewFileDescrpt, fin, strlen(fin)) < 0 ) {
                perror("ERROR --> Escritura de fin de mensajes");
                // ToDo logger
            }
    }

    static void *procesarMensajes(void *arg) {
        char *linea;
        size_t len = 0;
        ssize_t bytesLeidos;
        ssize_t bytesEscritos;
        int sockNewFileDescrpt = ((argthread_t *) arg)->clientFD;
        pthread_t *thread = ((argthread_t *) arg)->thread;
        FILE *mensajeCliente = fdopen(sockNewFileDescrpt, "r");

        // Registro del usuario
        if (!pedirLogin(mensajeCliente, (argthread_t *) arg)) {
            if (write(sockNewFileDescrpt, "fallo la conexion al sistema.\n", 30) < 0 ) {
                perror("ERROR --> No se pudo informar de error al cliente");
                // ToDo logger
            }
            fclose(mensajeCliente);
            close(sockNewFileDescrpt);
            kickearUsuario((argthread_t*) arg);
            free(arg);
            free(thread);
            return NULL;    // Salgo del thread
        }

        bytesEscritos = write(sockNewFileDescrpt, "conectado al servidor\n", 22);
        string userCon(((argthread_t *) arg)->user);
        userCon.erase(userCon.length()-1);
        logger.loggearConexion(userCon);
        cout << "\033[1m\033[32mSe conectó \033[1m\033[32m" << ((argthread_t *) arg)->user << "\033[0m";
        mandarUsuarios(sockNewFileDescrpt);     // Mando lista de usuarios al cliente por unica vez

        while (true) {

            // SETEO RESTRICCION DE TIMEOUT PARA EL PEDIDO DE INSTRUCCION/MENSAJES
            fd_set read_fds, write_fds, except_fds;
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            FD_ZERO(&except_fds);
            FD_SET(sockNewFileDescrpt, &read_fds);
            struct timeval timeout;

            timeout.tv_sec = 10;
            timeout.tv_usec = 0;

            // Uso el select para esperar el heartbeat del cliente
            if (int rv = select(sockNewFileDescrpt + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
                // Primer getline para recibir instruccion
                bytesLeidos = getline(&linea, &len, mensajeCliente);
            }

            else if (rv == 0) {
                cout << "\033[1;31mSe perdió la conexión con " << ((argthread_t *) arg)->user << "\033[0m";
                string userDesc(((argthread_t *) arg)->user);
                userDesc.erase(userDesc.length()-1);
                logger.loggearDesconexionViolenta(userDesc);
                free(linea);
                linea = NULL;
                break;
            }

            else{
                perror("ERROR --> select");
                free(linea);
                linea = NULL;
                break;
            }

            if (bytesLeidos < 0) {
                string userDesc(((argthread_t *) arg)->user);
                userDesc.erase(userDesc.length()-1);
                logger.loggearDesconexionViolenta(userDesc);
                cout << "\033[1;31mSe desconectó MAL " << ((argthread_t *) arg)->user << "\033[0m";
                free(linea);
                break;
            }

            cout << "debug: Mensaje recibido del cliente: " << linea;

            // Opcion enviar mensaje
            if (strcmp(linea, "/E/\n") == 0) {
                free(linea);
                linea = NULL;
                bytesLeidos = getline(&linea, &len, mensajeCliente);

                // ENVIO MENSAJE
                if ( !enviarMensaje((argthread_t*) arg, linea, &bytesLeidos) )
                    break;
            }

                // Opcion recibir msjs
            else if (strcmp(linea, "/R/\n") == 0) {
                free(linea);
                linea = NULL;
                recibirMensajes((argthread_t*) arg, &bytesEscritos, sockNewFileDescrpt);
            }

                // Desconecto al cliente desde el servidor
            else if (strcmp(linea, "/D/\n") == 0) {
                free(linea);
                linea = NULL;
                string userDesc(((argthread_t *) arg)->user);
                userDesc.erase(userDesc.length()-1);
                logger.loggearDesconexionNormal(userDesc);
                cout << "\033[32mSe desconectó BIEN \033[32m" << ((argthread_t*) arg)->user << "\033[0m";
                break;
            }

            else if (strcmp(linea, "/Z/\n") == 0) {
                if ((bytesEscritos = write(sockNewFileDescrpt, "/Z/\n", 4)) <= 0) {
                    cout << "El cliente se desconecto del servidor1" << endl;
                    break;
                }
            }

            else if (strcmp(linea, "/H/\n") == 0) {
                free(linea);
                linea = NULL;
            }

            else {
                cout << "El cliente se desconecto del servidor2" << endl;
                free(linea);
                linea = NULL;
                break;
            }

            free(linea);
            linea = NULL;

        }

        close(sockNewFileDescrpt);
        kickearUsuario((argthread_t*) arg);
        free(arg);
        free(thread);
        fclose(mensajeCliente);
        return NULL;
    }

public:
    Servidor(unsigned short int numPuerto, string nombreArchivo) {

        nombreArchivoCsv = nombreArchivo;
        bzero(&attr, sizeof(attr));

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        mutex_mensajes = PTHREAD_MUTEX_INITIALIZER;
        mutex_login = PTHREAD_MUTEX_INITIALIZER;


        // Creo thread de CONTROL
        if (pthread_create(&threadControl, &attr, controlInput, &serverOn) != 0) {
            cerr << "ERROR --> No se pudo crear thread de control" << endl;
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }


        bzero(&serv_addr, sizeof(serv_addr));   // Inicializo structs
        bzero(&cli_addr, sizeof(cli_addr));

        fileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

        if (fileDescrpt < 0) {
            logger.error("no se pudo abrir el socket");
            cerr << "ERROR --> No se pudo abrir socket" << endl;
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(numPuerto);  // --> convierte de hostbyte order a network byte order

        // Aviso al SO que asocie el programa al socket creado
        if (bind(fileDescrpt, (const sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            logger.error("no se pudo crear servidor");
            close(fileDescrpt);
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }

        // Comienza a escuchar a los clientes que se quieren conectar y los va encolando
        if (listen(fileDescrpt, MAX_CLIENTS) < 0) {
            logger.error("se alcanzo la cant maxima de clientes");
            cerr << "ERROR --> Falla en listen" << endl;
            close(fileDescrpt);
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }

        serverOn = true;
        cargarUsuarios(nombreArchivo);
        logger.inicializoServer();

    }

    void aceptarClientes() {

        while (serverOn) {
            int newFileDescrpt = accept(fileDescrpt, (sockaddr *) &cli_addr, &pesoCli_adrr);
            if (newFileDescrpt < 0) {
                // cerr << "ERROR --> No se pudo aceptar cliente" << endl;
                // ToDo logger
                continue;
            }


            argthread_t *arg = (argthread_t *) malloc(sizeof(argthread_t));

            if (!arg) {
                close(newFileDescrpt);  // rechazo cliente
                cerr << "ERROR --> Falta memoria para cliente" << endl;
                continue;
            }

            pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t));

            if (!thread) {
                close(newFileDescrpt); // rechazo cliente
                cerr << "ERROR --> Falta memoria para thread de cliente" << endl;
                continue;
            }

            arg->clientFD = newFileDescrpt;
            arg->thread = thread;
            arg->user = NULL;
            arg->clave = NULL;
            conectados.push_back(arg);
            if (pthread_create(thread, &attr, procesarMensajes, arg) != 0) {
                cerr << "ERROR --> Creación de thread fallida" << endl;
                conectados.pop_back();
                close(newFileDescrpt);      // Rechazar este cliente
                free(thread);
                free(arg);
            }


        }
        logger.cierroServer();
        logger.cerrarLog();
    }
};

vector<argthread_t*> Servidor::conectados;
unordered_map<string, string> Servidor::usuarios;
int Servidor::fileDescrpt;
vector<Mensaje> Servidor::mensajes;
pthread_mutex_t Servidor::mutex_mensajes;
pthread_mutex_t Servidor::mutex_login;
Log Servidor::logger(100);

int main(int argc, char** argv) {


    if (argc < 3) {
        fprintf(stderr, "Modo de Uso: %s <archivo-usuarios> <n° puerto>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    unsigned short int numPuerto = (unsigned short) strtoull(argv[2], NULL, 0);

    Servidor server = Servidor(numPuerto, argv[1]);
    server.aceptarClientes();
    return 0;
}
