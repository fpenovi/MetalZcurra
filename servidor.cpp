#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define TAM_BUFFER 256

using namespace std;


int main(int argc, char** argv) {

    // 127.0.0.1       localhost

    int sockFileDescrpt, sockNewFileDescrpt;
    socklen_t pesoCliente;
    unsigned short int numPuerto;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[TAM_BUFFER];    // array para los mensajes
    ssize_t bytesLeidos;
    ssize_t bytesEscritos;


    // Llamo a la funcion socket y obtengo un FD
    // AF_INET es para TCP/IP
    // SOCK_STREAM es para orientado a conexion (SOCK_DGRAM su contraparte)
    sockFileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFileDescrpt < 0) {
        perror("ERROR --> No se pudo abrir socket\n");
        exit(1);
    }

    // Inicializo los structs de los socket con todos los miembros en \0
    bzero(&serv_addr, sizeof(serv_addr));
    numPuerto = 5001;   // Elijo el puerto

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(numPuerto);  // --> convierte de hostbyte order a network byte order


    // Aviso al SO que asocie el programa al socket creado
    if (bind(sockFileDescrpt, (const sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR --> No se pudo hacer bind\n");
        exit(1);
    }

    // Comienza a escuchar a los clientes que se quieren conectar y los va encolando
    if (listen(sockFileDescrpt, 5) < 0 ) {
        perror("ERROR --> Falla en listen\n");
        exit(1);
    }

    // A partir de acá el servidor espera clientes
    printf("ESPERANDO CLIENTES...\n");
    printf("Presione * para salir\n");
    pesoCliente = sizeof(cli_addr);


    bool serverOn = true;

    while (serverOn) {

        // Se crea el nuevo file descriptor para el cliente que se conecta
        sockNewFileDescrpt = accept(sockFileDescrpt, (sockaddr*) &cli_addr, &pesoCliente);


        if (sockNewFileDescrpt < 0) {
            perror("ERROR --> No se pudo aceptar cliente\n");
            exit(1);
        }


        while (true) {

            // Inicializo el buffer de mensajes
            bzero(buffer, TAM_BUFFER);
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

            bytesEscritos = write(sockNewFileDescrpt, "Llego tu msj OK!", 16);

            if (bytesEscritos < 0) {
                perror("ERROR --> No se pudo responder al cliente");
                exit(1);
            }

        }

    }

    return 0;
}