//
// Created by juancho on 05/10/16.
//

#include "ProtocoloVistaUpdate.h"

int ProtocoloVistaUpdate::getObject_id() {
    return object_id;
}

int ProtocoloVistaUpdate::getEstado() {
    return estado;
}

int ProtocoloVistaUpdate::getX() {
    return x;
}

int ProtocoloVistaUpdate::getY() {
    return y;
}

void ProtocoloVistaUpdate::setObject_id(int object_id) {
    this->object_id = object_id;
}

void ProtocoloVistaUpdate::setEstado(int estado) {
    this->estado = estado;
}

void ProtocoloVistaUpdate::setX(int x) {
    this->x = x;
}

void ProtocoloVistaUpdate::setY(int y) {
    this->y = y;
}

string ProtocoloVistaUpdate::toString(){
    string id = to_string(object_id);
    string state = to_string(estado);
    string posx = to_string(x);
    string posy = to_string(y);

    string msj = id + "$" + state + "$" + posx + "$" + posy;
    return msj;
}

void ProtocoloVistaUpdate::parse(string stream, int* id, int* state, int* posx, int* posy) {

    string id_s = "";
    string state_s = "";
    string posx_s = "";
    string posy_s = "";

    string* variables[] = {&id_s, &state_s, &posx_s, &posy_s};

    int j = 0;

    for (int i=0; i<stream.size() - 1; i++) {

        char actual = stream[i];

        if (actual == '$') {
            j++;
            continue;
        }

        *(variables[j]) += actual;
    }

    *id = stoi(id_s);
    *state = stoi(state_s);
    *posx = stoi(posx_s);
    *posy = stoi(posy_s);
}