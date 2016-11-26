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
	bool derecha;
	int id;
	bool existe;
	bool disparando;
	Envolvente* envolvente;

public:
	Boss(int x, int y, int delta);
	bool estaVivo();
	int getId();
	bool getExiste();
	int getPosx();
	void setPosx(int aux);
	int getPosy();
	bool isDisprando();
	virtual void mover() = 0;
	virtual void crear() = 0;
	virtual int getSprites() = 0;
	virtual bool disparar() = 0;
	virtual ~Boss();
};

#endif /* BOSS_H_ */
