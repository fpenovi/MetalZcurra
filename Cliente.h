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
#include <list>

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
	bool nonBlocking;
	int flags;
    char port[8];
    char IP[16];
	Heartbeat* heartbeat;
	pthread_mutex_t mutex_envios;
	pthread_mutex_t mutex_mensajes;
	list<string> mensajes;

public:
    Cliente(string ip, string puerto);

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

    void conectar(string nombre);

    void activar_socket();

    void imprimir_usuarios();

	string recibir_vista();

	string recibir_nueva_vista();

	void encolar_vistas();

	string desencolar_vista();

	void setNonBlocking();

	void setBlocking();

	int getCantidadMensajesEncolados();
};


#endif //METALZCURRA_CLIENTE_H