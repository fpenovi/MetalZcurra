//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesColaborativo.h"


VistaPuntajesColaborativo::VistaPuntajesColaborativo(int cantPlayers, SDL_Renderer* renderer) : VistaPuntajes(renderer) {
	puntaje_t* puntaje = new puntaje_t;
	puntaje->puntos = 0;
	puntaje->s_puntos = "0";
	puntaje->puntajeColor = new SDL_Color{0, 0, 0, 0xFF};		// ToDo Poner color blanco?
	puntaje->puntajeTexture = new Textura(this->renderer);
	puntaje->x = 350;
	puntaje->y = 500;
	this->puntos = puntaje;
}


void VistaPuntajesColaborativo::actualizarPuntaje(int id, int puntos) {
	this->puntos->puntos = puntos;
	this->puntos->s_puntos = to_string(this->puntos->puntos);
}


void VistaPuntajesColaborativo::render() {
	puntos->puntajeTexture->loadFromText(puntos->s_puntos, *(puntos->puntajeColor), this->gFont);
	puntos->puntajeTexture->render(puntos->x, puntos->y);
}


VistaPuntajesColaborativo::~VistaPuntajesColaborativo() {
	delete puntos->puntajeColor;
	delete puntos->puntajeTexture;
}