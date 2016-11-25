//
// Created by franch on 11/23/16.
//

#include "KillAll.h"

KillAll::KillAll(int x, int y) : Bonus(x, y) {}

void KillAll::aplicarEfecto(Personaje *personaje) {
    // ToDO pedirle al object manager los enemigos, discriminar los que estan en pantalla y matarlos.
}

KillAll::~KillAll() {}