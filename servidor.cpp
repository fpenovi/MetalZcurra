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
} cliente_t;


void* procesarMensajes(void* arg) {

    char* linea;
    size_t len = 0;
    ssize_t bytesLeidos;
    ssize_t bytesEscritos;
    int sockNewFileDescrpt = ((cliente_t*) arg)->sockFileDescrpt;
    pthread_t* thread = ((cliente_t*) arg)->thread;
    FILE* mensajeCliente = fdopen(sockNewFileDescrpt, "r");

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

//Funcion que crea un hash del archivo (falta ver como devolverlo)
unordered_map<string,string> cargarUsuarios(FILE* archivo){
    char* linea = NULL;
    size_t len = 0;
    unordered_map<string,string> usuariosMap;
    while(getline(&linea, &len, archivo)!= -1){
        string usuario = strtok(linea,",");
        string password = strtok(NULL,",");
        usuariosMap[usuario] = password;
    }
    return usuariosMap;
}

int main(int argc, char** argv) {

    int sockFileDescrpt, sockNewFileDescrpt;
    socklen_t pesoCliente;
    unsigned short int numPuerto;
    struct sockaddr_in serv_addr, cli_addr;
    cliente_t* cliente;
    FILE* listaUsuarios;

    // Inicializo los structs con todos los miembros en \0
    bzero(&serv_addr, sizeof(serv_addr));

    // Llamo a la funcion socket y obtengo un FD
    // AF_INET es para TCP/IP
    // SOCK_STREAM es para orientado a conexion (SOCK_DGRAM su contraparte)
    sockFileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFileDescrpt < 0) {
        perror("ERROR --> No se pudo abrir socket\n");
        exit(1);
    }

    numPuerto = 5001;   // Elijo el puerto
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(numPuerto);  // --> convierte de hostbyte order a network byte order


    // Aviso al SO que asocie el programa al socket creado
    if (bind(sockFileDescrpt, (const sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR --> No se pudo hacer bind\n");
        close(sockFileDescrpt);
        exit(1);
    }

    // Comienza a escuchar a los clientes que se quieren conectar y los va encolando
    if (listen(sockFileDescrpt, MAX_CLIENTS) < 0) {
        perror("ERROR --> Falla en listen\n");
        close(sockFileDescrpt);
        exit(1);
    }

    // A partir de acá el servidor espera clientes
    printf("ESPERANDO CLIENTES...\n");
    printf("Presione * para salir\n");
    pesoCliente = sizeof(cli_addr);

    pthread_t* thread = NULL;      // Threads en los que correran los clientes
    pthread_attr_t attr;                // Variables para crear un thread detached
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    bool serverOn = true;

    // Aca comienza el parseo del archivo de usuarios
    listaUsuarios = fopen("usuarios.csv", "r");
    unordered_map<string,string> usuariosMap = cargarUsuarios(listaUsuarios);
    fclose(listaUsuarios);

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

        cliente->sockFileDescrpt = sockNewFileDescrpt;
        cliente->thread = thread;

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
