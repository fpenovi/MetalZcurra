//
// Created by nestor on 15/11/16.
//

#include "ObjectManager.h"
#include "EnemigosManager.h"

using namespace chrono;

struct argenemigos {
    bool* isKhOn;
    bool* isKhPaused;
};


void* enemigosManagerFunc(void* argKh) {

    bool* isKhOn = ( (argenemigos_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argenemigos_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    unordered_map<string, list<Mensaje*>*>* conectadosHash = objectManager->getConectadosHash();
    unordered_map<string, pthread_mutex_t>* mutexesHash = objectManager->getMutexesHash();
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

                    if (!(kv.second->getExiste()) && !(kv.second->estaMuerto()))
                        kv.second->crear();

                    if (kv.second->getExiste()){

                        if (kv.second->mover()) {

                            ProtocoloVistaUpdate update;

                            update.setTipoObjeto(4);
                            update.setEstado(true);
                            update.setX(kv.second->getPosx());
                            update.setY(kv.second->getPosy());
                            update.setObject_id(kv.second->getId());
                            update.setPosCamara(kv.second->getCantidadPasos());
                            update.setConectado(kv.second->estaMuerto());
                            update.setSpriteIndex(kv.second->getSprite());
                            update.setApuntando(kv.second->isDisparando());

                            int result;
                            string mensaje = update.toString();

                            for (auto kv : *conectadosHash) {

                                Mensaje *mensajeNuevo = new Mensaje("Server", kv.first, mensaje);

                                result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
                                if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

                                kv.second->push_back(mensajeNuevo);

                                result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
                                if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
                            }
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

    if (pthread_create(enemigosManagerTH, NULL, enemigosManagerFunc, argEnemigos))
        throw NoSePudoCrearThreadEnemigosManagerException();
}

void EnemigosManager::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*enemigosManagerTH, NULL);
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