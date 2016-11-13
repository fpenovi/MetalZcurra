//
// Created by nestor on 13/11/16.
//

#include "Direccion.h"

Direccion::Direccion() {
    derecha = true;
    izquierda = false;
    arriba = false;
    abajo = false;
    ultimaDerecha = true;
    solte = true;
}

bool Direccion::isDerecha() {
    return derecha;
}

void Direccion::setDerecha(bool derecha) {
    this->derecha = derecha;
}

bool Direccion::isIzquierda() {
    return izquierda;
}

void Direccion::setIzquierda(bool izquierda) {
    this->izquierda = izquierda;
}

bool Direccion::isArriba() {
    return arriba;
}

void Direccion::setArriba(bool arriba) {
    this->arriba = arriba;
}

bool Direccion::isAbajo() {
    return abajo;
}

void Direccion::setAbajo(bool abajo) {
    this->abajo = abajo;
}

bool Direccion::isUltimaDerecha() {
    return ultimaDerecha;
}

void Direccion::setUltimaDerecha(bool aux) {
    this->ultimaDerecha = aux;
}

bool Direccion::isSolte() {
    return solte;
}

void Direccion::setSolte(bool aux) {
    this->solte = aux;
}

void Direccion::disparoAlaIzq() {
    setDerecha(false);
    setIzquierda(true);
    setUltimaDerecha(false);
    setSolte(false);
}

void Direccion::disparoAlaDer() {
    setIzquierda(false);
    setDerecha(true);
    setUltimaDerecha(true);
    setSolte(false);
}

void Direccion::disparoArriba() {
    setAbajo(false);
    setArriba(true);
    if (isSolte()){
        setDerecha(false);
        setIzquierda(false);
    }
}

void Direccion::disparoAbajo() {
    setArriba(false);
    setAbajo(true);
    if (isSolte()){
        setDerecha(false);
        setIzquierda(false);
    }
}

void Direccion::ultimaDireccion() {
    if (isUltimaDerecha()) setDerecha(true);
    else setIzquierda(true);
}