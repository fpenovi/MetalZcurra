//
// Created by nestor on 26/11/16.
//

#include "VistaBoss.h"

bool VistaBoss::estaVivo() {
	return !muerto;
}

void VistaBoss::setMuerto(bool muerto) {
	this->muerto = muerto;
}