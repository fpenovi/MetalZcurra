//
// Created by juancho on 07/09/16.
//

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