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

using namespace std;

class Cliente {
    private:
        // atributos
        char* name;
        char* clave;
        int sockFileDescrpt;
        //pthread_t* thread;
        FILE* respuestaServidor;
        bool estado;
        char port[8];
        char IP[8];
    public:

        Cliente(char** argv);

    void cambiar_estado(bool nuevo_estado);

    void solicitarUserClave();

    void mandar_a_servidor(char* linea, int largo);

    void mandar_credencial_a_servidor();

    void asignarFD();

    void recibir_de_servidor();

    char* recibir_usuarios_de_servidor();

    void recibir_mensajes();

    void desconectar();

    void salir();

    void enviar();

    void lorem();

    void liberar();

    bool getEstado();

    void mostrar_menu();

    void conectar();

    void activar_socket();
};


#endif //METALZCURRA_CLIENTE_H