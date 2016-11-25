/*
 * HMGun.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "HMGun.h"

HMGun::HMGun(int x, int y) : Bonus(x, y) {}

void HMGun::aplicarEfecto(Personaje *personaje) {
    personaje->setArmaActual(Personaje::HMGUN);
    personaje->setShotGunSprites();
}

HMGun::~HMGun(){}

