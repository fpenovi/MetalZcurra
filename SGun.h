/*
 * SGun.h
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#ifndef SGUN_H_
#define SGUN_H_

#include "Bonus.h"

class SGun: public Bonus {

public:
	SGun(int x, int y);
	void aplicarEfecto(Personaje* personaje);
	virtual ~SGun();
};

#endif /* SGUN_H_ */
