//
// Created by nestor on 29/11/16.
//

#ifndef METALZCURRA_GRAVEDADMANAGER_H
#define METALZCURRA_GRAVEDADMANAGER_H

#include <stdexcept>
#include <pthread.h>
#include <chrono>
#include <signal.h>
#include <unordered_map>
#include <list>
#include "ProtocoloVistaUpdate.h"

using namespace std;

class NoSePudoCrearThreadGravedadManagerException : public runtime_error {

public:
    NoSePudoCrearThreadGravedadManagerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadGravedadManagerException : public runtime_error {

public:
    NoSePudoCerrarThreadGravedadManagerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct arggravedad arggravedad_t;

class GravedadManager {

private:
    pthread_t* gravedadManagerTH;
    arggravedad_t* argGravedad;
    bool isOn;
    bool isPaused;

public:
    GravedadManager();

    ~GravedadManager();

    void On();

    void Off();

    void Pause();

    void Resume();
};


#endif //METALZCURRA_GRAVEDADMANAGER_H
