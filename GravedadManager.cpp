//
// Created by nestor on 29/11/16.
//

#include "ObjectManager.h"
#include "GravedadManager.h"


using namespace chrono;

struct arggravedad {
    bool* isKhOn;
    bool* isKhPaused;
};


void* gravedadManagerFunc(void* argKh) {

    bool* isKhOn = ( (arggravedad_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (arggravedad_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    unordered_map<string, list<Mensaje*>*>* conectadosHash = objectManager->getConectadosHash();
    unordered_map<string, pthread_mutex_t>* mutexesHash = objectManager->getMutexesHash();
    unordered_map<int, Personaje*>* personajes = objectManager->getPersonajesHash();
    int* posX = objectManager->getPosX();

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

                for (auto kv : *personajes){

                    if (kv.second->gravedad()){
                        ProtocoloVistaUpdate update;

                        update.setTipoObjeto(1);
                        update.setEstado(kv.second->getSeMovio());
                        update.setX(*posX);
                        update.setY(kv.second->getPosy());
                        update.setObject_id(kv.second->getId());
                        update.setPosCamara(kv.second->getPosCamara());
                        update.setConectado(kv.second->getConectado());
                        update.setSpriteIndex(kv.second->getSprites());
                        update.setApuntando(kv.second->getDireccion());
                        update.setSaltando(kv.second->getSaltando());
                        update.setPuntaje(kv.second->getPuntaje());

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


GravedadManager::GravedadManager() {
    isOn = false;
    argGravedad = NULL;
    gravedadManagerTH = NULL;
}

GravedadManager::~GravedadManager() {
    delete gravedadManagerTH;
    delete argGravedad;
}

void GravedadManager::On() {

    gravedadManagerTH = new pthread_t;

    if (!gravedadManagerTH)
        throw NoSePudoCrearThreadGravedadManagerException();

    argGravedad = new arggravedad_t;

    if (!argGravedad)
        throw NoSePudoCrearThreadGravedadManagerException();

    isOn = true;
    isPaused = false;

    argGravedad->isKhOn = &isOn;
    argGravedad->isKhPaused = &isPaused;

    if (pthread_create(gravedadManagerTH, NULL, gravedadManagerFunc, argGravedad))
        throw NoSePudoCrearThreadGravedadManagerException();
}

void GravedadManager::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*gravedadManagerTH, NULL);
}

void GravedadManager::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void GravedadManager::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}