/*
 * ProtocoloComando.cpp
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#include "ProtocoloComando.h"

ProtocoloComando::ProtocoloComando() {
	// TODO Auto-generated constructor stub

}

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

    string msj = code + "$" + tipo + "\n";
    return msj;
}

void ProtocoloComando::parse(string stream, int* key, int* pressed) {

        string key_s = "";
        string pressed_s = "";

        string* variables[] = {&key_s, &pressed_s};

        int j = 0;

        for (int i=0; i<stream.size() - 1; i++) {

            char actual = stream[i];

            if (actual == '$') {
                j++;
                continue;
            }

            *(variables[j]) += actual;
        }

        *key = stoi(key_s);
        *pressed = stoi(pressed_s);
}

ProtocoloComando::~ProtocoloComando() {
	// TODO Auto-generated destructor stub
}

