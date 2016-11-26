/*
 * Boss.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#include "Boss.h"

Boss::Boss(int x, int y, int delta) {
	this->x = x;
	this->y = y;
	this->delta = delta;
	this->envolvente = new Envolvente();
	this->derecha = false;
	this->existe = false;
	this->disparando = false;
}


bool Boss::estaVivo() {
	return this->vida > 0;
}

int Boss::getId() {
	return id;
}

bool Boss::getExiste() {
	return existe;
}

int Boss::getPosx() {
	return x;
}

void Boss::setPosx(int aux) {
	x = aux;
}

int Boss::getPosy() {
	return y;
}

bool Boss::isDisprando(){
	return disparando;
}

Boss::~Boss() {
	delete this->envolvente;
}
