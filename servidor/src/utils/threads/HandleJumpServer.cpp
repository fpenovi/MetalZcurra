/*
 * HandleJumpServer.cpp
 *
 *  Created on: Oct 30, 2016
 *      Author: franch
 */

#include "HandleJumpServer.h"
#include "../protocolos/ProtocoloVistaUpdate.h"
#include <unistd.h>

using namespace chrono;

struct argkh {
    int* keyPressed;
    bool* isKhOn;
    bool* isKhPaused;
    ObjectManager* objectMan;
    string* emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;
};

void* handleJumpFunc(void* argKh) {

    bool* isKhOn = ( (argkh_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argkh_t*) argKh)->isKhPaused;
    int* keyPressed = ( (argkh_t*) argKh)->keyPressed;
    ObjectManager* objectManager = ( (argkh_t*) argKh)->objectMan;
    string* emisor = ( (argkh_t*) argKh)->emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash = ( (argkh_t*) argKh)->conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash = ( (argkh_t*) argKh)->mutexesHash;

    while (*isKhOn) {

        if (*isKhOn && !(*isKhPaused)) {

            int idEmisor = objectManager->getIdByUsername(*emisor);
            Personaje* personaje = objectManager->getObject(idEmisor);
            bool avanzar = objectManager->puedoAvanzar();
            int* posX = objectManager->getPosX();

            ProtocoloVistaUpdate update;
            int aux;

            for (int i = 0 ; i < 22 ; i++){

                if (personaje->getPosy() <= 305){
                    personaje->setVely(personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                    personaje->setVely(0);
                    personaje->setBajando(true);
                }
                else if (personaje->getPosy() >= 360){
                    personaje->setVely(-personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                    personaje->setVely(0);
                    personaje->setBajando(false);
                }
                else if (personaje->getBajando()){
                    personaje->setVely(personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                }

                else if(!(personaje->getBajando())) {
                    personaje->setVely(-personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                }

                update.setEstado(personaje->getSeMovio());
                update.setX(*posX);
                update.setY(personaje->getPosy());
                update.setObject_id(idEmisor);
                update.setPosCamara(personaje->getPosCamara());
                update.setConectado(personaje->getConectado());
                update.setSpriteIndex(personaje->getSpriteSaltando());

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

                usleep(40000);
            }
            *isKhPaused = true;
        }
    }

    return NULL;
}

HandleJumpServer::HandleJumpServer(ObjectManager* objectManager) {
    this->objectManager = objectManager;
    isOn = false;
    argKeyHold = NULL;
    handleJumpTH = NULL;
    keyPressed = 0;
    emisor;
}

void HandleJumpServer::On() {

    handleJumpTH = new pthread_t;

    if (!handleJumpTH)
        throw NoSePudoCrearThreadHandleJumpServerException();

    argKeyHold = new argkh_t;

    if (!argKeyHold)
        throw NoSePudoCrearThreadHandleJumpServerException();

    isOn = true;
    isPaused = false;

    argKeyHold->objectMan = objectManager;
    argKeyHold->isKhOn = &isOn;
    argKeyHold->isKhPaused = &isPaused;
    argKeyHold->keyPressed = &keyPressed;
    argKeyHold->emisor = &emisor;
    argKeyHold->conectadosHash = conectadosHash;
    argKeyHold->mutexesHash = mutexesHash;

    if (pthread_create(handleJumpTH, NULL, handleJumpFunc, argKeyHold))
        throw NoSePudoCrearThreadHandleJumpServerException();
}

void HandleJumpServer::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*handleJumpTH, NULL);
}

void HandleJumpServer::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void HandleJumpServer::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}

void HandleJumpServer::setKeyPressed(int key) {
    this->keyPressed = key;
}

int HandleJumpServer::getKeyPressed() {
    return  this->keyPressed;
}

void HandleJumpServer::setEmisor(string name) {
    this->emisor = name;
}

void HandleJumpServer::setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash) {
    this->conectadosHash = hash;
}

void HandleJumpServer::setMutexesHash(unordered_map<string, pthread_mutex_t>* hash) {
    this->mutexesHash = hash;
}

HandleJumpServer::~HandleJumpServer() {
    delete handleJumpTH;
    delete argKeyHold;
}
