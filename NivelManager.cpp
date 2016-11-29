/*
 * NivelManager.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "NivelManager.h"

NivelManager::NivelManager() {
	this->nivel = NULL;
	this->nivelActual = 0;
}


void NivelManager::setXmlNiveles(vector<string> niveles) {
	this->xmlNiveles = niveles;
	this->nivel = new Nivel(xmlNiveles[nivelActual]);
}


NivelManager* NivelManager::getInstance() {
	if (!instancia)
		instancia = new NivelManager();

	return instancia;
}


void NivelManager::siguienteNivel() {
	delete this->nivel;
	this->nivel = new Nivel(xmlNiveles[++nivelActual]);
}


vector<string> NivelManager::getCapas() {
	return nivel->getCapas();
}


bool NivelManager::hayColision(Personaje *personaje) {
	return nivel->hayColision(personaje);
}


bool NivelManager::hayMasNiveles() {
	return this->nivelActual < xmlNiveles.size();
}


bool NivelManager::haFinalizadoNivel() {
	return this->nivel->haFinalizado();
}

void NivelManager::moverPlataformas(){
	nivel->moverPlataformas();
}

NivelManager::~NivelManager() {
	delete this->nivel;
}

NivelManager* NivelManager::instancia;
