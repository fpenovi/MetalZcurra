/*
 * Rectangulo.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef RECTANGULO_H_
#define RECTANGULO_H_

class Rectangulo {

private:
	int* x;
	int* y;
	int ancho;
	int alto;

public:
	Rectangulo(int* x, int* y, int ancho, int alto);
	bool hayColision(Rectangulo* otro);
	virtual ~Rectangulo();
};

#endif /* RECTANGULO_H_ */
