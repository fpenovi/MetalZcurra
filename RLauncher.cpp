/*
 * RLauncher.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "RLauncher.h"

RLauncher::RLauncher(int x, int y) : Bonus(x, y) {}

void RLauncher::aplicarEfecto(Personaje *personaje) {
    // ToDo cambiarle el arma al personaje por rocket launcher
}

RLauncher::~RLauncher() {
	// Llama al destructor de Bonus (padre)
}

