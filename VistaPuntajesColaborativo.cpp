//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesColaborativo.h"


VistaPuntajesColaborativo::VistaPuntajesColaborativo(int cantPlayers, SDL_Renderer* renderer) : VistaPuntajes(renderer) {

	for (int i=1; i<=cantPlayers; i++)
		puntosPlayersById[i] = 0;

	puntaje_t* puntaje = new puntaje_t;
	puntaje->puntos = 0;
	puntaje->s_puntos = "0";
	puntaje->puntajeColor = new SDL_Color{193, 193, 193, 0xFF};
	puntaje->puntajeTexture = new Textura(this->renderer);
	puntaje->x = SCORE_X;
	puntaje->y = SCORE_Y;
	this->puntos = puntaje;
}


void VistaPuntajesColaborativo::actualizarPuntaje(int id, int puntos) {

	int puntosTotales = 0;
	puntosPlayersById[id] = puntos;

	for (auto kv : puntosPlayersById)
		puntosTotales += kv.second;

	this->puntos->puntos = puntosTotales;
	this->puntos->s_puntos = to_string(this->puntos->puntos);
}


void VistaPuntajesColaborativo::render() {
	puntos->puntajeTexture->loadFromText(puntos->s_puntos, *(puntos->puntajeColor), this->gFont);
	puntos->puntajeTexture->render(puntos->x, puntos->y);
}


void VistaPuntajesColaborativo::mostrarResumen() {

}


VistaPuntajesColaborativo::~VistaPuntajesColaborativo() {
	delete puntos->puntajeColor;
	delete puntos->puntajeTexture;
	delete puntos;
}