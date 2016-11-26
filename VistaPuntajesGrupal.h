//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJESGRUPAL_H
#define METALZCURRA_VISTAPUNTAJESGRUPAL_H


#include <bits/unordered_map.h>
#include "VistaPuntajes.h"

class VistaPuntajesGrupal : public VistaPuntajes {

private:
	unordered_map<string, int> puntosEquiposById;

	string getTeamNumberByPlayerId(int id);

public:
	VistaPuntajesGrupal(int cantPlayers);
	void actualizarPuntaje(string id, int puntos);
	void render();
	~VistaPuntajesGrupal();
};


#endif //METALZCURRA_VISTAPUNTAJESGRUPAL_H
