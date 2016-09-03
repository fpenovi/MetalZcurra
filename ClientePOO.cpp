#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
//#include <netinet/in.h>
#include <string.h>
//#include <unistd.h>
#include "ClientePOO.h"
#include <pthread.h>

using namespace std;



/*class Cliente {
private:
    // atributos
    char *name;
    char *clave;
    int sockFileDescrpt;
    //pthread_t* thread;
    FILE* respuestaServidor;

public:*/

    Cliente::Cliente(){
         name = NULL;
         clave = NULL;
         sockFileDescrpt = 0;
         //thread = NULL;
         FILE* respuestaServidor = NULL;
    }

    void Cliente::solicitarUserClave(){
        ssize_t bytesLeidos;
        size_t largo = 0;

        printf("Ingrese nombre de usuario:");
        bytesLeidos = getline(&(name), &largo, stdin);
        if (bytesLeidos < 0) {
            perror("ERROR --> Nombre de usuario\n");
            //free(userClave.usuario);
        }
        printf("Ingrese clave:");
        bytesLeidos = getline(&(clave), &largo, stdin);
        if (bytesLeidos < 0) {
            perror("ERROR --> Clave\n");
            //free(userClave.clave);
        }
    }

    void Cliente::mandar_a_servidor(char* linea, int largo){
        ssize_t bytesEsc = write(sockFileDescrpt, linea, largo);
        if (bytesEsc < 0) {
            perror("ERROR --> escribiendo al socket");
            close(sockFileDescrpt);
            exit(1);
        }
    }

    void Cliente::mandar_credencial_a_servidor(){

        char *linea=NULL;
        size_t len = 0;

        size_t bytesEsc = write(sockFileDescrpt, name, strlen(name));
        if (bytesEsc < 0) {
            perror("ERROR --> escribiendo al socket");
            close(sockFileDescrpt);
            exit(1);
        }

        size_t bytesEsc2 = write(sockFileDescrpt, clave, strlen(clave));
        if (bytesEsc2 < 0) {
            perror("ERROR --> escribiendo al socket");
            close(sockFileDescrpt);
            exit(1);
        }

        //ahora lee del servidor si el usuario y contra existen
        linea=NULL;
        cout << "CREDENCIAL. ANTES DE LEER RESPUESTA\n";
        size_t bytesLeidos = getline(&linea, &len, respuestaServidor);
        cout << "CREDENCIAL. DESPUES DE LEER RESPUESTA\n";

        //este cmp es re negro
        if (strcmp(linea, "fallo la conexion al sistema.\n") == 0) {
            printf("%s", linea);
            free(linea);
            free(name);
            free(clave);
            close(sockFileDescrpt);
            fclose(respuestaServidor);
            exit(0);
        }
        free(linea);

    }

    void Cliente::asignarFD(int fd){
        sockFileDescrpt = fd;
        respuestaServidor = fdopen(sockFileDescrpt, "r");
    }

    /*Cliente::activar_socket(char **parametros){
        struct sockaddr_in serv_addr;
        int numPuerto;
        struct hostent *server;

        numPuerto = atoi(parametros[2]);

        // Obtengo el servidor pasando el IP
        server = gethostbyname(parametros[1]);

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

        // Creo el socket para el cliente
        sockFileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

        if (sockFileDescrpt < 0) {
            perror("ERROR --> abriendo socket");
            exit(1);
        }

        // Conecto con el servidor
        if (connect(sockFileDescrpt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR --> conectando");
            close(sockFileDescrpt);
            return 0;
            //exit(1);
        }
    return 1;
    }*/

    void Cliente::recibir_de_servidor(){
        char *linea=NULL;
        size_t len = 0;
        size_t bytesLeidos;

        bytesLeidos = getline(&linea, &len, respuestaServidor);
        printf(" %s", linea);

        if (bytesLeidos < 0) {
            perror("ERROR --> leyendo de socket");
            free(linea);
            exit(1);
        }

        free(linea);
    }

    void Cliente::liberar(){
        printf("en el destructor del cliente");
        free(name);
        free(clave);
        close(sockFileDescrpt);
        fclose(respuestaServidor);
    }






int activar_socket(char **parametros){
    struct sockaddr_in serv_addr;
    int numPuerto;
    struct hostent *server;
    numPuerto = atoi(parametros[2]);

    // Creo el socket para el cliente
    int sockFileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFileDescrpt < 0) {
        perror("ERROR --> abriendo socket");
        exit(1);
    }

    // Obtengo el servidor pasando el IP
    server = gethostbyname(parametros[1]);

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
    return sockFileDescrpt;
}

int mostrar_menu() {
    string opcion;
    ssize_t bytesLeidos;
    size_t len = 0;
    while (true) {
        printf("1 : Conectar\n");
        printf("2 : Desconectar\n");
        printf("3 : Salir\n");
        printf("4 : Enviar\n");
        printf("5 : Recibir\n");
        printf("6 : Lorem Ipsum\n");
        break;

       /* bytesLeidos = getline(&opcion, &len, stdin);

        if (strcmp(opcion,"1")==0) return 1;
        else if (strcmp(opcion,"2")==0) return 2;
        else if (strcmp(opcion,"3")==0) return 3;
        else if (strcmp(opcion,"4")==0) return 4;
        else if (strcmp(opcion,"5")==0) return 5;
        else if (strcmp(opcion,"6")==0) return 6;
        printf("Intente otra vez\n");*/
    }
}


int main(int argc, char** argv) {
    int fd;
    if (argc < 3) {
        fprintf(stderr, "Modo de Uso: %s IP-hostname puerto\n", argv[0]);
        exit(0);
    }

    int respuesta = mostrar_menu();

    fd = activar_socket(argv);

    Cliente cliente;

    cliente.asignarFD(fd);

    // solicito usuario y contrasena al cliente
    cliente.solicitarUserClave();

    //mando el usuario y clave al servidor y manejo respuesta
    cliente.mandar_credencial_a_servidor();


    while (true) {
        ssize_t bytesLeidos;
        char *linea = NULL;
        size_t len = 0;

        printf("Escriba un mensaje para enviar al servidor: ");
        bytesLeidos = getline(&linea, &len, stdin);

        if (bytesLeidos < 0) {
            perror("ERROR --> EOF o error en lectura\n");
            free(linea);
            cliente.liberar();
        }

        //si mando '*' entonces sale del programa
        if (bytesLeidos - 1 < 2 && linea[0] == '*') {
            free(linea);
            break;
        }

        // Mando mensaje al servidor
        cliente.mandar_a_servidor(linea, bytesLeidos);
        free(linea);    // Una vez que mando el mensaje, libero la linea e inicializo en NULL;
        cliente.recibir_de_servidor();

    }
    cliente.liberar();
    exit(EXIT_SUCCESS);
}
