//
// Created by nestor on 27/11/16.
//

#ifndef METALZCURRA_PERSONAJESMANAGER_H
#define METALZCURRA_PERSONAJESMANAGER_H

#include <stdexcept>
#include <pthread.h>
#include <chrono>
#include <signal.h>
#include <unordered_map>
#include <list>
#include "ProtocoloVistaUpdate.h"

using namespace std;

class NoSePudoCrearThreadPersonajesManagerException : public runtime_error {

public:
    NoSePudoCrearThreadPersonajesManagerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadPersonajesManagerException : public runtime_error {

public:
    NoSePudoCerrarThreadPersonajesManagerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argpersonajes argpersonajes_t;

class PersonajesManager {

private:
    pthread_t* personajesManagerTH;
    argpersonajes_t* argPersonajes;
    bool isOn;
    bool isPaused;

public:
    PersonajesManager();

    ~PersonajesManager();

    void On();

    void Off();

    void Pause();

    void Resume();
};

#endif //METALZCURRA_PERSONAJESMANAGER_H
