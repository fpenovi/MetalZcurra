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
#include "Personaje.h"

using namespace std;

class Bonus {

protected:
	int x;
	int y;
	bool existe;
	int id;
	bool renderizado;
	int idColisionado;
	Envolvente* envolvente;

public:
	static string HMGun;
	static string SGun;
	static string RLauncher;
	static string KillAll;
	static string Recover;

	Bonus(int x, int y);
	void setPosicion(int x, int y);
	int getPosx();
	int getPosy();
	void setExiste(bool aux);
	bool getExiste();
	void setRenderizado(bool aux);
	bool getRenderizado();
	int getIdColisionado();
	int getId();
	void setId(int aux);
	void crear();
	bool hayColision();
	virtual void aplicarEfecto(Personaje* personaje) = 0;
	virtual ~Bonus();
};

#endif /* BONUS_H_ */
