/*
 * Rshobu.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#include "Rshobu.h"

Rshobu::Rshobu(int x, int y, int delta) : Boss(x, y, delta) {
	this->velocidad = 10;
	this->vida = 1000;
	// this->envolvente->agregarComponente(new Rectangulo(sarasa, sarasa, sarasa, sarasa));
}

Rshobu::~Rshobu() {
	// TODO Auto-generated destructor stub
}

