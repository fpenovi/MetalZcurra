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

bool Envolvente::hayColision(Envolvente* otra){

	for (Rectangulo* rect : componentes){
		Rectangulo* otroRect = otra->getComponentes()[0];
		if (rect->hayColision(otroRect)) return true;
	}
	return false;
}

vector<Rectangulo*> Envolvente::getComponentes(){
	return componentes;
}

int Envolvente::getX() {
	Rectangulo* rect = componentes[0];
	return rect->getX();
}

int Envolvente::getY() {
	Rectangulo* rect = componentes[0];
	return rect->getY();
}

Envolvente::~Envolvente() {
	// TODO Auto-generated destructor stub
}
