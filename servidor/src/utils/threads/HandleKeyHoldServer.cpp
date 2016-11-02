/*
 * HandleKeyHoldServer.cpp
 *
 *  Created on: Oct 30, 2016
 *      Author: franch
 */

#include "HandleKeyHoldServer.h"
#include "../ObjectManager.h"
#include "../protocolos/ProtocoloVistaUpdate.h"
#include <SDL2/SDL_keycode.h>

using namespace chrono;

struct argkh {
    int* keyPressed;
    bool* isKhOn;
    bool* isKhPaused;
    string* emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;
};

void* handleKeyHoldFunc(void* argKh) {

    bool* isKhOn = ( (argkh_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argkh_t*) argKh)->isKhPaused;
    int* keyPressed = ( (argkh_t*) argKh)->keyPressed;
    ObjectManager* objectManager = ObjectManager::getInstance();
    string* emisor = ( (argkh_t*) argKh)->emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash = ( (argkh_t*) argKh)->conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash = ( (argkh_t*) argKh)->mutexesHash;

    time_point<high_resolution_clock> start;
    start = high_resolution_clock::now();
    microseconds intervalo(35000);	// 35ms

    while (*isKhOn) {
        time_point<high_resolution_clock> actual;
        actual = high_resolution_clock::now();

        auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
        auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

        if (elapsed_ms.count() >= intervalo.count()) {

            if (*isKhOn && !(*isKhPaused)) {

                int idEmisor = objectManager->getIdByUsername(*emisor);
                Personaje* personaje = objectManager->getObject(idEmisor);
                bool avanzar = objectManager->puedoAvanzar();
                int* posX = objectManager->getPosX();

                ProtocoloVistaUpdate update;
                int aux;

                switch( *keyPressed ) {

                    case SDLK_LEFT:
                        personaje->setVelx(-personaje->getPersonaje_VEL());
                        personaje->moverX(avanzar, posX);

                        update.setEstado(personaje->getSeMovio());
                        update.setX(*posX);
                        update.setY(personaje->getPosy());
                        update.setObject_id(idEmisor);
                        update.setPosCamara(personaje->getPosCamara());
                        update.setConectado(personaje->getConectado());
                        update.setSpriteIndex(personaje->getFrameCorriendo());
                        break;

                    case SDLK_RIGHT:
                        aux = *posX;
                        personaje->setVelx(personaje->getPersonaje_VEL());
                        personaje->moverX(avanzar, posX);

                        if (aux < *posX){
                            objectManager->moverCamara(idEmisor);
                            objectManager->moverDesconectados();
                        }

                        update.setEstado(personaje->getSeMovio());
                        update.setX(*posX);
                        update.setY(personaje->getPosy());
                        update.setObject_id(idEmisor);
                        update.setPosCamara(personaje->getPosCamara());
                        update.setConectado(personaje->getConectado());

                        if (personaje->getSeMovio()) update.setSpriteIndex(personaje->getFrameCorriendo());
                        else update.setSpriteIndex(personaje->getSpriteParado());

                        break;

                }

                int result;
                string mensaje = update.toString();

                for (auto kv : *conectadosHash) {

                    Mensaje* mensajeNuevo = new Mensaje(*emisor, kv.first, mensaje);

                    result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
                    if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

                    kv.second->push_back(mensajeNuevo);

                    result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
                    if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
                }
            }

            start = chrono::system_clock::now();
        }
    }

    return NULL;
}

HandleKeyHoldServer::HandleKeyHoldServer() {
    isOn = false;
    argKeyHold = NULL;
    handleKeyHoldTH = NULL;
    keyPressed = 0;
    emisor;
}

void HandleKeyHoldServer::On() {

    handleKeyHoldTH = new pthread_t;

    if (!handleKeyHoldTH)
        throw NoSePudoCrearThreadHandleKeyHoldServerException();

    argKeyHold = new argkh_t;

    if (!argKeyHold)
        throw NoSePudoCrearThreadHandleKeyHoldServerException();

    isOn = true;
    isPaused = false;

    argKeyHold->isKhOn = &isOn;
    argKeyHold->isKhPaused = &isPaused;
    argKeyHold->keyPressed = &keyPressed;
    argKeyHold->emisor = &emisor;
    argKeyHold->conectadosHash = conectadosHash;
    argKeyHold->mutexesHash = mutexesHash;

    if (pthread_create(handleKeyHoldTH, NULL, handleKeyHoldFunc, argKeyHold))
        throw NoSePudoCrearThreadHandleKeyHoldServerException();
}

void HandleKeyHoldServer::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*handleKeyHoldTH, NULL);
}

void HandleKeyHoldServer::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void HandleKeyHoldServer::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}

void HandleKeyHoldServer::setKeyPressed(int key) {
    this->keyPressed = key;
}

int HandleKeyHoldServer::getKeyPressed() {
    return  this->keyPressed;
}

void HandleKeyHoldServer::setEmisor(string name) {
    this->emisor = name;
}

void HandleKeyHoldServer::setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash) {
    this->conectadosHash = hash;
}

void HandleKeyHoldServer::setMutexesHash(unordered_map<string, pthread_mutex_t>* hash) {
    this->mutexesHash = hash;
}

HandleKeyHoldServer::~HandleKeyHoldServer() {
    delete handleKeyHoldTH;
    delete argKeyHold;
}

