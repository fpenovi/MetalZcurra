#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "ClientePOO.h"
#include <pthread.h>
using namespace std;

Cliente::Cliente(char** argv){
    name = NULL;
    clave = NULL;
    sockFileDescrpt = 0;
    //thread = NULL;
    FILE* respuestaServidor = NULL;
    estado=false;
    string aux(argv[1]);
    strcpy(IP,aux.c_str());
    aux= argv[2];
    strcpy(port,aux.c_str());


}

bool Cliente::getEstado(){
    return estado;
}
void Cliente::cambiar_estado(bool nuevo_estado){
    estado = nuevo_estado;
}

void Cliente::solicitarUserClave(){
    ssize_t bytesLeidos;
    size_t largo = 0;

    printf("Ingrese nombre de usuario:");

    //ESTE SIGUIENTE GETLINE LEE BASURA QUE QUEDO DEL MENU INICIAL
    //QUE NO ENTIENDO PORQUE LAS LEE, CREO QUE ES UN \N
    bytesLeidos = getline(&(name), &largo, stdin);
    name == NULL;

    bytesLeidos = getline(&(name), &largo, stdin);
    if (bytesLeidos < 0) {
        perror("ERROR --> Nombre de usuario\n");
        free(name);
    }
    printf("Ingrese clave:");
    bytesLeidos = getline(&(clave), &largo, stdin);
    if (bytesLeidos < 0) {
        perror("ERROR --> Clave\n");
        free(clave);
        free(name);
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

    size_t bytesLeidos = getline(&linea, &len, respuestaServidor);

    //este cmp es re negro
    if (strcmp(linea, "fallo la conexion al sistema.\n") == 0) {
        printf("%s", linea);
        free(linea);
        liberar();
        return;
    }
    free(linea);
    estado = true;
    cout<<"La conexion con el servidor fue exitosa"<<endl;

}

void Cliente::asignarFD(){
    respuestaServidor = fdopen(sockFileDescrpt, "r");
}

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

void Cliente::recibir_mensajes(){
    printf("Recibo mensajes de la lista\n");
}

void Cliente::desconectar(){
    if (estado == false) {
        perror("El cliente ya se encuentra desconectado");
        return;
    }
    liberar();
}

void Cliente::salir(){
    if(estado) {
        liberar();
    }
    exit(0);
}

void Cliente::enviar(){
    if(! estado){
        cout<<"No esta conectado al servidor"<<endl;
        return;
    }
    ssize_t bytesLeidos;
    char *linea = NULL;
    size_t len = 0;
    // Agarra basura del menu, dejarlo , no se como limpiar el stdin
    // DATO CURIOSO : existe el cin.getline , que trabaja con strings en vez de con char y mem
    // dinamica, hubiese sido lindo saberlo N dias antes :D  -jC-
    bytesLeidos = getline(&linea, &len, stdin);

    printf("Escriba un mensaje para enviar al servidor: ");
    bytesLeidos = getline(&linea, &len, stdin);

    if (bytesLeidos < 0) {
        perror("ERROR --> EOF o error en lectura\n");
        free(linea);
        liberar();
    }

    // Mando mensaje al servidor
    mandar_a_servidor(linea, bytesLeidos);
    free(linea);    // Una vez que mando el mensaje, libero la linea e inicializo en NULL;
    //recibir_de_servidor();
}

void Cliente::lorem(){
    printf("ACA SE HACE EL LOREM");
}

void Cliente::liberar(){
    if (name!=NULL) {
        free(name);
        name = NULL;
    }
    if (clave!=NULL ) {
        free(clave);
        clave = NULL;
    }
    if(estado) {
        close(sockFileDescrpt);
        fclose(respuestaServidor);
    }
    estado = false;

}


void Cliente::activar_socket(){
    struct sockaddr_in serv_addr;
    int numPuerto;
    struct hostent *server;
    numPuerto = atoi(port);
    // Creo el socket para el cliente
    sockFileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFileDescrpt < 0) {
        perror("ERROR --> abriendo socket");
        exit(1);
    }

    // Obtengo el servidor pasando el IP
    server = gethostbyname(IP);

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
    return ;
}

void Cliente::mostrar_menu() {
    int opcion;
    ssize_t bytesLeidos;
    size_t len = 0;
    while (true) {
        cout<<endl;
        cout<<"//////////////////"<<endl;
        printf("1 : Conectar\n");
        printf("2 : Desconectar\n");
        printf("3 : Salir\n");
        printf("4 : Enviar\n");
        printf("5 : Recibir\n");
        printf("6 : Lorem Ipsum\n");
        cout<<"//////////////////"<<endl;
        cout<<endl;

        cin >> opcion;

        if (opcion==1) conectar() ;
        else if (opcion==2) desconectar();
        else if (opcion==3) salir() ;
        else if (opcion==4) return enviar();
        else if (opcion==5) return ;
        else if (opcion==6) return ;
        else printf("Intente otra vez\n");
    }
}

void Cliente::conectar() {
    if (estado == true) {
        cout << "el cliente ya esta conectado" << endl;
        return;
    }
    activar_socket();
    asignarFD();
    // solicito usuario y contrasena al cliente
    solicitarUserClave();
    //mando el usuario y clave al servidor y manejo respuesta
    mandar_credencial_a_servidor();
    return;

}
int main(int argc, char** argv) {
        Cliente cliente(argv);
        if (argc < 3) {
            fprintf(stderr, "Modo de Uso: %s IP-hostname puerto\n", argv[0]);
            exit(0);
        }
        while (true){
        cliente.mostrar_menu();
        }

}