/*
 * SGun.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#include "SGun.h"

SGun::SGun(int x, int y) : Bonus(x, y) {}

void SGun::aplicarEfecto(Personaje *personaje) {
    personaje->setArmaActual(Personaje::SHOTGUN);
    personaje->setShotGunSprites();
}

SGun::~SGun() {}

