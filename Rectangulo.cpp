/*
 * Rectangulo.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "Rectangulo.h"

Rectangulo::Rectangulo(int* x, int* y, int ancho, int alto) {
	this->x = x;
	this->y = y;
	this->ancho = ancho;
	this->alto = alto;
	this->offsetX = 0;
	this->offsetY = 0;
}


void Rectangulo::setOffset(int x, int y) {
	this->offsetX = x;
	this->offsetY = y;
}


bool Rectangulo::hayColision(Rectangulo* otro) {
	int thisX = *(this->x) + this->offsetX;
	int thisY = *(this->y) + this->offsetY;
	int otroX = *(otro->x) + otro->offsetX;
	int otroY = *(otro->y) + otro->offsetY;

	bool result = ((thisX + this->ancho) < otroX || (otroX + otro->ancho) < thisX || (thisY + this->alto) < otroY || (otroY + otro->alto) < thisY);
	return !result;

	/*if (X1+W1<X2 or X2+W2<X1 or Y1+H1<Y2 or Y2+H2<Y1):
	Intersection = Empty
	else:
	Intersection = Not Empty*/		// STACK OVERFLOW FTW!
}

int Rectangulo::getX() {
	return *x + offsetX;
}

int Rectangulo::getY() {
	return *y + offsetY;
}

int Rectangulo::getAncho() {
	return ancho;
}

int Rectangulo::getAlto() {
	return alto;
}

Rectangulo::~Rectangulo() { }