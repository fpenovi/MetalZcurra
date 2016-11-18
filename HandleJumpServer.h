//
// Created by nestor on 29/10/16.
//

#ifndef METALZCURRA_HANDLEJUMPSERVER_H
#define METALZCURRA_HANDLEJUMPSERVER_H

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

class NoSePudoCrearThreadHandleJumpServerException : public runtime_error {

public:
    NoSePudoCrearThreadHandleJumpServerException() : runtime_error("No se pudo crear el thread HandleJump") {	}
};

class NoSePudoCerrarThreadHandleJumpServerException : public runtime_error {

public:
    NoSePudoCerrarThreadHandleJumpServerException() : runtime_error("No se pudo cerrar el thread HandleJump") {	}
};


typedef struct argjump argjump_t;

class HandleJumpServer {

private:
    pthread_t* handleJumpTH;
    argjump_t* argJump;
    bool isOn;
    bool isPaused;
    int keyPressed;
    string emisor;

public:
    HandleJumpServer();

    ~HandleJumpServer();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setKeyPressed(int key);

    int getKeyPressed();

    void setEmisor(string name);
};


#endif //METALZCURRA_HANDLEJUMPSERVER_H
