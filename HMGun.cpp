/*
 * HMGun.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "HMGun.h"

HMGun::HMGun(int x, int y) : Bonus(x, y) {
    this->tipoDropeable = 0;
}

void HMGun::aplicarEfecto(Personaje *personaje) {
    personaje->setCantidadBalas(199, 149, Personaje::HMGUN);
    personaje->setArmaActual(Personaje::HMGUN);
    personaje->setShotGunSprites();
}

HMGun::~HMGun(){}

