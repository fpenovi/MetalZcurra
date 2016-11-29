/*
 * Rectangulo.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef RECTANGULO_H_
#define RECTANGULO_H_

using namespace std;

class Rectangulo {

private:
	int* x;
	int* y;
	int ancho;
	int alto;
	int offsetX;
	int offsetY;

public:
	Rectangulo(int* x, int* y, int ancho, int alto);
	void setOffset(int x, int y);
	bool hayColision(Rectangulo* otro);
	bool hayColisionSalto(Rectangulo *pRectangulo);
	int getX();
	int getY();
	int getAncho();
	int getAlto();
	virtual ~Rectangulo();
};

#endif /* RECTANGULO_H_ */
