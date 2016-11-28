//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJES_H
#define METALZCURRA_VISTAPUNTAJES_H

#include "Juego.h"
#include <string>
#include <SDL2/SDL_render.h>
#include <unordered_map>
#include "Textura.h"
#include "GrayOutHandler.h"

using namespace std;

typedef struct {
	Textura* puntajeTexture;
	SDL_Color* puntajeColor;
	int x;
	int y;
	int puntos;
	string s_puntos;
} puntaje_t;


class Juego;	// Para que pueda compilar al estar una clase incluida en la otra
class GrayOutHandler;

class VistaPuntajes {

protected:
	SDL_Renderer* renderer;
	unordered_map<int, SDL_Color*> playerColorsById;
	unordered_map<int, SDL_Color*> teamColorsByTeamId;
	unordered_map<int, int> puntosPlayersById;
	Textura* temp;
	TTF_Font* gFont;
	GrayOutHandler* screenGrayer;

	Uint8 LIMITE_SUPERIOR_TRANSPARENCIA = 255;
	Uint8 transparenciaActual = 255;
	Uint8 LIMITE_INFERIOR_TRANSPARENCIA = 0;
	const int SCORE_X = 50;
	const int SCORE_Y = 15;
	const int RED_TEAM_X = 275;
	const int GREEN_TEAM_X = 475;
	const int SCORE_X_SPACING = 175;
	const int FINAL_SCORE_VIEW_X = 100;
	const int FINAL_SCORE_VIEW_Y = 200;
	const int FINAL_SCORE_X_SPACING = 300;
	const int FINAL_SCORE_SPACING_Y = 100;

	VistaPuntajes(SDL_Renderer* renderer);

public:
	const static int MODO_INDIVIDUAL = 0;
	const static int MODO_COLABORATIVO = 1;
	const static int MODO_GRUPAL = 2;
	const static int CANT_EQUIPOS = 2;

	static VistaPuntajes* NewVistaPuntaje(int cantPlayers, int modoJuego, SDL_Renderer* renderer);
	virtual void actualizarPuntaje(int idJugador, int puntos) = 0;
	virtual void render() = 0;
	virtual void mostrarResumen(Juego* juego) = 0;
	virtual ~VistaPuntajes();
};


#endif //METALZCURRA_VISTAPUNTAJES_H
