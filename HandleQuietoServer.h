//
// Created by nestor on 31/10/16.
//

#ifndef METALZCURRA_HANDLEQUIETOSERVER_H
#define METALZCURRA_HANDLEQUIETOSERVER_H


#include <stdexcept>
#include <pthread.h>
#include "ObjectManager.h"
#include "ProtocoloVistaUpdate.h"
#include "Mensaje.h"
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

public:
    HandleQuietoServer();

    ~HandleQuietoServer();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setEmisor(string name);
};


#endif //METALZCURRA_HANDLEQUIETOSERVER_H
