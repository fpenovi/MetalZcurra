/*
 * Rshobu.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: franch
 */

#include "Rshobu.h"
#include "ObjectManager.h"

Rshobu::Rshobu(int x, int y, int delta) : Boss(x, y, delta) {
	this->velocidad = 7;
	this->vida = 1000;
	this->id = 1;
	this->envolvente->agregarComponente(new Rectangulo(&this->x, &this->y, 181, 100));
}

void Rshobu::animacionDerecha(){
	++frameDerecha;
	if( frameDerecha / 4 >= ANIMACION_HELICOPTERO_DERECHA ) frameDerecha = 0;
}

void Rshobu::animacionIzquierda(){
	++frameIzquierda;
	if( frameIzquierda / 4 >= ANIMACION_HELICOPTERO_IZQUIERDA ) frameIzquierda = 0;
}

void Rshobu::moverIzquierda(){
	x -= velocidad;
	setSprites();
	if (x < 0) {
		derecha = true;
		frameDerecha = 0;
		frameIzquierda = 0;
	}
}

void Rshobu::moverDerecha(){
	x += velocidad;
	setSprites();
	if (x + envolvente->getAncho() > 800) {
		derecha = false;
		frameDerecha = 0;
		frameIzquierda = 0;
	}
}

void Rshobu::mover(){
	if (!disparando) disparar();
	if (derecha) moverDerecha();
	else moverIzquierda();
}

void Rshobu::setSprites() {
	if (derecha) animacionDerecha();
	else animacionIzquierda();
}

int Rshobu::getSprites() {
	if (derecha) return (frameDerecha / 4);
	else return (frameIzquierda / 4);
}

void Rshobu::crear() {
	ObjectManager* objectManager = ObjectManager::getInstance();
	if ((*(objectManager->getPosX()) + 800) > envolvente->getX() ) {
		existe = true;
		x = 800;
	}
}

bool Rshobu::disparar() {
	microseconds intervalo(3000000);	// 3s
	actual = high_resolution_clock::now();

	auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
	auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

	if (elapsed_ms.count() >= intervalo.count() && estaVivo()) {
		ObjectManager* objectManager = ObjectManager::getInstance();
		objectManager->inicializarBalaBoss(x + 50, y + 100);
		start = chrono::system_clock::now();
	}
}


Rshobu::~Rshobu() {
	// TODO Auto-generated destructor stub
}
