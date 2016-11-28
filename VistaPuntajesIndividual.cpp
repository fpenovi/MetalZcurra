//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesIndividual.h"


VistaPuntajesIndividual::VistaPuntajesIndividual(int cantPlayers, SDL_Renderer* renderer) : VistaPuntajes(renderer) {

	int posX = SCORE_X;
	int spacing = SCORE_X_SPACING;		// Desactivo espaciado para primera iteracion

	for (int i = 1; i <= cantPlayers; i++) {
		puntaje_t* actual = new puntaje_t;
		posX = 	posX + spacing * (i-1);
		actual->puntos = 0;
		actual->s_puntos = "0";
		actual->puntajeColor = playerColorsById[i];
		actual->puntajeTexture = new Textura(this->renderer);
		actual->x = posX;
		actual->y = SCORE_Y;
		puntosById[i] = actual;
		posX = SCORE_X;
	}
}


void VistaPuntajesIndividual::actualizarPuntaje(int id, int puntos) {
	puntaje_t* pointsToUpdate = puntosById[id];
	pointsToUpdate->puntos = puntos;
	pointsToUpdate->s_puntos = to_string(pointsToUpdate->puntos);
}


void VistaPuntajesIndividual::render() {
	puntaje_t* puntaje;

	for (auto kv : puntosById) {
		puntaje = kv.second;
		puntaje->puntajeTexture->loadFromText(puntaje->s_puntos, *(puntaje->puntajeColor), this->gFont);
		puntaje->puntajeTexture->render(puntaje->x, puntaje->y);
	}
}


void VistaPuntajesIndividual::mostrarResumen() {

	int idPlayer;
	puntaje_t* puntajePlayer;
	string namePlayer;

	int posX;
	int posY;

	for (int i=0; i<puntosById.size(); i++) {
		idPlayer = i+1;
		puntajePlayer = puntosById[idPlayer];
		namePlayer = "Jugador " + to_string(idPlayer);
		posX = FINAL_SCORE_VIEW_X + (i % 2) * FINAL_SCORE_X_SPACING;
		posY = FINAL_SCORE_VIEW_Y + (i / 2) * FINAL_SCORE_SPACING_Y;

		// RENDERIZO PRIMERO EL NOMBRE ARRIBA
		puntajePlayer->puntajeTexture->loadFromText(namePlayer, *(puntajePlayer->puntajeColor), this->gFont);
		puntajePlayer->puntajeTexture->render(posX, posY);
		// RENDERIZO EL PUNTAJE DEBAJO Y CENTRADO
		temp->loadFromText(puntajePlayer->s_puntos, *(puntajePlayer->puntajeColor), this->gFont);
		temp->render(posX, posY + 40);
	}
}


VistaPuntajesIndividual::~VistaPuntajesIndividual() {
	for (auto kv : this->puntosById) {
		puntaje_t* actual = kv.second;
		delete actual->puntajeTexture;
		delete actual;
	}
}