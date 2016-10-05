//
// Created by duha on 04/10/16.
//

#include "ProtocoloComando.h"

int ProtocoloComando::getScancode() {
    return scancode;
}

int ProtocoloComando::getType() {
    return type;
}

void ProtocoloComando::setScancode(int scancode) {
    this->scancode = scancode;
}

void ProtocoloComando::setType(int type) {
    this->type = type;
}

string ProtocoloComando::toString() {
    string code = to_string(scancode);
    string tipo = to_string(type);

    return (code + "$" + tipo + "\n");
}

static ProtocoloComando ProtocoloComando::parseComando(string stream){

}