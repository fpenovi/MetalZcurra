//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJES_H
#define METALZCURRA_VISTAPUNTAJES_H

#include <string>
#include <SDL2/SDL_render.h>
#include <unordered_map>
#include "Textura.h"

using namespace std;

typedef struct {
	Textura* puntajeTexture;
	SDL_Color* puntajeColor;
	int x;
	int y;
	int puntos;
	string s_puntos;
} puntaje_t;


class VistaPuntajes {

protected:
	SDL_Renderer* renderer;
	unordered_map<int, SDL_Color*> playerColorsById;
	unordered_map<int, SDL_Color*> teamColorsByTeamId;
	unordered_map<int, int> puntosPlayersById;
	TTF_Font* gFont;

	const int SCORE_X = 50;
	const int SCORE_Y = 15;
	const int RED_TEAM_X = 275;
	const int GREEN_TEAM_X = 475;
	int SCORE_X_SPACING = 175;

	VistaPuntajes(SDL_Renderer* renderer);

public:
	const static int MODO_INDIVIDUAL = 0;
	const static int MODO_COLABORATIVO = 1;
	const static int MODO_GRUPAL = 2;
	const static int CANT_EQUIPOS = 2;

	static VistaPuntajes* NewVistaPuntaje(int cantPlayers, int modoJuego, SDL_Renderer* renderer);
	virtual void actualizarPuntaje(int idJugador, int puntos) = 0;
	virtual void render() = 0;
	virtual ~VistaPuntajes();
};


#endif //METALZCURRA_VISTAPUNTAJES_H
