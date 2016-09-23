#ifndef METALZCURRA_CLIENTE_H
#define METALZCURRA_CLIENTE_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include "Heartbeat.h"

using namespace std;

class Cliente {

private:
    char* name;
    char* clave;
    int sockFileDescrpt;
    int cantUsuarios;
    FILE* respuestaServidor;
    unordered_map<int, string> usuariosAenviar; //hash de usuarios
    bool estado;
    char port[8];
    char IP[8];
	Heartbeat* heartbeat;

public:
    Cliente(char** argv);

    void enviarAusuario(string usuario, string linea, bool debePedirRespuesta);

    void solicitarUserClave();

    void mandar_a_servidor(char* linea, int largo);

    bool mandar_credencial_a_servidor();

    void asignarFD();

    void recibir_de_servidor();

    void recibir_usuarios_de_servidor();

    void recibir_mensajes();

    bool heuristicaDeMensajes(size_t msjActual, size_t msjsTotales);

    void corroborarConexionConServer();

    void desconectar();

    void salir();

    void enviar();

    void lorem();

    void liberar();

    void mostrar_menu();

    void conectar();

    void activar_socket();

    void imprimir_usuarios();
};


#endif //METALZCURRA_CLIENTE_H