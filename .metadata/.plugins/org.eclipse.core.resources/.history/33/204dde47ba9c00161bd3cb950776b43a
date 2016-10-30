/*
 * Cliente.cpp
 *
 *  Created on: Oct 25, 2016
 *      Author: franch
 */

#include "Cliente.h"
#include <signal.h>
#include <chrono>
#include <fcntl.h>
#include <fstream>
#include <string.h>
#include <iostream>

using namespace std;
using namespace chrono;

Cliente::Cliente(char** argv){
    name = NULL;
    clave = NULL;
    heartbeat = NULL;
    sockFileDescrpt = 0;
    FILE* respuestaServidor = NULL;
    estado=false;
    nonBlocking = false;
    flags = -1;
    mutex_envios = PTHREAD_MUTEX_INITIALIZER;
    mutex_mensajes = PTHREAD_MUTEX_INITIALIZER;
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

    //cin.ignore();


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

bool Cliente::mandar_credencial_a_servidor(){

    char *linea=NULL;
    size_t len = 0;

    mandar_a_servidor(name,strlen(name));
    mandar_a_servidor(clave,strlen(clave));

    ssize_t bytesLeidos = getline(&linea, &len, respuestaServidor);

    //este cmp es re negro
    if (strcmp(linea, "fallo la conexion al sistema.\n") == 0) {
        printf("%s", linea);
        free(linea);
        liberar();
        return false;
    }
    free(linea);
    estado = true;
    cout<<"La conexion con el servidor fue exitosa"<<endl;
    recibir_usuarios_de_servidor();
    return true;
}

void Cliente::asignarFD(){
    respuestaServidor = fdopen(sockFileDescrpt, "r");
}

void Cliente::recibir_de_servidor(){
    char *linea=NULL;
    size_t len = 0;
    ssize_t bytesLeidos;
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
    ssize_t bytesLeidos;

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
    cantUsuarios = i;

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

    //cin.ignore();

    corroborarConexionConServer();

    const char* opc = "/R/\n";
    ssize_t bytesEsc = write(sockFileDescrpt, opc, strlen(opc));
    char* linea = NULL;
    size_t len = 0;
    ssize_t bytesLeidos;
    bool sigo = true;

    while (sigo) {
        bytesLeidos = getline(&linea, &len, respuestaServidor);

        if (bytesLeidos <= 0) {
            perror("ERROR --> Se cerró el server");
            salir();
        }

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

string Cliente::recibir_vista() {

    char* linea = NULL;
    size_t len = 0;
    ssize_t bytesLeidos;
    linea = (char*) malloc(500);
    bzero(linea, 500);

    //this->setBlocking();

    bytesLeidos = recv(sockFileDescrpt, linea, 499, 0);

    if (bytesLeidos < 0 && errno == EWOULDBLOCK) {
        cout << "EWOULDBLOCK" << endl;
        cout << bytesLeidos << endl;
        free(linea);
        linea = NULL;
        return "$\n";
    }

    if (errno == EBADF) {
        perror("ERROR --> Se cerró el server");
        free(linea);
        linea = NULL;
        salir();
    }

    string mensaje(linea);
    free(linea);
    linea = NULL;
    cout << mensaje;

    return mensaje;

}

string Cliente::desencolar_vista() {

    if (mensajes.size() == 0){
        return "$\n";
    }

    cout << mensajes.size() << endl;

    string mensaje = mensajes.front();

    mensajes.pop_front();

    return mensaje;
}

void Cliente::encolar_vistas() {

    char* linea = NULL;
    size_t len = 0;
    ssize_t bytesLeidos;

    bytesLeidos = getline(&linea, &len, respuestaServidor);

    if (bytesLeidos < 0) {
        perror("ERROR --> Se cerró el server");
        salir();
    }

    string mensaje(linea);
    cout << mensaje;

    mensajes.push_back(mensaje);

    free(linea);
    linea = NULL;

}

string Cliente::recibir_nueva_vista() {

    char* linea = NULL;
    size_t len = 0;
    ssize_t bytesLeidos;

    int result = pthread_mutex_lock(&mutex_envios);
    if (result != 0) perror("Fallo el pthread_mutex_lock en login");

    bytesLeidos = getline(&linea, &len, respuestaServidor);

    result = pthread_mutex_unlock(&mutex_envios);
    if (result != 0) perror("Fallo el pthread_mutex_unlock en login");

    if (bytesLeidos <= 0) {
        perror("ERROR --> Se cerró el server");
        salir();
    }

    string mensaje(linea);
    cout << linea;
    free(linea);
    linea = NULL;
    return mensaje;
}

void Cliente::desconectar(){
    if (!estado) {
        perror("El cliente ya se encuentra desconectado");
        return;
    }
    write(sockFileDescrpt, "/D/\n", 4);
    heartbeat->Off();
    liberar();
}

void Cliente::salir(){
    if(estado) {
        desconectar();
    }
    exit(0);
}

bool Cliente::heuristicaDeMensajes(size_t msjActual, size_t msjsTotales) {
    return ( (msjsTotales == 1)); //? true : (msjActual % msjsTotales) == 0 );
}

void Cliente::corroborarConexionConServer() {

    ssize_t bytesLeidos;
    ssize_t bytesEsc;
    char respuesta[10];

    // Envio un pedido de respuesta al servidor para checkear que sigo conectado
    bytesEsc = write(sockFileDescrpt, "/Z/\n", 4);

    if (bytesEsc <= 0) {
        perror("ERROR --> Has sido desconectado del servidor1");
        salir();
    }

    // Initialize file descriptor sets
    fd_set read_fds, write_fds, except_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(sockFileDescrpt, &read_fds);

    // Seteo el timeout a 5 segundos
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    if (int rv = select(sockFileDescrpt + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {

        bzero(respuesta, 10);
        bytesLeidos = read(sockFileDescrpt, respuesta, 10);

        if (bytesLeidos <= 0 || strcmp(respuesta, "/Z/\n") != 0) {
            perror("ERROR --> Has sido desconectado del servidor2");
            salir();
        }
    }
    else if(rv == 0)
    {
        perror("ERROR --> timeout");
        salir();
    }
    else{
        perror("ERROR --> select");
        salir();
    }
}

void Cliente::enviar(){

    if(! estado){
        cout<<"No esta conectado al servidor"<<endl;
        return;
    }

    cout<<endl;
    imprimir_usuarios();
    cout<<endl;

    //cin.ignore();

    printf("Elija una opcion: ");
    cout<<endl;

    size_t len = 0;
    char* linea = NULL;
    ssize_t bytesLeidos = getline(&linea, &len, stdin);

    int opcion = atoi(linea);
    if (opcion > cantUsuarios || opcion <= 0) {
        printf("No eligió un numero de pantalla\n");
        free(linea);
        return;
    }

    cout <<"Escriba un mensaje a: " << usuariosAenviar[opcion]<<endl;
    bytesLeidos = getline(&linea, &len, stdin);
    if (bytesLeidos < 0) {
        perror("ERROR --> Error stdin");
        free(linea);
        salir();
    }
    enviarAusuario(usuariosAenviar[opcion], linea, heuristicaDeMensajes(1, 1));
    free(linea);

}

void Cliente::enviarAusuario(string usuario, string linea, bool debePedirRespuesta) {

    int result = pthread_mutex_lock(&mutex_envios);
    if (result != 0) perror("Fallo el pthread_mutex_lock en login");
    ssize_t bytesLeidos;
    ssize_t bytesEsc;
    char respuesta[4];
    const char *opc = "/E/\n";

    heartbeat->Pause();

    if (debePedirRespuesta)
        corroborarConexionConServer();

    // Mando el protocolo de ENVIO
    signal(SIGPIPE, SIG_IGN);
    bytesEsc = write(sockFileDescrpt, opc, strlen(opc));

    if (bytesEsc < 0) {
        perror("ERROR --> Has sido desconectado del servidor");
        salir();
    }

    string mensajeCompleto = usuario + "$"; //LE AGREGO EL PROTOCOLO
    mensajeCompleto += linea;
    const char *envio = mensajeCompleto.c_str();

    // Mando el destino y el mensaje
    signal(SIGPIPE, SIG_IGN);
    bytesEsc = write(sockFileDescrpt, envio, mensajeCompleto.size());

    if (bytesEsc <= 0) {
        perror("ERROR --> Has sido desconectado del servidor");
        salir();
    }

    heartbeat->Resume();
    result = pthread_mutex_unlock(&mutex_envios);
    if (result != 0) perror("Fallo el pthread_mutex_unlock en login");
}

void Cliente::lorem() {

    if (!estado) {
        cout << "No esta conectado al servidor" << endl;
        return;
    }

    //cin.ignore();
    size_t len = 0;
    ssize_t bytesLeidos;

    int frecuencia;
    char* linea = NULL;


    cout << "Que frecuencia quiere usar?"<<endl;
    bytesLeidos = getline(&linea, &len, stdin);
    frecuencia = atoi(linea);
    linea = NULL;
    int cantidad;
    cout << "Cuantos mensajes quieren enviar?"<<endl;
    bytesLeidos = getline(&linea, &len, stdin);
    cantidad = atoi(linea);

    srand(time(NULL));
    int aleatorio = ( rand() % (cantUsuarios - 1) ) + 1;
    string destinatario = usuariosAenviar[aleatorio];

    cout << "Le envio el lorem a " << destinatario << endl;
    len = 0;
    int tam = (rand() % 200) + 1;
    int contador = 0;
    size_t bytesEsc = 0;

    ifstream archivo;
    archivo.open("lorem.txt");
    int can = 0;

    string* texto = new string;
    string mensaje;
    size_t i = 0;

    corroborarConexionConServer();

    // Leo el archivo y lo cargo en RAM
    while (i < cantidad){
        getline(archivo, mensaje);
        *texto += mensaje;
        i++;
    }

    i = 0;
    size_t enviados = 0;
    mensaje = "";

    int64_t SEGUNDO_nano = 1000000000;              // un segundo en nanosegundos
    nanoseconds intervalo(SEGUNDO_nano/frecuencia);

    time_point<high_resolution_clock> start;
    start = high_resolution_clock::now();

    while (enviados < cantidad) {

        time_point<high_resolution_clock> actual;
        actual = high_resolution_clock::now();

        auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
        auto elapsed_ns = duration_cast<nanoseconds>(deltaTiempo);

        if (elapsed_ns.count() >= intervalo.count()) {

            // Debo mandar la linea
            if (mensaje.size() == tam) {
                mensaje += "\n";
                enviarAusuario(destinatario, mensaje, heuristicaDeMensajes(++enviados, cantidad));
                mensaje = "";
            }

            start = high_resolution_clock::now();
        }

        // Solo agrego letra al mensaje si le falta para alcanzar su tamano
        if (mensaje.size() < tam) {
            mensaje += (*texto)[i];
            i++;

            // Si ya recorri el texto y tengo que comenzar desde el principio
            if (i == texto->size())
                i = 0;
        }
    }

    cout << "fin lorem" << endl;
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
    delete heartbeat;
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

        if (opcion=="1") conectar();
        else if (opcion=="2") desconectar();
        else if (opcion=="3") salir();
        else if (opcion=="4") enviar();
        else if (opcion=="5") recibir_mensajes();
        else if (opcion=="6") lorem();
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
    if (mandar_credencial_a_servidor()) {
        heartbeat = new Heartbeat(sockFileDescrpt);
        heartbeat->On();
    }
}

void Cliente::setNonBlocking() {

    if (!estado)
        return;

    if (nonBlocking)
        return;

    flags = fcntl(sockFileDescrpt, F_GETFL, 0);
    if (flags < 0)
        perror("fcntl(F_GETFL)");

    // SETEO FLAG DE FILE DESCRIPTOR NO BLOCKEANTE
    if (fcntl(sockFileDescrpt, F_SETFL, flags | O_NONBLOCK) < 0)
        perror("ERROR -> fcntl seteando non-blocking");

    nonBlocking = true;
}

void Cliente::setBlocking() {

    if (!estado)
        return;

    if (!nonBlocking)
        return;

    flags = fcntl(sockFileDescrpt, F_GETFL, 0);
    if (flags < 0)
        perror("fcntl(F_GETFL)");

    // SETEO FLAG DE FILE DESCRIPTOR NO BLOCKEANTE
    if (fcntl(sockFileDescrpt, F_SETFL, flags & ~O_NONBLOCK) < 0)
        perror("ERROR -> fcntl seteando blocking");

    nonBlocking = false;
}

int Cliente::getCantidadMensajesEncolados() {
    return this->mensajes.size();
}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

