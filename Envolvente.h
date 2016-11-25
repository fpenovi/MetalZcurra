/*
 * Envolvente.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef ENVOLVENTE_H_
#define ENVOLVENTE_H_

#include <vector>
#include "Rectangulo.h"

using namespace std;

class Envolvente {

private:
	vector<Rectangulo*> componentes;

public:
	Envolvente();
	void agregarComponente(Rectangulo* rect);
	bool hayColision(Envolvente* otra);
	bool hayColisionConPlataforma(Envolvente *otra);
	vector<Rectangulo*> getComponentes();
	int getX();
	int getY();
	int getAncho();
	int getAlto();
	virtual ~Envolvente();
};

#endif /* ENVOLVENTE_H_ */
