//
// Created by nestor on 09/11/16.
//

#include "VistaBala.h"

VistaBala::VistaBala(Textura* textura) {
    existe = false;
    derecha = true;
    arriba = false;
    abajo = false;
    TEXTURA_BALA = textura;
}

void VistaBala::cargarImagen(){
    ancho = TEXTURA_BALA->getAncho();
    alto = TEXTURA_BALA->getAlto();
}

void VistaBala::render(){
    if (existe){

        double angulo = 0;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (derecha && arriba){
            flip = SDL_FLIP_NONE;
            angulo = 325;
        }
        else if (derecha && abajo){
            flip = SDL_FLIP_NONE;
            angulo = 45;
        }
        else if (!derecha && arriba ){
            flip = SDL_FLIP_HORIZONTAL;
            angulo = 45;
        }
        else if (!derecha && abajo){
            flip = SDL_FLIP_HORIZONTAL;
            angulo = 325;
        }
        else if (!derecha){
            flip = SDL_FLIP_HORIZONTAL;
        }

        TEXTURA_BALA->render(posx,posy,NULL,angulo,NULL, flip);

        if (posx > 800 || posx < 0 || posy < 0 || posy > 520) existe = false;
    }
}

bool VistaBala::existeBala(){
    return existe;
}

void VistaBala::setID(int nuevoID){
    id = nuevoID;
}
void VistaBala::crear(int x, int y){
    existe = true;

    posx = x;

    posy = y;
}

int VistaBala::getPosx(){
    return posx;
}

int VistaBala::getPosy(){
    return posy;
}

int VistaBala::getAncho(){
    return ancho;
}

int VistaBala::getAlto(){
    return alto;
}

void VistaBala::desaparecer(){
    existe = false;
}

void VistaBala::setExiste(bool exist){
    existe = exist;
}

int VistaBala::getId() {
    return id;
}

void VistaBala::setPosX(int x) {
    posx = x;
}

void VistaBala::setPosY(int y) {
    posy = y;
}

void VistaBala::setDerecha(int aux) {
    derecha = (bool) aux;
}

void VistaBala::setArriba(int aux) {
    arriba = (bool) aux;
}

void VistaBala::setAbajo(int aux) {
    abajo = (bool) aux;
}