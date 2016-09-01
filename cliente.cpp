#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

using namespace std;

typedef struct userClave{
    char* usuario=NULL;
    char* clave=NULL;
}userClave_t;

userClave_t solicitarUserClave(){
    userClave_t userClave;
    ssize_t bytesLeidos;
    size_t largo = 0;

    printf("Ingrese nombre de usuario:");
    bytesLeidos = getline(&(userClave.usuario), &largo, stdin);
    if (bytesLeidos < 0) {
        perror("ERROR --> Nombre de usuario\n");
        free(userClave.usuario);
    }
    printf("Ingrese clave:");
    bytesLeidos = getline(&(userClave.clave), &largo, stdin);
    if (bytesLeidos < 0) {
        perror("ERROR --> Clave\n");
        free(userClave.clave);
    }
    return userClave;
}

void mostrarMenuInicial() {

    while (true) {

        char* linea = NULL;
        size_t tam_buf = 0;
        size_t res = 0;

        printf("1 : Conectarse\n");
        printf("2 : Salir\n");
        res = getline(&linea, &tam_buf, stdin);

        if (res == -1) {
            perror("ERROR --> End of File o error de lectura\n");
        }

        else if (linea[0] == '1') {
            free(linea);
            break;
        }

        else if (linea[0] == '2') {
            free(linea);
            exit(EXIT_SUCCESS);
        }

        free(linea);
    }
}

void mandar_a_servidor(int fd,char* linea, int largo){
    size_t bytesEsc = write(fd, linea, largo);
    if (bytesEsc < 0) {
        perror("ERROR --> escribiendo al socket");
        close(fd);
        exit(1);
    }
}

int main(int argc, char** argv) {
    int sockFileDescrpt, numPuerto;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char* linea = NULL;
    size_t len = 0;


    if (argc < 3) {
        fprintf(stderr, "Modo de Uso: %s IP-hostname puerto\n", argv[0]);
        exit(0);
    }

    mostrarMenuInicial();

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

    FILE* respuestaServidor = fdopen(sockFileDescrpt, "r");

    /* Manejo el envio de usuario y contrasena al server*/
    userClave_t userClaveIngresada = solicitarUserClave();

    //mando el usuario al servidor
    mandar_a_servidor(sockFileDescrpt,userClaveIngresada.usuario,strlen(userClaveIngresada.usuario));

    //mando la clave al servidor
    mandar_a_servidor(sockFileDescrpt,userClaveIngresada.clave,strlen(userClaveIngresada.clave));

    //ahora lee del servidor si el usuario y contra existen
    size_t bytesLeidos = getline(&linea, &len, respuestaServidor);
    printf("%s",linea);

    //este cmp es re negro
    if(strcmp(linea,"fallo la conexion al sistema.\n")==0) {
        printf("%s", linea);
        free(linea);
        free(userClaveIngresada.clave);
        free(userClaveIngresada.usuario);
        close(sockFileDescrpt);
        fclose(respuestaServidor);
        exit(0);
    }


    while (true) {
        ssize_t bytesLeidos, bytesEscritos;
        char* linea = NULL;
        size_t len = 0;
        printf("Escriba un mensaje para enviar al servidor: ");
        bytesLeidos = getline(&linea, &len, stdin);

        if (bytesLeidos < 0) {
            perror("ERROR --> EOF o error en lectura\n");
            free(linea);
            free(userClaveIngresada.clave);
            free(userClaveIngresada.usuario);

        }

        //si mando '*' entonces sale del programa
        if (bytesLeidos-1 < 2 && linea[0] == '*' ) {
            free(linea);
            free(userClaveIngresada.clave);
            free(userClaveIngresada.usuario);
            break;
        }

        // Mando mensaje al servidor
        mandar_a_servidor(sockFileDescrpt,linea,bytesLeidos);
        free(linea);    // Una vez que mando el mensaje, libero la linea e inicializo en NULL;
        linea = NULL;

        // Leo la respuesta escrita por el servidor en el FD
        bytesLeidos = getline(&linea, &len, respuestaServidor);
        printf(" %s", linea);

        if (bytesLeidos < 0) {
            perror("ERROR --> leyendo de socket");
            free(linea);
            exit(1);
        }

        free(linea);
        linea = NULL;
    }

    free(linea);
    close(sockFileDescrpt);
    fclose(respuestaServidor);
    exit(0);
}