//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJESCOLABORATIVO_H
#define METALZCURRA_VISTAPUNTAJESCOLABORATIVO_H


#include "VistaPuntajes.h"

class VistaPuntajesColaborativo : public VistaPuntajes {

private:
	puntaje_t* puntos;

public:
	VistaPuntajesColaborativo(int cantPlayers, SDL_Renderer* renderer);
	void actualizarPuntaje(int id, int puntos);
	void render();
	void mostrarResumen(Juego* juego);
	~VistaPuntajesColaborativo();
};


#endif //METALZCURRA_VISTAPUNTAJESCOLABORATIVO_H
