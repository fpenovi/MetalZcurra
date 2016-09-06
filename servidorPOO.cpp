#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unordered_map>
#include <vector>
#include "auxiliares.h"

#define MAX_CLIENTS 5

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


class servidorPOO {

private:
    static int fileDescrpt;
    string nombreArchivoCsv;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t pesoCli_adrr;
    pthread_t threadControl;
    pthread_attr_t attr;
    bool serverOn = false;
    static vector<argthread_t*> conectados;
    static unordered_map<string, string> usuarios;
    // Log logger; --> un atributo va a ser un objeto Log para logear.


    static void* controlInput(void* serverStatus) {

        cout << "ESPERANDO CLIENTES" << endl << "Presione * para salir..." << endl;

        string input;
        while (true) {
            cin >> input;

            if (input.compare("*") == 0) {
                *((bool*) serverStatus) = false;
                cerrarConexiones();
                shutdown(fileDescrpt, SHUT_RDWR);
                return NULL;
            }
        }
    }

    static void cerrarConexiones() {
        size_t numConexiones = conectados.size();

        for (size_t i=0; i<numConexiones; i++) {
            argthread_t* actual = conectados[i];
            shutdown(actual->clientFD, SHUT_RDWR);
            // No libero el resto de las cosas de argthread porque se liberan
            // al final de la funcion procesarMensajes
        }
    }

    void cargarUsuarios(string filename) {

        char* linea = NULL;
        size_t len = 0;
        FILE* archivo = fopen(filename.c_str(), "r");

        while (getline(&linea, &len, archivo)!= -1){
            string usuario = strtok(linea,",");
            string password = strtok(NULL,",");
            usuarios[usuario] = password;
        }

        fclose(archivo);
    }

    static bool esValido(string usuario, string clave) {
        usuario.erase(usuario.length()-1);
        if (usuarios.find(usuario) == usuarios.end())
            return false;

        return (usuarios[usuario] == clave);
    }

    static bool pedirLogin(FILE* mensajeCliente, argthread_t* arg) {
        size_t len = 0;
        // Pido el usuario al cliente
        getline(&arg->user, &len, mensajeCliente);
        getline(&arg->clave, &len, mensajeCliente);
        string user(arg->user);
        string pass(arg->clave);
        return esValido(user,pass);
    }

    static void* mandarUsuarios(int sockNewFileDescrpt){
        //Consigo las claves del hash de usuarios
        vector<string> keys;
        for(auto kv : usuarios) {
            keys.push_back(kv.first);
        }

        //Los junto en un vector separados por un espacio
        string texto;
        for(int i=0 ; i < keys.size() ; i++ ){
            texto += keys[i] + ",";
        }
        texto += "\n";

        //Transformo string en char*
        char* textoUsuarios = new char[texto.length()+1];
        strcpy(textoUsuarios,texto.c_str());

        //Mando el vector al cliente
        ssize_t bytesEscritos = write(sockNewFileDescrpt,textoUsuarios, strlen(textoUsuarios));
        delete textoUsuarios;

        if (bytesEscritos < 0) {
            perror("ERROR --> No se pudo responder al cliente");
            exit(1);
        }
    }

    static void* agregarMensaje(){
        //ToDo hacerrrrrrrrrrrrrr
    }

