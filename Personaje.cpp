//
// Created by juancho on 05/10/16.
//

#include "Personaje.h"

/*void Personaje::mover(){

    int pos1 = posx;
    int pos2 = posy;
    //moverlo a derecha o izquierda
    posx += velx;

    //Que no salga de la pantalla
    if( ( posx < 0 ) || ( posx + ancho > LEVEL_WIDTH ) ) {
        posx -= velx;
    }

    posy += vely;

    if( ( posy < 0 ) || ( posy + alto > LEVEL_HEIGHT ) ) {
        //Move back
        posy -= vely;
    }

    if (pos1 == posx && pos2 ==  posy) {
        seMovio = false;
        return;
    }
    seMovio = true;
}*/

void Personaje::moverX() {
    int pos1 = posx;

    //moverlo a derecha o izquierda
    posx += velx;

    //Que no salga de la pantalla
    if( ( posx < 0 ) || ( posx + ancho > LEVEL_WIDTH ) ) {
        posx -= velx;
    }

    if (pos1 == posx ) {
        seMovio = false;
        return;
    }
    seMovio = true;

}

void Personaje::moverY() {
    int pos2 = posy;

    posy += vely;

    if( ( posy < 0 ) || ( posy + alto > LEVEL_HEIGHT ) ) {
        //Move back
        posy -= vely;
    }

    if (pos2 ==  posy) {
        seMovio = false;
        return;
    }
    seMovio = true;

}

int Personaje::getPosx() {
    return posx;
}

void Personaje::setPosx(int posx) {
    this->posx = posx;
}

int Personaje::getPosy() {
    return posy;
}

void Personaje::setPosy(int posy) {
    this->posy = posy;
}

int Personaje::getVelx() {
    return velx;
}

void Personaje::setVelx(int velx) {
    this->velx = velx;
}

int Personaje::getVely() {
    return vely;
}

void Personaje::setVely(int vely) {
    this->vely = vely;
}

int Personaje::getSeMovio() {
    return seMovio;
}

void Personaje::setSeMovio(int seMovio) {
    this->seMovio = seMovio;
}

int Personaje::getPersonaje_VEL(){
    return Personaje_VEL;
}

int Personaje::getPersonaje_VEL_Y(){
    return Personaje_VEL_Y;
}

int Personaje::getId() {
    return id;
}

void Personaje::setId(int id) {
    this->id = id;
}

bool Personaje::getBajando() {
    return bajando;
}

void Personaje::setBajando(bool bajando){
    this->bajando = bajando;
}