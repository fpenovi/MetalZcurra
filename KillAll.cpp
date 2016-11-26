//
// Created by franch on 11/23/16.
//

#include "KillAll.h"
#include "ObjectManager.h"

KillAll::KillAll(int x, int y) : Bonus(x, y) {
    this->tipoDropeable = 1;
}

void KillAll::aplicarEfecto(Personaje *personaje) {
    ObjectManager::getInstance()->killAll();
}

KillAll::~KillAll() {}