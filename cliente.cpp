#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

using namespace std;


int main(int argc, char** argv) {
    int sockFileDescrpt, numPuerto;
    ssize_t bytesLeidos, bytesEscritos;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr,"Modo de Uso: %s IP-hostname puerto\n", argv[0]);
        exit(0);
    }

    numPuerto = atoi(argv[2]);

    // Creo el socket para el cliente
    sockFileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFileDescrpt < 0) {
        perror("ERROR --> abriendo socket");
        exit(1);
    }

    // Obtengo el servidor pasando el IP
    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR --> no existe ese host\n");
        exit(0);
    }


    // Inicializar struct socket
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // Copio el address del server al struct del cliente
    bcopy((char*)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(numPuerto);

    // Conecto con el servidor
    if (connect(sockFileDescrpt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR --> conectando");
        exit(1);
    }

    while (true) {

        printf("Escriba un mensaje para enviar al servidor: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        // Mando mensaje al servidor
        bytesEscritos = write(sockFileDescrpt, buffer, strlen(buffer));

        if (bytesEscritos < 0) {
            perror("ERROR --> escribiendo al socket");
            exit(1);
        }

        // Leo la respuesta escrita por el servidor en el FD
        bzero(buffer, 256);
        bytesLeidos = read(sockFileDescrpt, buffer, 255);

        if (bytesLeidos < 0) {
            perror("ERROR --> leyendo de socket");
            exit(1);
        }
    }

    printf("%s\n", buffer);
    exit(0);
    return 0;
}