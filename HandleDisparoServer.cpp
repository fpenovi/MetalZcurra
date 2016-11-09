//
// Created by nestor on 09/11/16.
//

#include "HandleDisparoServer.h"

using namespace chrono;

struct argdisparo {
    bool* isKhOn;
    bool* isKhPaused;
    string* emisor;
};


void* handleDisparoFunc(void* argKh) {

    bool* isKhOn = ( (argdisparo_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argdisparo_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    string* emisor = ( (argdisparo_t*) argKh)->emisor;

    int idEmisor = objectManager->getIdByUsername(*emisor);
    Personaje* personaje = objectManager->getObject(idEmisor);

    time_point<high_resolution_clock> start;
    start = high_resolution_clock::now();
    microseconds intervalo(100000);	// 100ms

    while (*isKhOn) {
        time_point<high_resolution_clock> actual;
        actual = high_resolution_clock::now();

        auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
        auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

        if (elapsed_ms.count() >= intervalo.count()) {

            if (*isKhOn && !(*isKhPaused)) {

                objectManager->inicializarBala(idEmisor, personaje->getPosCamara() + 60, personaje->getPosy() + 20);

                *isKhPaused = true;
            }

            start = chrono::system_clock::now();
        }
    }

    return NULL;
}


HandleDisparoServer::HandleDisparoServer() {
    isOn = false;
    argDisparo = NULL;
    handleDisparoTH = NULL;
    emisor;
}

HandleDisparoServer::~HandleDisparoServer() {
    delete handleDisparoTH;
    delete argDisparo;
}

void HandleDisparoServer::On() {

    handleDisparoTH = new pthread_t;

    if (!handleDisparoTH)
        throw NoSePudoCrearThreadHandleDisparoServerException();

    argDisparo = new argdisparo_t;

    if (!argDisparo)
        throw NoSePudoCrearThreadHandleDisparoServerException();

    isOn = true;
    isPaused = false;

    argDisparo->isKhOn = &isOn;
    argDisparo->isKhPaused = &isPaused;
    argDisparo->emisor = &emisor;

    if (pthread_create(handleDisparoTH, NULL, handleDisparoFunc, argDisparo))
        throw NoSePudoCrearThreadHandleDisparoServerException();
}

void HandleDisparoServer::Off() {
    if (!isOn)
        return;

    isOn = false;
}

void HandleDisparoServer::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void HandleDisparoServer::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}

void HandleDisparoServer::setEmisor(string name) {
    this->emisor = name;
}