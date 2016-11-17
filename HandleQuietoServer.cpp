//
// Created by nestor on 31/10/16.
//

#include "HandleQuietoServer.h"

using namespace chrono;

struct argquieto {
    bool* isKhOn;
    bool* isKhPaused;
    string* emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;
};


void* handleQuietoFunc(void* argKh) {

    bool* isKhOn = ( (argquieto_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argquieto_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    string* emisor = ( (argquieto_t*) argKh)->emisor;
    unordered_map<string, list<Mensaje*>*>* conectadosHash = ( (argquieto_t*) argKh)->conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash = ( (argquieto_t*) argKh)->mutexesHash;

    int idEmisor = objectManager->getIdByUsername(*emisor);
    Personaje* personaje = objectManager->getObject(idEmisor);

    time_point<high_resolution_clock> start;
    start = high_resolution_clock::now();
    microseconds intervalo(150000);	// 150ms

    while (*isKhOn) {
        time_point<high_resolution_clock> actual;
        actual = high_resolution_clock::now();

        auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
        auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

        if (elapsed_ms.count() >= intervalo.count()) {

            if (*isKhOn && !(*isKhPaused)) {

                int* posX = objectManager->getPosX();

                ProtocoloVistaUpdate update;

                personaje->setSpriteParado();
                personaje->setVelx(0);
                personaje->moverX();

                update.setTipoObjeto(1);
                update.setEstado(personaje->getSeMovio());
                update.setX(*posX);
                update.setY(personaje->getPosy());
                update.setObject_id(idEmisor);
                update.setPosCamara(personaje->getPosCamara());
                update.setConectado(personaje->getConectado());
                update.setSpriteIndex(personaje->getSprites());

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


HandleQuietoServer::HandleQuietoServer() {
    isOn = false;
    argQuieto = NULL;
    handleQuietoTH = NULL;
    emisor;
}

HandleQuietoServer::~HandleQuietoServer() {
    delete handleQuietoTH;
    delete argQuieto;
}

void HandleQuietoServer::On() {

    handleQuietoTH = new pthread_t;

    if (!handleQuietoTH)
        throw NoSePudoCrearThreadHandleQuietoServerException();

    argQuieto = new argquieto_t;

    if (!argQuieto)
        throw NoSePudoCrearThreadHandleQuietoServerException();

    isOn = true;
    isPaused = false;

    argQuieto->isKhOn = &isOn;
    argQuieto->isKhPaused = &isPaused;
    argQuieto->emisor = &emisor;
    argQuieto->conectadosHash = conectadosHash;
    argQuieto->mutexesHash = mutexesHash;

    if (pthread_create(handleQuietoTH, NULL, handleQuietoFunc, argQuieto))
        throw NoSePudoCrearThreadHandleQuietoServerException();
}

void HandleQuietoServer::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*handleQuietoTH, NULL);
}

void HandleQuietoServer::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void HandleQuietoServer::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}

void HandleQuietoServer::setEmisor(string name) {
    this->emisor = name;
}

void HandleQuietoServer::setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash) {
    this->conectadosHash = hash;
}

void HandleQuietoServer::setMutexesHash(unordered_map<string, pthread_mutex_t>* hash) {
    this->mutexesHash = hash;
}