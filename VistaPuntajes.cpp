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

VistaPuntajes* VistaPuntajes::NewVistaPuntaje(int cantPlayers, string modoJuego) {

	if (modoJuego.compare(MODO_INDIVIDUAL) == 0)
		return new VistaPuntajesIndividual(cantPlayers);

	if (modoJuego.compare(MODO_COLABORATIVO) == 0)
		return new VistaPuntajesColaborativo(cantPlayers);

	if (modoJuego.compare(MODO_GRUPAL) == 0)
		return new VistaPuntajesGrupal(cantPlayers);
}