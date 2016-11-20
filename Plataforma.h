/*
 * Plataforma.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef PLATAFORMA_H_
#define PLATAFORMA_H_

#include "Envolvente.h"

using namespace std;

class Plataforma {

private:
	int x;
	int y;
	Envolvente* envolvente;

public:
	Plataforma(int x, int y, int ancho, int alto);
	// bool hayColision(Personaje* personaje);
	virtual ~Plataforma();
};

#endif /* PLATAFORMA_H_ */
