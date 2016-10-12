//
// Created by nestor on 11/10/16.
//

#ifndef METALZCURRA_HANDLEJUMP_H
#define METALZCURRA_HANDLEJUMP_H

#include <stdexcept>
#include <pthread.h>
#include "Cliente.h"

using namespace std;

class NoSePudoCrearThreadHandleJumpException : public runtime_error {

public:
    NoSePudoCrearThreadHandleJumpException() : runtime_error("No se pudo crear el thread HandleJump") {	}
};

class NoSePudoCerrarThreadHandleJumpException : public runtime_error {

public:
    NoSePudoCerrarThreadHandleJumpException() : runtime_error("No se pudo cerrar el thread HandleJump") {	}
};


typedef struct argkh argkh_t;

class HandleJump {

private:
    Cliente* cliente;
    pthread_t* handleJumpTH;
    argkh_t* argKeyHold;
    bool isOn;
    bool isPaused;
    int keyPressed;

public:
    HandleJump(Cliente* cliente);

    ~HandleJump();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setKeyPressed(int key);
};

#endif //METALZCURRA_HANDLEJUMP_H
