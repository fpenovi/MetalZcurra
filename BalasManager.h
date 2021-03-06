//
// Created by nestor on 09/11/16.
//

#ifndef METALZCURRA_BALASMANAGER_H
#define METALZCURRA_BALASMANAGER_H

#include <stdexcept>
#include <pthread.h>
#include <chrono>
#include <signal.h>
#include <unordered_map>
#include <list>
#include "ProtocoloVistaUpdate.h"

using namespace std;

class NoSePudoCrearThreadBalasManagerException : public runtime_error {

public:
    NoSePudoCrearThreadBalasManagerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadBalasManagerException : public runtime_error {

public:
    NoSePudoCerrarThreadBalasManagerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argbalas argbalas_t;

class BalasManager {

private:
    pthread_t* balasManagerTH;
    argbalas_t* argBalas;
    bool isOn;
    bool isPaused;

public:
    BalasManager();

    ~BalasManager();

    void On();

    void Off();

    void Pause();

    void Resume();
};


#endif //METALZCURRA_BALASMANAGER_H