    static void* procesarMensajes(void* arg) {
        char* linea;
        size_t len = 0;
        ssize_t bytesLeidos;
        ssize_t bytesEscritos;
        int sockNewFileDescrpt = ((argthread_t*) arg)->clientFD;
        pthread_t* thread = ((argthread_t*) arg)->thread;
        FILE* mensajeCliente = fdopen(sockNewFileDescrpt, "r");

        if (!pedirLogin(mensajeCliente, (argthread_t*) arg) ) {
            write(sockNewFileDescrpt, "fallo la conexion al sistema.\n", 30);
            fclose(mensajeCliente);
            close(sockNewFileDescrpt);
            free(arg);
            free(thread);
            return NULL;    // Salgo del thread
        }

        bytesEscritos = write(sockNewFileDescrpt, "conectado al servidor\n", 22);
        // ToDo Escribir en el logger que se conecto
        cout << "Se conectó " << ((argthread_t*) arg)->user << endl;

        while (true) {
            bytesLeidos = getline(&linea, &len, mensajeCliente);

            if (bytesLeidos < 0) {
                cout << "Se desconectó " << ((argthread_t*) arg)->user << endl;
                free(linea);
                break;
            }

            cout << "Mensaje recibido del cliente: " << linea;

            if (strcmp(linea, "4\n") == 0) {
                mandarUsuarios(sockNewFileDescrpt);
                bytesLeidos = getline(&linea, &len, mensajeCliente);
                if (bytesLeidos < 0) {
                    cout << "Se desconectó " << ((argthread_t *) arg)->user << endl;
                    free(linea);
                    break;
                }
                agregarMensaje();
            }

            else if (strcmp(linea,"6\n")==0)
                mandarUsuarios(sockNewFileDescrpt);

            // ToDo Los otros casos del protocolo
            /*
            else if(strcmp(linea, "5\n") == 0); // Recibir msjs
            else if(strcmp(linea, "6\n") == 0); // Lorem
            else if(strcmp(linea, "2\n") == 0); // Desconectar desde el servidor tambien
            */

            free(linea);
            linea = NULL;

            bytesEscritos = write(sockNewFileDescrpt,"\xE2\x9C\x93\n", 4);

            if (bytesEscritos < 0) {
                perror("ERROR --> No se pudo responder al cliente");
                exit(1);
            }
        }

        close(sockNewFileDescrpt);
        free(arg);
        free(thread);
        fclose(mensajeCliente);
        cout << "Llego a liberar las cosas del argthread" << endl;
        return NULL;
    }

public:
    servidorPOO(unsigned short int numPuerto, string nombreArchivo) {

        nombreArchivoCsv = nombreArchivo;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        // Creo thread de CONTROL
        if (pthread_create(&threadControl, &attr, controlInput, &serverOn) != 0) {
            // ToDo logger...
            cerr << "ERROR --> No se pudo crear thread de control" << endl;
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }

        bzero(&serv_addr, sizeof(serv_addr));   // Inicializo structs
        bzero(&cli_addr, sizeof(cli_addr));

        fileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

        if (fileDescrpt < 0) {
            // ToDo pedirle al logger que escriba el error ( i.e. logger.error("yada yada") )
            cerr << "ERROR --> No se pudo abrir socket" << endl;
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(numPuerto);  // --> convierte de hostbyte order a network byte order

        // Aviso al SO que asocie el programa al socket creado
        if (bind(fileDescrpt, (const sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
            // ToDo pedirle al logger...
            close(fileDescrpt);
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }

        // Comienza a escuchar a los clientes que se quieren conectar y los va encolando
        if (listen(fileDescrpt, MAX_CLIENTS) < 0) {
            // ToDo pedirle al logger...
            cerr << "ERROR --> Falla en listen" << endl;
            close(fileDescrpt);
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException();
        }

        serverOn = true;
        cargarUsuarios(nombreArchivo);


    }

    void aceptarClientes() {

        while (serverOn) {
            int newFileDescrpt = accept(fileDescrpt, (sockaddr *) &cli_addr, &pesoCli_adrr);
            if (newFileDescrpt < 0) {
                // ToDo pedirle al logger...
                cerr << "ERROR --> No se pudo aceptar cliente" << endl;
                continue;
            }

            // ToDo falta en el caso de que no falle, crear los threads y agregarlos a la lista

            argthread_t* arg = (argthread_t*) malloc(sizeof(argthread_t));

            if (!arg){
                close(newFileDescrpt);  // rechazo cliente
                // ToDo logger...
                cerr << "ERROR --> Falta memoria para cliente" << endl;
                continue;
            }

            pthread_t* thread = (pthread_t*) malloc(sizeof(pthread_t));

            if (!thread) {
                close(newFileDescrpt); // rechazo cliente
                // ToDo logger...
                cerr << "ERROR --> Falta memoria para thread de cliente" << endl;
                continue;
            }

            arg->clientFD = newFileDescrpt;
            arg->thread = thread;
            conectados.push_back(arg);
            if (pthread_create(thread, &attr, procesarMensajes, arg) != 0) {
                cerr << "ERROR --> Creación de thread fallida" << endl;
                conectados.pop_back();
                close(newFileDescrpt);      // Rechazar este cliente
                free(thread);
                free(arg);
            }

        }
    }

};

vector<argthread_t*> servidorPOO::conectados;
unordered_map<string, string> servidorPOO::usuarios;
int servidorPOO::fileDescrpt;

int main(int argc, char** argv) {


    if (argc < 3) {
        fprintf(stderr, "Modo de Uso: %s <archivo-usuarios> <n° puerto>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    unsigned short int numPuerto = (unsigned short) strtoull(argv[2], NULL, 0);

    servidorPOO server = servidorPOO(numPuerto, argv[1]);
    server.aceptarClientes();

    return 0;
}