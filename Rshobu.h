/*
 * Rshobu.h
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#ifndef RSHOBU_H_
#define RSHOBU_H_

#include "Boss.h"

class Rshobu: public Boss {

public:
	Rshobu(int x, int y, int delta);
	virtual ~Rshobu();
};

#endif /* RSHOBU_H_ */
