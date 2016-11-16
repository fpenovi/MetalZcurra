//
// Created by nestor on 15/11/16.
//

#include "ObjectManager.h"
#include "Enemigo.h"

Enemigo::Enemigo(){
    posy = 440;
    posx = 600;
    ancho = 60;
    alto = 80;
    existe = false;
    muerto = false;
    velocidad = 7;
}

void Enemigo::mover() {
    if (posx < 0) velocidad = -7;
    if (velocidad == -7 && posx > 800) velocidad = 7;
    posx -= velocidad;
}

void Enemigo::morir(){
    muerto = true;
    existe = false;
}

int Enemigo::getAncho(){
    return ancho;
}

int Enemigo::getAlto(){
    return alto;
}

int Enemigo::getPosx(){
    return posx;
}

int Enemigo::getPosy(){
    return posy;
}

int Enemigo::getId() {
    return id;
}

void Enemigo::setId(int id) {
    this->id = id;
}

void Enemigo::setPosx(int x) {
    posx = x;
}

int Enemigo::estaMuerto() {
    return muerto;
}

int Enemigo::getExiste() {
    return existe;
}

void Enemigo::crear() {
    ObjectManager* objectManager = ObjectManager::getInstance();
    if ((*(objectManager->getPosX()) + 800) > posx ) existe = true;
    if ( posx < 0 ) existe = false;
}