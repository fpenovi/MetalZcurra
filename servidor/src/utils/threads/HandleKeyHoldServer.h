/*
 * HandleKeyHoldServer.h
 *
 *  Created on: Oct 30, 2016
 *      Author: franch
 */

#ifndef HANDLEKEYHOLDSERVER_H_
#define HANDLEKEYHOLDSERVER_H_

#include <stdexcept>
#include <pthread.h>
#include <chrono>
#include <unordered_map>
#include <list>
#include "../../Mensaje.h"

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
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;

public:
    HandleKeyHoldServer();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setKeyPressed(int key);

    int getKeyPressed();

    void setEmisor(string name);

    void setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash);

    void setMutexesHash(unordered_map<string, pthread_mutex_t>* hash);

	virtual ~HandleKeyHoldServer();
};

#endif /* HANDLEKEYHOLDSERVER_H_ */