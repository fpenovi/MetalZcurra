//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesColaborativo.h"
#include "VistaPuntajes.h"


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


void VistaPuntajesColaborativo::mostrarResumen(Juego* juego) {

	int idPlayer;
	int puntajePlayer;
	string namePlayer;

	int posX;
	int posY;

	if (transparenciaActual > LIMITE_INFERIOR_TRANSPARENCIA)
		juego->transparentar(--transparenciaActual);

	for (int i=0; i<puntosPlayersById.size(); i++) {
		idPlayer = i+1;
		puntajePlayer = puntosPlayersById[idPlayer];
		namePlayer = "Jugador " + to_string(idPlayer);
		posX = FINAL_SCORE_VIEW_X + (i % 2) * FINAL_SCORE_X_SPACING;
		posY = FINAL_SCORE_VIEW_Y + (i / 2) * FINAL_SCORE_SPACING_Y;

		// RENDERIZO PRIMERO EL NOMBRE ARRIBA
		temp->loadFromText(namePlayer, *(playerColorsById[idPlayer]), this->gFont);
		temp->render(posX, posY);
		// RENDERIZO EL PUNTAJE DEBAJO Y CENTRADO
		temp->loadFromText(to_string(puntajePlayer), *(playerColorsById[idPlayer]), this->gFont);
		temp->render(posX, posY + 40);
	}
	temp->loadFromText("Total: ", *(puntos->puntajeColor), this->gFont);
	temp->render(FINAL_SCORE_VIEW_X + FINAL_SCORE_X_SPACING, 450);
	temp->loadFromText(puntos->s_puntos, *(puntos->puntajeColor), this->gFont);
	temp->render(FINAL_SCORE_VIEW_X + FINAL_SCORE_X_SPACING + 160, 450);
}


VistaPuntajesColaborativo::~VistaPuntajesColaborativo() {
	delete puntos->puntajeColor;
	delete puntos->puntajeTexture;
	delete puntos;
}