//
// Created by nestor on 15/11/16.
//

#ifndef METALZCURRA_ENEMIGOSMANAGER_H
#define METALZCURRA_ENEMIGOSMANAGER_H

#include <stdexcept>
#include <pthread.h>
#include <chrono>
#include <signal.h>
#include <unordered_map>
#include <list>
#include "Mensaje.h"
#include "ProtocoloVistaUpdate.h"

using namespace std;

class NoSePudoCrearThreadEnemigosManagerException : public runtime_error {

public:
    NoSePudoCrearThreadEnemigosManagerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadEnemigosManagerException : public runtime_error {

public:
    NoSePudoCerrarThreadEnemigosManagerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argenemigos argenemigos_t;

class EnemigosManager {

private:
    pthread_t* enemigosManagerTH;
    argenemigos_t* argEnemigos;
    bool isOn;
    bool isPaused;
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;

public:
    EnemigosManager();

    ~EnemigosManager();

    void On();

    void Off();

    void Pause();

    void Resume();

    void setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash);

    void setMutexesHash(unordered_map<string, pthread_mutex_t>* hash);
};

#endif //METALZCURRA_ENEMIGOSMANAGER_H
