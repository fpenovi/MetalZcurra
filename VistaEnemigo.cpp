//
// Created by nestor on 16/11/16.
//

#include "VistaEnemigo.h"

VistaEnemigo::VistaEnemigo(SDL_Renderer* renderer) {
    existe = false;
    muerto = false;
    TEXTURA_ENEMIGO = new Textura(renderer);
}

bool VistaEnemigo::cargarImagen(){
    if( !TEXTURA_ENEMIGO->cargarImagen( "imag/cruz/quieto.png") )
    {

        printf( "Fallo imagen enemigo\n" );
        return false;
    }
    ancho = TEXTURA_ENEMIGO->getAncho();
    alto = TEXTURA_ENEMIGO->getAlto();
    return true;
}

void VistaEnemigo::render(){
    if (existe && !muerto){
        TEXTURA_ENEMIGO->render(posx,posy);
    }
}

bool VistaEnemigo::existeEnemigo(){
    return existe;
}

void VistaEnemigo::setID(int nuevoID){
    id = nuevoID;
}

int VistaEnemigo::getPosx(){
    return posx;
}

int VistaEnemigo::getPosy(){
    return posy;
}

int VistaEnemigo::getAncho(){
    return ancho;
}

int VistaEnemigo::getAlto(){
    return alto;
}

void VistaEnemigo::morir(){
    muerto = true;
    existe = false;
}

void VistaEnemigo::setExiste(bool exist){
    existe = exist;
}

int VistaEnemigo::getId() {
    return id;
}

void VistaEnemigo::setPosX(int x) {
    posx = x;
}

void VistaEnemigo::setPosY(int y) {
    posy = y;
}

VistaEnemigo::~VistaEnemigo() {
    delete TEXTURA_ENEMIGO;
}