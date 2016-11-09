//
// Created by nestor on 06/11/16.
//

#include "Bala.h"

Bala::Bala(){
    velocidad = 30;
    existe = false;
}

void Bala::mover(){
    if (posx > 800) {
        desaparecer();
    }
    posx += velocidad;
}

bool Bala::existeBala(){
    return existe;
}

void Bala::setID(int nuevoID){
    id = nuevoID;
}

void Bala::crear(int idEmisor, int x, int y){
    existe = true;

    posx = x;

    posy = y;

    idDuenio = idEmisor;
}

int Bala::getPosx(){
    return posx;
}

int Bala::getPosy(){
    return posy;
}

int Bala::getAncho(){
    return ancho;
}

int Bala::getAlto(){
    return alto;
}

void Bala::desaparecer(){
    existe = false;
    idDuenio = 0;
    posy = 0;
    posx = 0;
}

int Bala::getId() {
    return id;
}

void Bala::setIdDuenio(int id) {
    this->idDuenio = id;
}

int Bala::getIdDuenio() {
    return idDuenio;
}
