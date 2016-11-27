//
// Created by franch on 25/11/16.
//

#include "VistaPuntajes.h"
#include "VistaPuntajesIndividual.h"
#include "VistaPuntajesColaborativo.h"
#include "VistaPuntajesGrupal.h"


VistaPuntajes::VistaPuntajes() {
	// setearle la pantalla para poder escribir
}

VistaPuntajes* VistaPuntajes::NewVistaPuntaje(int cantPlayers, int modoJuego) {

	if (modoJuego == MODO_INDIVIDUAL)
		return new VistaPuntajesIndividual(cantPlayers);

	if (modoJuego == MODO_COLABORATIVO)
		return new VistaPuntajesColaborativo(cantPlayers);

	if (modoJuego == MODO_GRUPAL)
		return new VistaPuntajesGrupal(cantPlayers);
}