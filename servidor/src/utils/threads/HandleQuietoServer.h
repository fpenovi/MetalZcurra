/*
 * HandleQuietoServer.h
 *
 *  Created on: 2 nov. 2016
 *      Author: nestor
 */

#ifndef HANDLEQUIETOSERVER_H_
#define HANDLEQUIETOSERVER_H_

#include <stdexcept>
#include <pthread.h>
#include "../../Mensaje.h"
#include "../protocolos/ProtocoloVistaUpdate.h"
#include "../ObjectManager.h"
#include <chrono>
#include <signal.h>
#include <SDL2/SDL_keycode.h>
#include <unordered_map>
#include <list>

using namespace std;

class NoSePudoCrearThreadHandleQuietoServerException : public runtime_error {

public:
    NoSePudoCrearThreadHandleQuietoServerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadHandleQuietoServerException : public runtime_error {

public:
    NoSePudoCerrarThreadHandleQuietoServerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argquieto argquieto_t;

class HandleQuietoServer {

private:
    pthread_t* handleQuietoTH;
    argquieto_t* argQuieto;
    bool isOn;
    bool isPaused;
    string emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;

public:
    HandleQuietoServer();

    ~HandleQuietoServer();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setEmisor(string name);

    void setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash);

    void setMutexesHash(unordered_map<string, pthread_mutex_t>* hash);
};


#endif /* HANDLEQUIETOSERVER_H_ */
