/*
 * Boss.h
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#ifndef BOSS_H_
#define BOSS_H_

#include "Envolvente.h"

class Boss {

protected:
	int x;
	int y;
	int delta;
	int velocidad;
	int vida;
	Envolvente* envolvente;

public:
	Boss(int x, int y, int delta);
	bool estaVivo();
	virtual ~Boss();
};

#endif /* BOSS_H_ */
