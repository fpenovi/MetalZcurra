/*
 * HMGun.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef HMGUN_H_
#define HMGUN_H_

#include "Bonus.h"

class HMGun: public Bonus {

public:
	HMGun(int x, int y);
	void aplicarEfecto(Personaje* personaje);
	virtual ~HMGun();
};

#endif /* HMGUN_H_ */
