#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define TAM_BUFFER 256


int main(int argc, char** argv) {
    int sockFileDescrpt, numPuerto;
    ssize_t bytesLeidos, bytesEscritos;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[TAM_BUFFER];


    while (true){
        char *linea = NULL;
        size_t tam_buf = 0;
        size_t res = 0;

        printf("1 : Conectarse\n");
        printf("2 : Salir\n");
        res = getline(&linea,&tam_buf,stdin);
        if (res == -1){
            perror("Error en getline\n");
            free(linea);
            continue;
        }

        if (linea[0] == '1') {
            free(linea);
            break;
        }
        else if (linea[0] == '2') {
            free(linea);
            exit(EXIT_SUCCESS);
        }
        free(linea);

    }

    if (argc < 3) {
        fprintf(stderr, "Modo de Uso: %s IP-hostname puerto\n", argv[0]);
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
        close(sockFileDescrpt);
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
        close(sockFileDescrpt);
        exit(1);
    }

    printf("Conectado al servidor, presione * para desconectarse\n");

    while (true) {

        printf("Escriba un mensaje para enviar al servidor: ");
        bzero(buffer, TAM_BUFFER);
        fgets(buffer, TAM_BUFFER, stdin);

        if (strlen(buffer)-1 < 2 && buffer[0] == '*' )
            break;

        // Mando mensaje al servidor
        bytesEscritos = write(sockFileDescrpt, buffer, TAM_BUFFER);

        if (bytesEscritos < 0) {
            perror("ERROR --> escribiendo al socket");
            close(sockFileDescrpt);
            exit(1);
        }

        // Leo la respuesta escrita por el servidor en el FD
        bzero(buffer, TAM_BUFFER);
        bytesLeidos = read(sockFileDescrpt, buffer, TAM_BUFFER);
        printf("%s", buffer);

        if (bytesLeidos < 0) {
            perror("ERROR --> leyendo de socket");
            exit(1);
        }
    }

    close(sockFileDescrpt);
    exit(0);
}