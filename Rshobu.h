/*
 * Rshobu.h
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#ifndef RSHOBU_H_
#define RSHOBU_H_

#include <chrono>
#include "Boss.h"

using namespace chrono;

class Rshobu: public Boss {

private:
	const int ANIMACION_HELICOPTERO_DERECHA = 12;
	const int ANIMACION_HELICOPTERO_IZQUIERDA = 7;
	const int ANIMACION_HELICE = 5;

	int frameDerecha;
	int frameIzquierda;

	time_point<high_resolution_clock> start;
	time_point<high_resolution_clock> actual;

public:
	Rshobu(int x, int y, int delta);
	virtual ~Rshobu();

	void animacionIzquierda();

	void animacionDerecha();

	void moverIzquierda();

	void moverDerecha();

	void mover();

	void setSprites();

	int getSprites();

	void crear();

	bool disparar();
};

#endif /* RSHOBU_H_ */
