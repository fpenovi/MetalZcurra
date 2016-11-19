//
// Created by nestor on 06/11/16.
//

#include "Bala.h"

Bala::Bala(){
    velocidad = 30;
    existe = false;

    derecha = false;
    izquierda = false;
    arriba = false;
    abajo = false;
}

void Bala::mover(){
    if (posx > 800 || posx < 0 || posy < 0 || posy > 520) {
        desaparecer();
        return;
    }

    if (derecha) posx += velocidad;
    if (izquierda) posx -= velocidad;
    if (arriba) posy -= velocidad;
    if (abajo) posy += velocidad;
}

bool Bala::existeBala(){
    return existe;
}

void Bala::setID(int nuevoID){
    id = nuevoID;
}

void Bala::crear(int idEmisor, int x, int y, Direccion* direccion){
    existe = true;
    posx = x;
    posy = y;
    idDuenio = idEmisor;

    setDireccion(direccion->isDerecha(), direccion->isIzquierda(), direccion->isArriba(), direccion->isAbajo());
}

void Bala::crearBalaEnemiga(int x, int y, bool derecha) {
    existe = true;
    posx = x;
    posy = y;

    setDireccion(derecha, !derecha, false, false);
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

void Bala::setDireccion(bool der, bool izq, bool arr, bool abj) {
    this->derecha = der;
    this->izquierda = izq;
    this->arriba = arr;
    this->abajo = abj;
}

int Bala::getDerecha() {
    return derecha;
}

int Bala::getArriba() {
    return arriba;
}

int Bala::getAbajo() {
    return abajo;
}

int Bala::getIzquierda() {
    return izquierda;
}