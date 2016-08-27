#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define TAM_BUFFER 256
#define MAX_CLIENTS 5

using namespace std;


typedef struct {
    int sockFileDescrpt;
    int sockNewFileDescrpt;
} FD_t;


void* procesarMensajes(void* arg) {

    char buffer[TAM_BUFFER];    // array para los mensajes
    ssize_t bytesLeidos;
    ssize_t bytesEscritos;
    int sockNewFileDescrpt = ((FD_t*) arg)->sockNewFileDescrpt;

    while (true) {

        bzero(buffer, TAM_BUFFER);      // Inicializo el buffer de mensajes
        bytesLeidos = read(sockNewFileDescrpt, buffer, TAM_BUFFER - 1);

        if (bytesLeidos < 0) {
            perror("ERROR --> Lectura fallida\n");
            exit(1);
        }

        if (bytesLeidos == 0) {
            printf("No hay mas para leer\n");
            break;
        }

        printf("Mensaje recibido del cliente: %s", buffer);
        bytesEscritos = write(sockNewFileDescrpt,"\xE2\x9C\x93\n", 7);

        if (bytesEscritos < 0) {
            perror("ERROR --> No se pudo responder al cliente");
            exit(1);
        }
    }

    close(sockNewFileDescrpt);
    return NULL;
}


int main(int argc, char** argv) {

    int sockFileDescrpt, sockNewFileDescrpt;
    socklen_t pesoCliente;
    unsigned short int numPuerto;
    struct sockaddr_in serv_addr, cli_addr;
    FD_t fileDescriptors;

    // Inicializo los structs con todos los miembros en \0
    bzero(&fileDescriptors, sizeof(fileDescriptors));
    bzero(&serv_addr, sizeof(serv_addr));

    // Llamo a la funcion socket y obtengo un FD
    // AF_INET es para TCP/IP
    // SOCK_STREAM es para orientado a conexion (SOCK_DGRAM su contraparte)
    sockFileDescrpt = socket(AF_INET, SOCK_STREAM, 0);
    fileDescriptors.sockFileDescrpt = sockFileDescrpt;

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

    bool serverOn = true;
    pthread_t clientes[MAX_CLIENTS];

    pthread_attr_t attr;                // Variables para crear un thread detached
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int threadActual = 0;

    while (serverOn) {

        // Se crea el nuevo file descriptor para el cliente que se conecta
        sockNewFileDescrpt = accept(sockFileDescrpt, (sockaddr *) &cli_addr, &pesoCliente);
        fileDescriptors.sockNewFileDescrpt = sockNewFileDescrpt;

        if (sockNewFileDescrpt < 0) {
            perror("ERROR --> No se pudo aceptar cliente\n");
            close(sockFileDescrpt);
            exit(1);
        }

        // Cuando el cliente es aceptado, creo un nuevo thread
        if (pthread_create(&clientes[threadActual], &attr, procesarMensajes, &fileDescriptors) != 0) {
            perror("ERROR --> Creación de thread fallida\n");
            close(sockNewFileDescrpt);      // Rechazar este cliente
            continue;
        }

        threadActual++;
    }

    close(sockFileDescrpt);
    close(sockNewFileDescrpt);
    return 0;
}
