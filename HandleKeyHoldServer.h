//
// Created by nestor on 26/10/16.
//

#ifndef METALZCURRA_HANDLEKEYHOLDSERVER_H
#define METALZCURRA_HANDLEKEYHOLDSERVER_H

#include <stdexcept>
#include <pthread.h>
#include "Cliente.h"
#include "ObjectManager.h"
#include "ProtocoloVistaUpdate.h"
#include "Mensaje.h"
#include <chrono>
#include <signal.h>
#include <SDL2/SDL_keycode.h>
#include <unordered_map>
#include <list>

using namespace std;

class NoSePudoCrearThreadHandleKeyHoldServerException : public runtime_error {

public:
    NoSePudoCrearThreadHandleKeyHoldServerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadHandleKeyHoldServerException : public runtime_error {

public:
    NoSePudoCerrarThreadHandleKeyHoldServerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argkh argkh_t;

class HandleKeyHoldServer {

private:
    pthread_t* handleKeyHoldTH;
    argkh_t* argKeyHold;
    bool isOn;
    bool isPaused;
    int keyPressed;
    string emisor;

public:
    HandleKeyHoldServer();

    ~HandleKeyHoldServer();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setKeyPressed(int key);

    int getKeyPressed();

    void setEmisor(string name);
};


#endif //METALZCURRA_HANDLEKEYHOLDSERVER_H
