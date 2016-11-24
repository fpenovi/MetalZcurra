/*
 * Bonus.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "Bonus.h"

#define ANCHO_BONUS 51
#define ALTO_BONUS 45

Bonus::Bonus(int x, int y) {
	this->x = x;
	this->y = y;
	this->envolvente = new Envolvente();
	this->envolvente->agregarComponente(new Rectangulo(&this->x,&this->y , ANCHO_BONUS, ALTO_BONUS));
}

void Bonus::setPosicion(int x, int y) {
	this->x = x;
	this->y = y;
}

Bonus::~Bonus() {
	// TODO Auto-generated destructor stub
}

string Bonus::HMGun = "HMGun";
string Bonus::SGun = "SGun";
string Bonus::RLauncher = "RLauncher";
string Bonus::KillAll = "KillAll";
string Bonus::Recover = "Recover";
