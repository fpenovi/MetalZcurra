//
// Created by juancho on 05/10/16.
//

#include <iostream>
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

int ProtocoloVistaUpdate::getPosCamara(){
    return posCamara;
}

int ProtocoloVistaUpdate::getSpriteIndex() {
    return spriteIndex;
}

int ProtocoloVistaUpdate::getApuntando() {
    return apuntando;
}

int ProtocoloVistaUpdate::getSaltando() {
    return saltando;
}

int ProtocoloVistaUpdate::getPuntaje() {
    return puntaje;
}

void ProtocoloVistaUpdate::setTipoObjeto(int tipo) {
    this->tipoObjeto = tipo;
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

void ProtocoloVistaUpdate::setApuntando(int i) {
    this->apuntando = i;
}

void ProtocoloVistaUpdate::setSaltando(int aux) {
    saltando = aux;
}

void ProtocoloVistaUpdate::setPuntaje(int aux) {
    puntaje = aux;
}

string ProtocoloVistaUpdate::toString(){
    string tipo = to_string(tipoObjeto);
    string id = to_string(object_id);
    string state = to_string(estado);
    string posx = to_string(x);
    string posy = to_string(y);
    string posCam = to_string(posCamara);
    string conexion = to_string(conectado);
    string index = to_string(spriteIndex);
    string aim = to_string(apuntando);
    string salto = to_string(saltando);
    string points = to_string(puntaje);

    string msj = tipo + "$" + id + "$" + state + "$" + posx + "$" + posy + "$" + posCam + "$" + conexion + "$" + index + "$" + aim + "$" + salto + "$" + points +  "\n";
    return msj;
}

void ProtocoloVistaUpdate::parse(string stream, int* tipo, int* id, int* state, int* posx, int* posy, int* posCamara, int* conectado, int* spriteIndex, int* aim, int* salto, int* points) {

    string tipo_s = "";
    string id_s = "";
    string state_s = "";
    string posx_s = "";
    string posy_s = "";
    string posCamara_s = "";
    string conectado_s = "";
    string index_s = "";
    string aim_s = "";
    string salto_s = "";
    string points_s = "";

    string* variables[] = {&tipo_s, &id_s, &state_s, &posx_s, &posy_s, &posCamara_s, &conectado_s, &index_s, &aim_s, &salto_s, &points_s};

    int j = 0;

    for (int i=0; i<stream.size() - 1; i++) {

        char actual = stream[i];

        if (actual == '$') {
            j++;
            continue;
        }

        *(variables[j]) += actual;
    }

    *tipo = stoi(tipo_s);
    *id = stoi(id_s);
    *state = stoi(state_s);
    *posx = stoi(posx_s);
    *posy = stoi(posy_s);
    *posCamara = stoi(posCamara_s);
    *conectado = stoi(conectado_s);
    *spriteIndex = stoi(index_s);
    *aim = stoi(aim_s);
    *salto = stoi(salto_s);
    *points = stoi(points_s);
}
