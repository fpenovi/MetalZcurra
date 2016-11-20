/*
 * NivelManager.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef NIVELMANAGER_H_
#define NIVELMANAGER_H_

#include <string>
#include <vector>
#include "Nivel.h"

using namespace std;

class NivelManager {

private:
	vector<string> xmlNiveles;
	int nivelActual;
	Nivel* nivel;
	static NivelManager* instancia;
	NivelManager();

public:
	~NivelManager();
	static NivelManager* getInstance();
	void setXmlNiveles(vector<string> niveles);
	// bool hayColision(Personaje* personaje);
	bool haFinalizadoNivel();
	bool hayMasNiveles();
	void siguienteNivel();
	vector<string> getCapas();
};

#endif /* NIVELMANAGER_H_ */
