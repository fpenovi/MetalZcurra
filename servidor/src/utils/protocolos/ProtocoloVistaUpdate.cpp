/*
 * ProtocoloVistaUpdate.cpp
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#include "ProtocoloVistaUpdate.h"

ProtocoloVistaUpdate::ProtocoloVistaUpdate() {
	// TODO Auto-generated constructor stub

}

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

int ProtocoloVistaUpdate::getPosCamara(){
    return posCamara;
}

int ProtocoloVistaUpdate::getSpriteIndex() {
    return spriteIndex;
}

void ProtocoloVistaUpdate::setPosCamara(int posCamara) {
    this->posCamara = posCamara;
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

void ProtocoloVistaUpdate::setConectado(int conectado) {
    this->conectado = conectado;
}

void ProtocoloVistaUpdate::setSpriteIndex(int idx) {
    this->spriteIndex = idx;
}

string ProtocoloVistaUpdate::toString(){
    string id = to_string(object_id);
    string state = to_string(estado);
    string posx = to_string(x);
    string posy = to_string(y);
    string posCam = to_string(posCamara);
    string conexion = to_string(conectado);
    string index = to_string(spriteIndex);

    string msj = id + "$" + state + "$" + posx + "$" + posy + "$" + posCam + "$" + conexion + "$" + index + "\n";
    return msj;
}

void ProtocoloVistaUpdate::parse(string stream, int* id, int* state, int* posx, int* posy, int* posCamara, int* conectado, int* spriteIndex) {

    string id_s = "";
    string state_s = "";
    string posx_s = "";
    string posy_s = "";
    string posCamara_s = "";
    string conectado_s = "";
    string index_s = "";

    string* variables[] = {&id_s, &state_s, &posx_s, &posy_s, &posCamara_s, &conectado_s, &index_s};

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
    *posCamara = stoi(posCamara_s);
    *conectado = stoi(conectado_s);
    *spriteIndex = stoi(index_s);
}

ProtocoloVistaUpdate::~ProtocoloVistaUpdate() {
	// TODO Auto-generated destructor stub
}

