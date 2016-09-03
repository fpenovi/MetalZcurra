//
// Created by alan on 01/09/16.
//

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

    public:

        Cliente();

        void solicitarUserClave();

        void mandar_a_servidor(char* linea, int largo);

        void mandar_credencial_a_servidor();

        void asignarFD(int fd);

        /*int activar_socket(char **parametros);*/

        void recibir_de_servidor();

        void liberar();

};


#endif //METALZCURRA_CLIENTE_H
