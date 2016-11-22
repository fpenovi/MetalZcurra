//
// Created by nestor on 09/11/16.
//

#include "ObjectManager.h"
#include "BalasManager.h"

using namespace chrono;

struct argbalas {
    bool* isKhOn;
    bool* isKhPaused;
};


void* balasManagerFunc(void* argKh) {

    bool* isKhOn = ( (argbalas_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argbalas_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    unordered_map<string, list<Mensaje*>*>* conectadosHash = objectManager->getConectadosHash();
    unordered_map<string, pthread_mutex_t>* mutexesHash = objectManager->getMutexesHash();
    unordered_map<int, Bala*>* balas = objectManager->getBalasHash();

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

                for (auto kv : *balas){

                    if (kv.second->existeBala()){

                        ProtocoloVistaUpdate update;

                        kv.second->mover();
                        kv.second->handleColision();

                        update.setTipoObjeto(2);
                        update.setEstado(kv.second->existeBala());
                        update.setX(kv.second->getPosx());
                        update.setY(kv.second->getPosy());
                        update.setObject_id(kv.second->getId());
                        update.setPosCamara(kv.second->getArriba());
                        update.setConectado(kv.second->getAbajo());
                        update.setSpriteIndex(kv.second->getDerecha());
                        update.setApuntando(kv.second->getIzquierda());
                        update.setSaltando(0);


                        int result;
                        string mensaje = update.toString();

                        for (auto kv : *conectadosHash) {

                            Mensaje* mensajeNuevo = new Mensaje("Server", kv.first, mensaje);

                            result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
                            if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

                            kv.second->push_back(mensajeNuevo);

                            result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
                            if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
                        }
                    }
                }
            }

            start = chrono::system_clock::now();
        }
    }

    return NULL;
}


BalasManager::BalasManager() {
    isOn = false;
    argBalas = NULL;
    balasManagerTH = NULL;
}

BalasManager::~BalasManager() {
    delete balasManagerTH;
    delete argBalas;
}

void BalasManager::On() {

    balasManagerTH = new pthread_t;

    if (!balasManagerTH)
        throw NoSePudoCrearThreadBalasManagerException();

    argBalas = new argbalas_t;

    if (!argBalas)
        throw NoSePudoCrearThreadBalasManagerException();

    isOn = true;
    isPaused = false;

    argBalas->isKhOn = &isOn;
    argBalas->isKhPaused = &isPaused;

    if (pthread_create(balasManagerTH, NULL, balasManagerFunc, argBalas))
        throw NoSePudoCrearThreadBalasManagerException();
}

void BalasManager::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*balasManagerTH, NULL);
}

void BalasManager::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void BalasManager::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}