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
    unordered_map<string, list<Mensaje*>*>* conectadosHash = objectManager->getConectadosHash();
    unordered_map<string, pthread_mutex_t>* mutexesHash = objectManager->getMutexesHash();

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

                Direccion* direccion = objectManager->getDireccionById(idEmisor);
                int tipoArma = personaje->getArmaActual();

                if (tipoArma == 0) {
                    if (direccion->isDerecha())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() + 60,personaje->getPosy() + 20, tipoArma);
                    else if (direccion->isIzquierda())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara(), personaje->getPosy() + 20, tipoArma);
                    else if (!direccion->isDerecha() && !direccion->isIzquierda() && direccion->isArriba())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() + 30, personaje->getPosy(), tipoArma);
                    else if (!direccion->isDerecha() && !direccion->isIzquierda() && direccion->isAbajo())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() + 30, personaje->getPosy() + 80, tipoArma);
                }
                else if (tipoArma == 2){
                    if (direccion->isDerecha())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() + 80,personaje->getPosy() - 25, tipoArma);
                    else if (direccion->isIzquierda())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() - 300, personaje->getPosy() - 25, tipoArma);
                    else if (!direccion->isDerecha() && !direccion->isIzquierda() && direccion->isArriba())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() - 110, personaje->getPosy() - 300, tipoArma);
                    else if (!direccion->isDerecha() && !direccion->isIzquierda() && direccion->isAbajo())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() - 137, personaje->getPosy() + 160, tipoArma);
                }
                else {
                    if (direccion->isDerecha())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() + 80,personaje->getPosy() + 30, tipoArma);
                    else if (direccion->isIzquierda())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() - 20, personaje->getPosy() + 30, tipoArma);
                    else if (!direccion->isDerecha() && !direccion->isIzquierda() && direccion->isArriba())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() - 15, personaje->getPosy() - 20, tipoArma);
                    else if (!direccion->isDerecha() && !direccion->isIzquierda() && direccion->isAbajo())
                        objectManager->inicializarBala(idEmisor, personaje->getPosCamara() - 15, personaje->getPosy() + 80, tipoArma);
                }


                int cant;
                if (tipoArma != 0) cant = 4;
                else cant = 10;

                personaje->setDisparando(true);
                for (int i = 0; i < cant; i++){
                    ProtocoloVistaUpdate update;
                    personaje->setSprites();

                    update.setTipoObjeto(3);
                    update.setEstado(personaje->getDisparando());
                    update.setX(0);
                    update.setY(0);
                    update.setObject_id(idEmisor);
                    update.setPosCamara(0);
                    update.setConectado(1);
                    update.setSpriteIndex(personaje->getSprites());
                    update.setApuntando(personaje->getDireccion());
                    update.setSaltando(personaje->getCambioDeArma());
                    update.setPuntaje(0);

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

                personaje->restarBala();
                if (tipoArma == 1){
                    personaje->restarBala();
                    personaje->restarBala();
                    personaje->restarBala();
                    personaje->restarBala();
                }

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
    pthread_join(*handleDisparoTH, NULL);
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