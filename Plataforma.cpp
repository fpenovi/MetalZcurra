/*
 * Plataforma.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "Plataforma.h"

Plataforma::Plataforma(int x, int y, int ancho, int alto) {
	this->x = x;
	this->y = y;
	this->envolvente = new Envolvente();
	this->envolvente->agregarComponente(new Rectangulo(&this->x, &this->y, ancho, alto));
}

Envolvente* Plataforma::getEnvolvente(){
	return envolvente;
}

void Plataforma::moverPlataforma(){
	this->x -= 7;
}

Plataforma::~Plataforma() {
	delete this->envolvente;
}

