//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesGrupal.h"

#define CANT_EQUIPOS 2


VistaPuntajesGrupal::VistaPuntajesGrupal(int cantPlayers) {

	for (int i = 1; i <= cantPlayers; i++)						// id 1 --> equipo 2, id 2 --> equipo 1,
		puntosEquiposById[getTeamNumberByPlayerId(i)];			// id 3 --> equipo 2, id 4 --> equipo 1.
}


void VistaPuntajesGrupal::actualizarPuntaje(string id, int puntos) {

}


void VistaPuntajesGrupal::render() {
	// ToDo do it!
}


string VistaPuntajesGrupal::getTeamNumberByPlayerId(int id) {
	return to_string((id % CANT_EQUIPOS) + 1);
}


VistaPuntajesGrupal::~VistaPuntajesGrupal() {

}