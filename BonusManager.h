//
// Created by nestor on 25/11/16.
//

#ifndef METALZCURRA_BONUSMANAGER_H
#define METALZCURRA_BONUSMANAGER_H

#include <stdexcept>
#include <pthread.h>
#include <chrono>
#include <signal.h>
#include <unordered_map>
#include <list>
#include "ProtocoloVistaUpdate.h"

using namespace std;

class NoSePudoCrearThreadBonusManagerException : public runtime_error {

public:
    NoSePudoCrearThreadBonusManagerException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadBonusManagerException : public runtime_error {

public:
    NoSePudoCerrarThreadBonusManagerException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argbonus argbonus_t;

class BonusManager {

private:
    pthread_t* bonusManagerTH;
    argbonus_t* argBonus;
    bool isOn;
    bool isPaused;

public:
    BonusManager();

    ~BonusManager();

    void On();

    void Off();

    void Pause();

    void Resume();
};


#endif //METALZCURRA_BONUSMANAGER_H
