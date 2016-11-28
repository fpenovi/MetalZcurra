//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJESINDIVIDUAL_H
#define METALZCURRA_VISTAPUNTAJESINDIVIDUAL_H


#include "VistaPuntajes.h"

class VistaPuntajesIndividual : public VistaPuntajes {

private:
	unordered_map<int, puntaje_t*> puntosById;

public:
	VistaPuntajesIndividual(int cantPlayers, SDL_Renderer* renderer);
	void actualizarPuntaje(int id, int puntos);
	void render();
	void mostrarResumen(Juego* juego);
	~VistaPuntajesIndividual();
};


#endif //METALZCURRA_VISTAPUNTAJESINDIVIDUAL_H
