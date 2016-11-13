//
// Created by nestor on 09/11/16.
//

#include "VistaBala.h"

VistaBala::VistaBala(SDL_Renderer* renderer) {
    existe = false;
    TEXTURA_BALA = new Textura(renderer);
}

bool VistaBala::cargarImagen(){
    if( !TEXTURA_BALA->cargarImagen( "imag/bala/bala.png") )
    {

        printf( "Fallo imagen bala\n" );
        return false;
    }
    ancho = TEXTURA_BALA->getAncho();
    alto = TEXTURA_BALA->getAlto();
    return true;
}

void VistaBala::render(){
    if (existe){
        TEXTURA_BALA->render(posx,posy);
        if (posx > 800 || posx < 0 || posy < 0 || posy > 440) existe = false;
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

VistaBala::~VistaBala() {
    delete TEXTURA_BALA;
}
