//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesGrupal.h"

#define CANT_EQUIPOS 2


VistaPuntajesGrupal::VistaPuntajesGrupal(int cantPlayers, SDL_Renderer* renderer) : VistaPuntajes(renderer) {

	int posX = SCORE_X;
	int spacing = 0;		// Desactivo espaciado para primera iteracion

	for (int i = 1; i <= cantPlayers; i++) {
		puntaje_t* actual = new puntaje_t;
		posX = 	posX + spacing * (i-1);							// id 1 --> equipo 2, id 2 --> equipo 1,
		actual->puntos = 0;										// id 3 --> equipo 2, id 4 --> equipo 1.
		actual->s_puntos = "0";
		actual->puntajeColor = teamColorsByTeamId[getTeamNumberByPlayerId(i)];
		actual->puntajeTexture = new Textura(this->renderer);
		actual->x = posX;
		actual->y = SCORE_Y;
		puntosEquiposById[getTeamNumberByPlayerId(i)] = actual;
		if (i == 1) spacing = SCORE_X_SPACING;
	}
}


void VistaPuntajesGrupal::actualizarPuntaje(int id, int puntos) {
	puntaje_t* pointsToUpdate = puntosEquiposById[getTeamNumberByPlayerId(id)];
	pointsToUpdate->puntos = puntos + pointsToUpdate->puntos;
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