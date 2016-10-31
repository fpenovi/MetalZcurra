/*
 * HandleJumpServer.h
 *
 *  Created on: Oct 30, 2016
 *      Author: franch
 */

#ifndef HANDLEJUMPSERVER_H_
#define HANDLEJUMPSERVER_H_

#include <stdexcept>
#include <pthread.h>
#include <chrono>
#include <unordered_map>
#include <list>
#include "../../Mensaje.h"
#include "../ObjectManager.h"

using namespace std;

class NoSePudoCrearThreadHandleJumpServerException : public runtime_error {

public:
    NoSePudoCrearThreadHandleJumpServerException() : runtime_error("No se pudo crear el thread HandleJump") {	}
};

class NoSePudoCerrarThreadHandleJumpServerException : public runtime_error {

public:
    NoSePudoCerrarThreadHandleJumpServerException() : runtime_error("No se pudo cerrar el thread HandleJump") {	}
};

typedef struct argkh argkh_t;

class HandleJumpServer {

private:
    pthread_t* handleJumpTH;
    argkh_t* argKeyHold;
    bool isOn;
    bool isPaused;
    int keyPressed;
    ObjectManager* objectManager;
    string emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;

public:
    HandleJumpServer(ObjectManager* objectManager);

    void On();

    void Off();

    void Pause();

    void Resume();

    void setKeyPressed(int key);

    int getKeyPressed();

    void setEmisor(string name);

    void setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash);

    void setMutexesHash(unordered_map<string, pthread_mutex_t>* hash);

    virtual ~HandleJumpServer();
};

#endif /* HANDLEJUMPSERVER_H_ */
