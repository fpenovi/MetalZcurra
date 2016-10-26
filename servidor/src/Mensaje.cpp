/*
 * Mensaje.cpp
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */


#include "Mensaje.h"

Mensaje::Mensaje(string nombreEnviador,string nombreReceptor, string str){
    enviador = nombreEnviador;
    receptor = nombreReceptor;
    texto = str;
}

string Mensaje::getMensaje() {
    return texto;
}

string Mensaje::getNameReceptor() {
    return receptor;
}

string Mensaje::getNameEmisor(){
    return enviador;
}

Mensaje::~Mensaje() {
	// TODO Auto-generated destructor stub
}
