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
#include "Personaje.h"

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
	bool hayColision(Personaje* personaje);
	bool hayColisionSalto(Personaje *pPersonaje);
	bool haFinalizadoNivel();
	bool hayMasNiveles();
	void siguienteNivel();
	void moverPlataformas();
	vector<string> getCapas();
};

#endif /* NIVELMANAGER_H_ */
