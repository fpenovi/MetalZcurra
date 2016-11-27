/*
 * Bonus.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "Bonus.h"
#include "ObjectManager.h"

#define ANCHO_BONUS 51
#define ALTO_BONUS 45

Bonus::Bonus(int x, int y) {
	this->x = x;
	this->y = y;
	this->envolvente = new Envolvente();
	this->envolvente->agregarComponente(new Rectangulo(&this->x,&this->y , ANCHO_BONUS, ALTO_BONUS));
	this->existe = false;
	this->renderizado = false;
}

void Bonus::setExiste(bool aux) {
	existe = aux;
}

bool Bonus::getExiste() {
	return existe;
}

int Bonus::getId() {
	return id;
}

void Bonus::setId(int aux) {
	id = aux;
}

void Bonus::crear(){
	ObjectManager* objectManager = ObjectManager::getInstance();
	if ( envolvente->getX() + ANCHO_BONUS < 0 ) existe = false;
	else if ((*(objectManager->getPosX()) + 800) > envolvente->getX() ) {
		existe = true;
		x = 800;
	}
}

void Bonus::setPosicion(int x, int y) {
	this->x = x;
	this->y = y;
}

void Bonus::setRenderizado(bool aux) {
	renderizado = aux;
}

bool Bonus::getRenderizado() {
	return renderizado;
}

int Bonus::getPosx() {
	return x;
}

int Bonus::getPosy() {
	return y;
}

bool Bonus::hayColision(){
	ObjectManager* objectManager = ObjectManager::getInstance();

	unordered_map<int, Personaje*>* personajes = objectManager->getPersonajesHash();

	for (auto kv : *personajes){

		if ((kv.second->getEnvolvente())->hayColision(envolvente)) {
			aplicarEfecto(kv.second);
			idColisionado = kv.second->getId();
			return true;
		}
	}
	return false;
}

int Bonus::getIdColisionado() {
	return idColisionado;
}

int Bonus::getTipoDropeable() {
	return tipoDropeable;
}

Bonus::~Bonus() {
	delete this->envolvente;
}

string Bonus::HMGun = "HMGun";

string Bonus::SGun = "SGun";

string Bonus::RLauncher = "RLauncher";

string Bonus::KillAll = "KillAll";

string Bonus::Recover = "Recover";
