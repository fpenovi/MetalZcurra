#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unordered_map>


#define MAX_CLIENTS 5

using namespace std;


typedef struct {
    int sockFileDescrpt;
    pthread_t* thread;
    char* user=NULL;
    char* clave=NULL;
} cliente_t;


void iniciarServidor(int* sockFD, unsigned short int numPuerto, sockaddr_in* serv_addr) {

	// Inicializo struct con todos los miembros en \0
	bzero(serv_addr, sizeof(*serv_addr));

	// Llamo a la funcion socket y obtengo un FD
	// AF_INET es para TCP/IP
	// SOCK_STREAM es para orientado a conexion (SOCK_DGRAM su contraparte)
	*sockFD = socket(AF_INET, SOCK_STREAM, 0);

	if (*sockFD < 0) {
		perror("ERROR --> No se pudo abrir socket\n");
		exit(EXIT_FAILURE);
	}

	serv_addr->sin_family = AF_INET;
	serv_addr->sin_addr.s_addr = INADDR_ANY;
	serv_addr->sin_port = htons(numPuerto);  // --> convierte de hostbyte order a network byte order

	// Aviso al SO que asocie el programa al socket creado
	if (bind(*sockFD, (const sockaddr *) serv_addr, sizeof(*serv_addr)) < 0) {
		perror("ERROR --> No se pudo hacer bind\n");
		close(*sockFD);
		exit(EXIT_FAILURE);
	}

	// Comienza a escuchar a los clientes que se quieren conectar y los va encolando
	if (listen(*sockFD, MAX_CLIENTS) < 0) {
		perror("ERROR --> Falla en listen\n");
		close(*sockFD);
		exit(EXIT_FAILURE);
	}

}


bool validarCliente(unordered_map<string, string> map, cliente_t* cliente){

    string strUser(cliente->user);
    if (!strUser.empty() && strUser[strUser.length()-1] == '\n'){
        strUser.erase(strUser.length()-1);
    }
    string strClave(cliente->clave);

    auto it = map.find(strUser);
    if (it == map.end()){
        return false;
    }
    if (map[strUser]!= strClave){
        return false;
    }
    return true;
}


void* procesarMensajes(void* arg) {

    char* linea;
    size_t len = 0;
    ssize_t bytesLeidos;
    ssize_t bytesEscritos;
    int sockNewFileDescrpt = ((cliente_t*) arg)->sockFileDescrpt;
    pthread_t* thread = ((cliente_t*) arg)->thread;
    FILE* mensajeCliente = fdopen(sockNewFileDescrpt, "r");
    bytesEscritos = write(sockNewFileDescrpt,"conectado al servidor\n", 30);
    while (true) {
        bytesLeidos = getline(&linea, &len, mensajeCliente);

        if (bytesLeidos < 0) {
            printf("Se desconecto un cliente\n");
            free(linea);
            break;
        }

        printf("Mensaje recibido del cliente: %s", linea);
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
    return NULL;
}


unordered_map<string, string> cargarUsuarios(string filename) {

	// Ver de usar lectura de archivos para C++ para evitar problemas de memoria dinamica...

    char* linea = NULL;
    size_t len = 0;
    unordered_map<string,string> usuariosMap;
	FILE* archivo = fopen(filename.c_str(), "r");

    while (getline(&linea, &len, archivo)!= -1){
        string usuario = strtok(linea,",");
        string password = strtok(NULL,",");
        usuariosMap[usuario] = password;
    }

	fclose(archivo);
    return usuariosMap;
}


int main(int argc, char** argv) {

	if (argc < 3) {
		fprintf(stderr, "Modo de Uso: %s <archivo-usuarios> <n° puerto>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

    int sockFileDescrpt, sockNewFileDescrpt;
    socklen_t pesoCliente;
    unsigned short int numPuerto;
    struct sockaddr_in serv_addr, cli_addr;
    cliente_t* cliente;

	numPuerto = (unsigned short) strtoull(argv[2], NULL, 0);	// convierto el string del n° puerto
    iniciarServidor(&sockFileDescrpt, numPuerto, &serv_addr);

    // A partir de acá el servidor espera clientes
    printf("ESPERANDO CLIENTES...\nPresione * para salir\n");

    pthread_t* thread = NULL;      // Threads en los que correran los clientes
    pthread_attr_t attr;                // Variables para crear un thread detached
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pesoCliente = sizeof(cli_addr);
    bool serverOn = true;

    unordered_map<string, string> usuariosMap = cargarUsuarios(argv[1]);

    while (serverOn) {

        // Se crea el nuevo file descriptor para el cliente que se conecta
        sockNewFileDescrpt = accept(sockFileDescrpt, (sockaddr *) &cli_addr, &pesoCliente);

        if (sockNewFileDescrpt < 0) {
            perror("ERROR --> No se pudo aceptar cliente\n");
            close(sockFileDescrpt);
            exit(1);
        }

        cliente = (cliente_t*) malloc(sizeof(cliente_t));

        if (!cliente)
            exit(EXIT_FAILURE);

        thread = (pthread_t*) malloc(sizeof(pthread_t));

        if (!thread)
            exit(EXIT_FAILURE);
        // valido usuario y clave si funciona permite el acceso
        // si no, se cierra el servidor tambien .
        size_t len=0 , bytesLeidos, bytesEscritos;
        cliente->sockFileDescrpt = sockNewFileDescrpt;
        cliente->thread = thread;
        FILE* mensajeCliente = fdopen(sockNewFileDescrpt, "r");

        bytesLeidos = getline(&cliente->user, &len, mensajeCliente);
        bytesLeidos = getline(&(cliente->clave), &len, mensajeCliente);

        if(!validarCliente(usuariosMap,cliente)){
            bytesEscritos = write(sockNewFileDescrpt,"fallo la conexion al sistema.\n", 30);
            close(sockNewFileDescrpt);
            free(cliente);
            continue;

        }

        // Cuando el cliente es aceptado, creo un nuevo thread
        if (pthread_create(thread, &attr, procesarMensajes, cliente) != 0) {
            perror("ERROR --> Creación de thread fallida\n");
            close(sockNewFileDescrpt);      // Rechazar este cliente
            free(thread);
            free(cliente);
        }

    }

    pthread_attr_destroy(&attr);
    close(sockFileDescrpt);
    close(sockNewFileDescrpt);
    return 0;
}
