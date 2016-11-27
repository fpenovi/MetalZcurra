//
// Created by nestor on 29/10/16.
//

#include "HandleJumpServer.h"

using namespace chrono;

struct argjump {
    int* keyPressed;
    bool* isKhOn;
    bool* isKhPaused;
    string* emisor;
};


void* handleJumpFunc(void* argKh) {

    bool* isKhOn = ( (argjump_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argjump_t*) argKh)->isKhPaused;
    int* keyPressed = ( (argjump_t*) argKh)->keyPressed;
    ObjectManager* objectManager = ObjectManager::getInstance();
    string* emisor = ( (argjump_t*) argKh)->emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash = objectManager->getConectadosHash();
    unordered_map<string, pthread_mutex_t>* mutexesHash = objectManager->getMutexesHash();

    while (*isKhOn) {

        if (*isKhOn && !(*isKhPaused)) {

            int idEmisor = objectManager->getIdByUsername(*emisor);
            Personaje* personaje = objectManager->getObject(idEmisor);
            bool avanzar = objectManager->puedoAvanzar();
            int* posX = objectManager->getPosX();

            ProtocoloVistaUpdate update;
            int aux;

            personaje->resetFrames();

            int posActual, posFinal;
            posActual = personaje->getPosy();
            personaje->setUltimaPosy(posActual);
            posFinal = posActual - 120;
            personaje->setGravity(false);

            for (int i = 0 ; i < 48 ; i++){

                if (personaje->getPosy() <= posFinal){
                    personaje->setVely(personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                    personaje->setVely(0);
                    personaje->setBajando(true);
                    personaje->setSprites();
                }
                else if (personaje->getPosy() >= posActual){
                    personaje->setUltimaPosy(personaje->getPosy());
                    personaje->setVely(-personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                    personaje->setVely(0);
                    personaje->setBajando(false);
                    personaje->setSaltando(false);
                }
                else if (personaje->getBajando()){
                    personaje->setVely(personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                    personaje->setSprites();
                }

                else if(!(personaje->getBajando())) {
                    personaje->setVely(-personaje->getPersonaje_VEL_Y());
                    personaje->moverY();
                    personaje->setSprites();
                }

                update.setTipoObjeto(1);
                update.setEstado(personaje->getSeMovio());
                update.setX(*posX);
                update.setY(personaje->getPosy());
                update.setObject_id(idEmisor);
                update.setPosCamara(personaje->getPosCamara());
                update.setConectado(personaje->getConectado());
                update.setSpriteIndex(personaje->getSprites());
                update.setApuntando(personaje->getDireccion());
                update.setSaltando(personaje->getSaltando());
                update.setPuntaje(personaje->getPuntaje());

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
            personaje->setGravity(true);
            *isKhPaused = true;
        }
    }

    return NULL;
}


HandleJumpServer::HandleJumpServer() {
    isOn = false;
    argJump = NULL;
    handleJumpTH = NULL;
    keyPressed = 0;
    emisor;
}

HandleJumpServer::~HandleJumpServer() {
    delete handleJumpTH;
    delete argJump;
}

void HandleJumpServer::On() {

    handleJumpTH = new pthread_t;

    if (!handleJumpTH)
        throw NoSePudoCrearThreadHandleJumpServerException();

    argJump = new argjump_t;

    if (!argJump)
        throw NoSePudoCrearThreadHandleJumpServerException();

    isOn = true;
    isPaused = false;

    argJump->isKhOn = &isOn;
    argJump->isKhPaused = &isPaused;
    argJump->keyPressed = &keyPressed;
    argJump->emisor = &emisor;

    if (pthread_create(handleJumpTH, NULL, handleJumpFunc, argJump))
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