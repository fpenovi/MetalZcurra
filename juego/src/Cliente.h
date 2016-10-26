/*
 * Cliente.h
 *
 *  Created on: Oct 25, 2016
 *      Author: franch
 */

#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <string>
#include <unordered_map>
#include <list>
#include <pthread.h>
#include "utils/Heartbeat.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

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

	string recibir_vista();

	string recibir_nueva_vista();

	void encolar_vistas();

	string desencolar_vista();

	void setNonBlocking();

	void setBlocking();

	int getCantidadMensajesEncolados();

	virtual ~Cliente();
};

#endif /* CLIENTE_H_ */
