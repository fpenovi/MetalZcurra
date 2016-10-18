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

string ProtocoloNuevaVista::toString(){
    string id = to_string(object_id);
    string sprite = to_string(spriteId);
    string posx = to_string(x);
    string posy = to_string(y);
    string camara = to_string(cam);

    string msj = id + "$" + sprite + "$" + posx + "$" + posy + "$" + camara + "\n";
    return msj;
}

void ProtocoloNuevaVista::parse(string stream, int* id, int* sprite, int* posx, int* posy, int* camara) {

    string id_s = "";
    string sprite_s = "";
    string posx_s = "";
    string posy_s = "";
    string cam_s = "";

    string* variables[] = {&id_s, &sprite_s, &posx_s, &posy_s, &cam_s};

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
}