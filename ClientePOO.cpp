#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "ClientePOO.h"
#include <pthread.h>
#include <unordered_map>
#include <fstream>

using namespace std;

Cliente::Cliente(char** argv){
    name = NULL;
    clave = NULL;
    sockFileDescrpt = 0;
    //thread = NULL;
    FILE* respuestaServidor = NULL;
    estado=false;

    usuariosAenviar; //hash de usuarios
    int cantUsuarios = 0;

    string aux(argv[1]);
    strcpy(IP,aux.c_str());
    aux= argv[2];
    strcpy(port,aux.c_str());

}

void Cliente::solicitarUserClave(){
    ssize_t bytesLeidos;
    size_t largo = 0;

    printf("Ingrese nombre de usuario:");

    cin.ignore();


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

    mandar_a_servidor(name,strlen(name));
    mandar_a_servidor(clave,strlen(clave));

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
    recibir_usuarios_de_servidor();

}

void Cliente::asignarFD(){
    respuestaServidor = fdopen(sockFileDescrpt, "r");
}

void Cliente::recibir_de_servidor(){
    char *linea=NULL;
    size_t len = 0;
    size_t bytesLeidos;
    char respuesta[1];

    read(sockFileDescrpt,respuesta,1);
    //bytesLeidos = getline(&linea, &len, respuestaServidor);

    if (bytesLeidos < 0) {
        perror("ERROR --> leyendo de socket");
        free(linea);
        exit(1);
    }

    //free(linea);
}

void Cliente::recibir_usuarios_de_servidor(){

    char *linea=NULL;
    size_t len = 0;
    size_t bytesLeidos;

    bytesLeidos = getline(&linea, &len, respuestaServidor);
    if (bytesLeidos < 0) {
        perror("ERROR --> leyendo de socket");
        free(linea);
        exit(1);
    }

    len = 0;
    bytesLeidos = 0;

    char *token = strtok(linea, ",");
    int i=1;

    while (token != NULL){
        if (strcmp(token,"\n" )== 0) break;
        printf("%i. ",i);
        printf( "%s\n", token );
        string strUsuario(token);
        usuariosAenviar[i]=strUsuario;

        token = strtok(NULL,",");
        i = i+1;
    }
    printf("%i. %s\n",i,"TODOS\n");

    string todos = "TODOS";
    usuariosAenviar[i]=todos;
    cantUsuarios = i-1;

    free(linea);

}

void Cliente::imprimir_usuarios(){
    for (int i = 1; i<=cantUsuarios;i++){
        cout << i <<". " << usuariosAenviar[i]<<endl;
    }
}

void Cliente::recibir_mensajes(){

    if(! estado){
        cout<<"No esta conectado al servidor"<<endl;
        return;
    }

    cin.ignore();

    char* opc = "/R/\n";
    mandar_a_servidor(opc, strlen(opc));
    char* linea = NULL;
    size_t len = 0;
    size_t bytesLeidos;
    bool sigo = true;

    while (sigo) {
        bytesLeidos = getline(&linea, &len, respuestaServidor);

        if (strcmp(linea,"$\n") == 0) {

            sigo = false;
            cout << "no hay nada mas para recibir"<<endl;
            break;
        }
        printf("%s", linea);
        free(linea);
        linea = NULL;
    }

    free(linea);
}

void Cliente::desconectar(){
    if (estado == false) {
        perror("El cliente ya se encuentra desconectado");
        return;
    }
    write(sockFileDescrpt, "/D/\n", 4);
    liberar();
}

void Cliente::salir(){
    if(estado) {
        desconectar();
    }
    exit(0);
}

void Cliente::enviar(){

    if(! estado){
        cout<<"No esta conectado al servidor"<<endl;
        return;
    }

    cout<<endl;
    imprimir_usuarios();
    cout<<endl;

    cin.ignore();

    printf("Elija una opcion: ");
    cout<<endl;

    size_t len = 0;
    char* linea = NULL;
    size_t bytesLeidos = getline(&linea, &len, stdin);

    int opcion = atoi(linea);
    if (opcion > cantUsuarios || opcion <= 0) {
        printf("No eligió un numero de pantalla\n");
        free(linea);
        return;
    }

    cout <<"Escriba un mensaje a: " << usuariosAenviar[opcion]<<endl;
    bytesLeidos = getline(&linea, &len, stdin);
    enviarAusuario(usuariosAenviar[opcion],linea);
    free(linea);

}

void Cliente::enviarAusuario(string usuario,string linea) {

    const char* opc = "/E/\n";
    size_t bytesEsc = write(sockFileDescrpt, opc, strlen(opc));
    //string msg(linea);
    string mensajeCompleto = usuario + "$"; //LE AGREGO EL PROTOCOLO
    mensajeCompleto+=linea;
    const char* envio = mensajeCompleto.c_str();
    bytesEsc = write(sockFileDescrpt, envio, strlen(envio));
    //recibir_de_servidor(); //esto estaria recibiendo el tick ---> hace lento el lorem ipsum tambien
}

void Cliente::lorem() {

    if (!estado) {
        cout << "No esta conectado al servidor" << endl;
        return;
    }

    cin.ignore();
    char *linea = NULL;
    size_t len = 0;
    size_t bytesLeidos;
    int frecuencia = 1;
    srand (time(NULL));
    int aleatorio = (rand()%cantUsuarios)+1;//i-1=cantidad de usuarios de 0 a i, +1= que no cuente al 0
    int cantidad =1000;
    string destinatario = usuariosAenviar[aleatorio];
    cout << "Le envio el lorem a " << destinatario << endl;
    len = 0;
    int tam =200;
    char buffer[tam + 2];
    int contador = 0;
    size_t bytesEsc = 0;

    ifstream archivo;
    archivo.open("lorem.txt");
    int can=0;

    string* texto = new string;
    string mensaje;
    size_t i =0;

    while(i < cantidad){
        getline(archivo,mensaje);
        *texto += mensaje;
        i++;
    }

    i = 0;
    size_t enviados=0;
    mensaje = "";
    size_t largomsg = 0;
    while (enviados<cantidad){
        if (i == texto->size())
            i = 0;

        if (largomsg == tam) {
            mensaje+="\n";
            enviarAusuario(destinatario,mensaje);
            enviados++;
            mensaje = "";
            largomsg=0;
        }

        mensaje+=(*texto)[i];
        i++;
        largomsg++;
    }
    delete(texto);
    archivo.close();
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
    string opcion;
    ssize_t bytesLeidos;
    size_t len = 0;
    while (true) {
        cout<<endl;
        cout<<"\033[1;31m//////////////////\033[0m"<<endl;
        printf("\033[1;31m1 : Conectar\033[0m\n");
        printf("\033[1;31m2 : Desconectar\033[0m\n");
        printf("\033[1;31m3 : Salir\033[0m\n");
        printf("\033[1;31m4 : Enviar\033[0m\n");
        printf("\033[1;31m5 : Recibir\033[0m\n");
        printf("\033[1;31m6 : Lorem Ipsum\033[0m\n");
        cout<<"\033[1;31m//////////////////\033[0m"<<endl;
        cout<<endl;

        cin >> opcion;
        if (opcion=="1") conectar() ;
        else if (opcion=="2") desconectar();
        else if (opcion=="3") salir() ;
        else if (opcion=="4") enviar();
        else if (opcion=="5") recibir_mensajes() ;
        else if (opcion=="6") lorem() ;
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