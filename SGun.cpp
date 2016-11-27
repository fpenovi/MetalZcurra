/*
 * SGun.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#include "SGun.h"

SGun::SGun(int x, int y) : Bonus(x, y) {
    this->tipoDropeable = 0;
}

void SGun::aplicarEfecto(Personaje *personaje) {
    personaje->setCantidadBalas(29, 9, Personaje::SHOTGUN);
    personaje->setArmaActual(Personaje::SHOTGUN);
    personaje->setShotGunSprites();
}

SGun::~SGun() {}

