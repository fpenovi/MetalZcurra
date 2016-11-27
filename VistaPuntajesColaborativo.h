//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJESCOLABORATIVO_H
#define METALZCURRA_VISTAPUNTAJESCOLABORATIVO_H


#include "VistaPuntajes.h"

class VistaPuntajesColaborativo : public VistaPuntajes {

private:
	int puntos;

public:
	VistaPuntajesColaborativo(int cantPlayers);
	void actualizarPuntaje(int id, int puntos);
	void render();
	~VistaPuntajesColaborativo();
};


#endif //METALZCURRA_VISTAPUNTAJESCOLABORATIVO_H
