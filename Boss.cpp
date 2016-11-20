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
}

Boss::~Boss() {
	// TODO Auto-generated destructor stub
}

