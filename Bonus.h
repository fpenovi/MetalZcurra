/*
 * Bonus.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef BONUS_H_
#define BONUS_H_

#include <string>
#include "Envolvente.h"

using namespace std;

class Bonus {

protected:
	int x;
	int y;
	Envolvente* envolvente;

public:
	static string HMGun;
	static string SGun;
	static string RLauncher;
	static string KillAll;
	static string Recover;

	Bonus(int x, int y);
	virtual ~Bonus();
};

#endif /* BONUS_H_ */
