//
// Created by nestor on 25/11/16.
//

#include "ObjectManager.h"
#include "BonusManager.h"

using namespace chrono;

struct argbonus {
    bool* isKhOn;
    bool* isKhPaused;
};


void* bonusManagerFunc(void* argKh) {

    bool* isKhOn = ( (argbonus_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argbonus_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    unordered_map<string, list<Mensaje*>*>* conectadosHash = objectManager->getConectadosHash();
    unordered_map<string, pthread_mutex_t>* mutexesHash = objectManager->getMutexesHash();
    unordered_map<int, Bonus*>* bonuses = objectManager->getBonusesHash();

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

                for (auto kv : *bonuses){

                    if (!(kv.second->getExiste()) && !kv.second->getRenderizado())
                        kv.second->crear();

                    if (kv.second->getExiste() && !kv.second->getRenderizado()){

                        kv.second->setRenderizado(true);

                        ProtocoloVistaUpdate update;

                        update.setTipoObjeto(5);
                        update.setEstado(kv.second->getExiste());
                        update.setX(kv.second->getPosx());
                        update.setY(kv.second->getPosy());
                        update.setObject_id(kv.second->getId());
                        update.setPosCamara(0);
                        update.setConectado(0);
                        update.setSpriteIndex(0);
                        update.setApuntando(0);
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

                    if (kv.second->getExiste() && kv.second->hayColision()){

                        kv.second->setExiste(false);

                        ProtocoloVistaUpdate update;

                        update.setTipoObjeto(5);
                        update.setEstado(kv.second->getExiste());
                        update.setX(kv.second->getPosx());
                        update.setY(kv.second->getPosy());
                        update.setObject_id(kv.second->getId());
                        update.setPosCamara(0);
                        update.setConectado(kv.second->getIdColisionado());
                        update.setSpriteIndex(0);
                        update.setApuntando(1);
                        update.setSaltando(kv.second->getTipoDropeable());

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


BonusManager::BonusManager() {
    isOn = false;
    argBonus = NULL;
    bonusManagerTH = NULL;
}

BonusManager::~BonusManager() {
    delete bonusManagerTH;
    delete argBonus;
}

void BonusManager::On() {

    bonusManagerTH = new pthread_t;

    if (!bonusManagerTH)
        throw NoSePudoCrearThreadBonusManagerException();

    argBonus = new argbonus_t;

    if (!argBonus)
        throw NoSePudoCrearThreadBonusManagerException();

    isOn = true;
    isPaused = false;

    argBonus->isKhOn = &isOn;
    argBonus->isKhPaused = &isPaused;

    if (pthread_create(bonusManagerTH, NULL, bonusManagerFunc, argBonus))
        throw NoSePudoCrearThreadBonusManagerException();
}

void BonusManager::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*bonusManagerTH, NULL);
}

void BonusManager::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void BonusManager::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}