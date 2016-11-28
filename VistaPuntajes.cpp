//
// Created by franch on 25/11/16.
//

#include "VistaPuntajes.h"
#include "VistaPuntajesIndividual.h"
#include "VistaPuntajesColaborativo.h"
#include "VistaPuntajesGrupal.h"


VistaPuntajes::VistaPuntajes(SDL_Renderer* renderer) {
	this->renderer = renderer;

	this->playerColorsById[1] = new SDL_Color{161, 65, 29, 0xFF};
	this->playerColorsById[2] = new SDL_Color{217, 219, 73, 0xFF};
	this->playerColorsById[3] = new SDL_Color{90, 81, 76, 0xFF};
	this->playerColorsById[4] = new SDL_Color{62, 121, 180, 0xFF};

	this->teamColorsByTeamId[1] = new SDL_Color{161, 65, 29, 0xFF};		// ROJO
	this->teamColorsByTeamId[2] = new SDL_Color{73, 194, 140, 0xFF};	// VERDE

	this->gFont = TTF_OpenFont("imag/sprites/letras/puntos.ttf", 30);
}

VistaPuntajes* VistaPuntajes::NewVistaPuntaje(int cantPlayers, int modoJuego, SDL_Renderer* renderer) {

	if (modoJuego == MODO_INDIVIDUAL)
		return new VistaPuntajesIndividual(cantPlayers, renderer);

	if (modoJuego == MODO_COLABORATIVO)
		return new VistaPuntajesColaborativo(cantPlayers, renderer);

	if (modoJuego == MODO_GRUPAL)
		return new VistaPuntajesGrupal(cantPlayers, renderer);
}

VistaPuntajes::~VistaPuntajes() {
	for (auto kv : this->playerColorsById) {
		SDL_Color* color = kv.second;
		delete color;
	}

	for (auto kv : this->teamColorsByTeamId) {
		SDL_Color* color = kv.second;
		delete color;
	}

	TTF_CloseFont(this->gFont);
}