//
// Created by nestor on 12/10/16.
//

#include <iostream>
#include "ProtocoloNuevaVista.h"

int ProtocoloNuevaVista::getObject_id() {
    return object_id;
}

int ProtocoloNuevaVista::getX() {
    return x;
}

int ProtocoloNuevaVista::getY() {
    return y;
}

int ProtocoloNuevaVista::getSpriteId() {
    return spriteId;
}

int ProtocoloNuevaVista::getCam() {
    return cam;
}

int ProtocoloNuevaVista::getDuenio() {
    return duenio;
}

void ProtocoloNuevaVista::setObject_id(int object_id) {
    this->object_id = object_id;
}

void ProtocoloNuevaVista::setX(int x) {
    this->x = x;
}

void ProtocoloNuevaVista::setY(int y) {
    this->y = y;
}

void ProtocoloNuevaVista::setSpriteId(int id) {
    this->spriteId = id;
}

void ProtocoloNuevaVista::setCam(int camara) {
    this->cam = camara;
}

void ProtocoloNuevaVista::setConectado(int conectado) {
    this->conectado = conectado;
}

void ProtocoloNuevaVista::setDuenio(int aux) {
    this->duenio = aux;
}

string ProtocoloNuevaVista::toString(){
    string id = to_string(object_id);
    string sprite = to_string(spriteId);
    string posx = to_string(x);
    string posy = to_string(y);
    string camara = to_string(cam);
    string conexion = to_string(conectado);
    string owner = to_string(duenio);

    string msj = id + "$" + sprite + "$" + posx + "$" + posy + "$" + camara + "$" + conexion + "$" + owner + "\n";
    return msj;
}

void ProtocoloNuevaVista::parse(string stream, int* id, int* sprite, int* posx, int* posy, int* camara, int* conexion, int* owner) {

    string id_s = "";
    string sprite_s = "";
    string posx_s = "";
    string posy_s = "";
    string cam_s = "";
    string conectado_s = "";
    string owner_s = "";

    string* variables[] = {&id_s, &sprite_s, &posx_s, &posy_s, &cam_s, &conectado_s, &owner_s};

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
    *sprite = stoi(sprite_s);
    *posx = stoi(posx_s);
    *posy = stoi(posy_s);
    *camara = stoi(cam_s);
    *conexion = stoi(conectado_s);
    *owner = stoi(owner_s);
}