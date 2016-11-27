//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJESGRUPAL_H
#define METALZCURRA_VISTAPUNTAJESGRUPAL_H


#include "VistaPuntajes.h"

class VistaPuntajesGrupal : public VistaPuntajes {

private:
	unordered_map<int, puntaje_t*> puntosEquiposById;

	int getTeamNumberByPlayerId(int id);

public:
	VistaPuntajesGrupal(int cantPlayers, SDL_Renderer* renderer);
	void actualizarPuntaje(int id, int puntos);
	void render();
	~VistaPuntajesGrupal();
};


#endif //METALZCURRA_VISTAPUNTAJESGRUPAL_H
