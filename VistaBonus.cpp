//
// Created by nestor on 25/11/16.
//

#include "VistaBonus.h"

VistaBonus::VistaBonus(Textura *textura) {
    existe = false;
    TEXTURA_BONUS = textura;
}

void VistaBonus::render() {
    if (existe)
        TEXTURA_BONUS->render(x, y);
}

void VistaBonus::setPosx(int aux) {
    x = aux;
}

void VistaBonus::setPosy(int aux) {
    y = aux;
}

void VistaBonus::setExiste(bool aux) {
    existe = aux;
}

void VistaBonus::setId(int id) {
    this->id = id;
}

bool VistaBonus::getExiste() {
    return existe;
}

int VistaBonus::getPosx() {
    return x;
}

int VistaBonus::reiniciar(){
    existe = false;
}