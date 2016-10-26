/*
 * HandleJump.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef HANDLEJUMP_H_
#define HANDLEJUMP_H_

#include <stdexcept>
#include <pthread.h>
#include "../Cliente.h"

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

    void On();

    void Off();

    void Pause();

    void Resume();

    void setKeyPressed(int key);

	virtual ~HandleJump();
};

#endif /* HANDLEJUMP_H_ */
