/*
 * Envolvente.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "Envolvente.h"

Envolvente::Envolvente() {
	// TODO Auto-generated constructor stub

}

void Envolvente::agregarComponente(Rectangulo* rect) {
	this->componentes.push_back(rect);
}

Envolvente::~Envolvente() {
	// TODO Auto-generated destructor stub
}

