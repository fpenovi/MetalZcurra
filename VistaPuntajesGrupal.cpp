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


void VistaPuntajesGrupal::mostrarResumen(Juego* juego) {
	int idPlayer;
	int puntajePlayer;
	string namePlayer;

	int posX;
	int posY;

	if (transparenciaActual > LIMITE_INFERIOR_TRANSPARENCIA)
		juego->transparentar(--transparenciaActual);

	temp->loadFromText("EQUIPO: " + puntosEquiposById[2]->s_puntos, *(teamColorsByTeamId[2]), this->gFont);
	temp->render(FINAL_SCORE_VIEW_X, FINAL_SCORE_VIEW_Y - 65);
	temp->loadFromText("EQUIPO: " + puntosEquiposById[1]->s_puntos, *(teamColorsByTeamId[1]), this->gFont);
	temp->render(FINAL_SCORE_VIEW_X + FINAL_SCORE_X_SPACING, FINAL_SCORE_VIEW_Y - 65);

	for (int i=0; i<puntosPlayersById.size(); i++) {
		idPlayer = i+1;
		puntajePlayer = puntosPlayersById[idPlayer];
		namePlayer = "Jugador " + to_string(idPlayer);
		posX = FINAL_SCORE_VIEW_X + (i % 2) * FINAL_SCORE_X_SPACING;
		posY = FINAL_SCORE_VIEW_Y + (i / 2) * FINAL_SCORE_SPACING_Y;

		// RENDERIZO PRIMERO EL NOMBRE ARRIBA
		temp->loadFromText(namePlayer, *(teamColorsByTeamId[getTeamNumberByPlayerId(idPlayer)]), this->gFont);
		temp->render(posX, posY);
		// RENDERIZO EL PUNTAJE DEBAJO Y CENTRADO
		temp->loadFromText(to_string(puntajePlayer), *(teamColorsByTeamId[getTeamNumberByPlayerId(idPlayer)]), this->gFont);
		temp->render(posX, posY + 40);
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