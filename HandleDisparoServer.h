//
// Created by nestor on 09/11/16.
//

#ifndef METALZCURRA_HANDLEDISPAROSERVER_H
#define METALZCURRA_HANDLEDISPAROSERVER_H

#include <stdexcept>
#include <pthread.h>
#include "ObjectManager.h"
#include <chrono>
#include <signal.h>
#include <unordered_map>
#include <list>

using namespace std;

class NoSePudoCrearThreadHandleDisparoServerException : public runtime_error {

public:
    NoSePudoCrearThreadHandleDisparoServerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadHandleDisparoServerException : public runtime_error {

public:
    NoSePudoCerrarThreadHandleDisparoServerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argdisparo argdisparo_t;

class HandleDisparoServer {

private:
    pthread_t* handleDisparoTH;
    argdisparo_t* argDisparo;
    bool isOn;
    bool isPaused;
    string emisor;

public:
    HandleDisparoServer();

    ~HandleDisparoServer();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setEmisor(string name);
};

#endif //METALZCURRA_HANDLEDISPAROSERVER_H
