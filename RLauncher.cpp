/*
 * RLauncher.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "RLauncher.h"

RLauncher::RLauncher(int x, int y) : Bonus(x, y) {
    this->tipoDropeable = 0;
    this->tipoArma = 3;
}

void RLauncher::aplicarEfecto(Personaje *personaje) {
    personaje->setCantidadBalas(29, 14, Personaje::RLAUNCHER);
    personaje->setArmaActual(Personaje::RLAUNCHER);
    personaje->setShotGunSprites();
}

RLauncher::~RLauncher() {}

