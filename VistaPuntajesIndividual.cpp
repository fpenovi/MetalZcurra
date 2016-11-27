//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesIndividual.h"


VistaPuntajesIndividual::VistaPuntajesIndividual(int cantPlayers) {

	for (int i = 1; i <= cantPlayers; i++)
		puntosById[i] = 0;


	// setear posiciones en donde se van a graficar los puntos en pantalla
}


void VistaPuntajesIndividual::actualizarPuntaje(int id, int puntos) {
	puntosById[id] = puntosById[id] + puntos;
}


void VistaPuntajesIndividual::render() {
	// ToDo do it
}


VistaPuntajesIndividual::~VistaPuntajesIndividual() {

}