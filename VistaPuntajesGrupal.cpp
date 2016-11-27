//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesGrupal.h"


VistaPuntajesGrupal::VistaPuntajesGrupal(int cantPlayers, SDL_Renderer* renderer) : VistaPuntajes(renderer) {

	int posX[] = {GREEN_TEAM_X, RED_TEAM_X};

	for (int i = 1; i <= cantPlayers; i++) {
		puntaje_t* actual = new puntaje_t;						// id 1 --> equipo 2, id 2 --> equipo 1,
		actual->puntos = 0;										// id 3 --> equipo 2, id 4 --> equipo 1.
		actual->s_puntos = "0";
		actual->puntajeColor = teamColorsByTeamId[getTeamNumberByPlayerId(i)];
		actual->puntajeTexture = new Textura(this->renderer);
		actual->x = posX[i % CANT_EQUIPOS];
		actual->y = SCORE_Y;
		puntosEquiposById[getTeamNumberByPlayerId(i)] = actual;
		puntosPlayersById[i] = 0;
	}
}


void VistaPuntajesGrupal::actualizarPuntaje(int id, int puntos) {

	int idEquipo = getTeamNumberByPlayerId(id);
	int totalEquipo = 0;
	puntaje_t* pointsToUpdate = puntosEquiposById[idEquipo];
	puntosPlayersById[id] = puntos;

	for (auto kv : puntosPlayersById) {
		int playerId = kv.first;

		if ((playerId % CANT_EQUIPOS + 1) == idEquipo)
			totalEquipo += kv.second;
	}

	pointsToUpdate->puntos = totalEquipo;
	pointsToUpdate->s_puntos = to_string(pointsToUpdate->puntos);
}


void VistaPuntajesGrupal::render() {
	puntaje_t* puntaje;

	for (auto kv : puntosEquiposById) {
		puntaje = kv.second;
		puntaje->puntajeTexture->loadFromText(puntaje->s_puntos, *(puntaje->puntajeColor), this->gFont);
		puntaje->puntajeTexture->render(puntaje->x, puntaje->y);
	}
}


int VistaPuntajesGrupal::getTeamNumberByPlayerId(int id) {
	return (id % CANT_EQUIPOS) + 1;
}


VistaPuntajesGrupal::~VistaPuntajesGrupal() {
	for (auto kv : this->puntosEquiposById) {
		puntaje_t* actual = kv.second;
		delete actual->puntajeTexture;
		delete actual;
	}
}