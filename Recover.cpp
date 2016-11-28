//
// Created by franch on 11/23/16.
//

#include "Recover.h"


Recover::Recover(int x, int y) : Bonus(x, y) {
    this->tipoDropeable = 2;
}

void Recover::aplicarEfecto(Personaje *personaje) {
    personaje->sumarVida(1);
}

Recover::~Recover() {
    // Llama al destructor de Bonus (padre)
}