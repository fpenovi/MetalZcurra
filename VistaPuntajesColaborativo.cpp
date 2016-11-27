//
// Created by franch on 25/11/16.
//

#include "VistaPuntajesColaborativo.h"


VistaPuntajesColaborativo::VistaPuntajesColaborativo(int cantPlayers) {
	this->puntos = 0;
}


void VistaPuntajesColaborativo::actualizarPuntaje(int id, int puntos) {
	this->puntos += puntos;
}


void VistaPuntajesColaborativo::render() {
	// ToDo do it!
}


VistaPuntajesColaborativo::~VistaPuntajesColaborativo() {

}