//
// Created by nestor on 15/11/16.
//

#include "ObjectManager.h"
#include "EnemigosManager.h"

using namespace chrono;

struct argenemigos {
    bool* isKhOn;
    bool* isKhPaused;
    unordered_map<string, list<Mensaje*>*>* conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash;
};


void* enemigosManagerFunc(void* argKh) {

    bool* isKhOn = ( (argenemigos_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argenemigos_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    unordered_map<string, list<Mensaje*>*>* conectadosHash = ( (argenemigos_t*) argKh)->conectadosHash;
    unordered_map<string, pthread_mutex_t>* mutexesHash = ( (argenemigos_t*) argKh)->mutexesHash;
    unordered_map<int, Enemigo*>* enemigos = objectManager->getEnemigosHash();

    time_point<high_resolution_clock> start;
    start = high_resolution_clock::now();
    microseconds intervalo(40000);	// 40ms

    while (*isKhOn) {
        time_point<high_resolution_clock> actual;
        actual = high_resolution_clock::now();

        auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
        auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

        if (elapsed_ms.count() >= intervalo.count()) {

            if (*isKhOn && !(*isKhPaused)) {

                for (auto kv : *enemigos){

                    if (!(kv.second->getExiste()) && !(kv.second->estaMuerto())){
                        kv.second->crear();
                    }

                    if (kv.second->getExiste()){

                        kv.second->mover();

                        ProtocoloVistaUpdate update;

                        update.setTipoObjeto(4);
                        update.setEstado(true);
                        update.setX(kv.second->getPosx());
                        update.setY(kv.second->getPosy());
                        update.setObject_id(kv.second->getId());
                        update.setPosCamara(0);
                        update.setConectado(kv.second->estaMuerto());
                        update.setSpriteIndex(0);

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


EnemigosManager::EnemigosManager() {
    isOn = false;
    argEnemigos = NULL;
    enemigosManagerTH = NULL;
}

EnemigosManager::~EnemigosManager() {
    delete enemigosManagerTH;
    delete argEnemigos;
}

void EnemigosManager::On() {

    enemigosManagerTH = new pthread_t;

    if (!enemigosManagerTH)
        throw NoSePudoCrearThreadEnemigosManagerException();

    argEnemigos = new argenemigos_t;

    if (!argEnemigos)
        throw NoSePudoCrearThreadEnemigosManagerException();

    isOn = true;
    isPaused = false;

    argEnemigos->isKhOn = &isOn;
    argEnemigos->isKhPaused = &isPaused;
    argEnemigos->conectadosHash = conectadosHash;
    argEnemigos->mutexesHash = mutexesHash;

    if (pthread_create(enemigosManagerTH, NULL, enemigosManagerFunc, argEnemigos))
        throw NoSePudoCrearThreadEnemigosManagerException();
}

void EnemigosManager::Off() {
    if (!isOn)
        return;

    isOn = false;
}

void EnemigosManager::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void EnemigosManager::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}

void EnemigosManager::setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash) {
    this->conectadosHash = hash;
}

void EnemigosManager::setMutexesHash(unordered_map<string, pthread_mutex_t>* hash) {
    this->mutexesHash = hash;
}