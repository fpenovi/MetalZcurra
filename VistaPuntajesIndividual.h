//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJESINDIVIDUAL_H
#define METALZCURRA_VISTAPUNTAJESINDIVIDUAL_H


#include <bits/unordered_map.h>
#include "VistaPuntajes.h"

class VistaPuntajesIndividual : public VistaPuntajes {

private:
	unordered_map<int, int> puntosById;

public:
	VistaPuntajesIndividual(int cantPlayers);
	void actualizarPuntaje(int id, int puntos);
	void render();
	~VistaPuntajesIndividual();
};


#endif //METALZCURRA_VISTAPUNTAJESINDIVIDUAL_H
