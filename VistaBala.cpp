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

void VistaBala::cargarImagenShotgun(){
    for (int i = 0; i < ANIMACION_BALA_SHOTGUN ;i++){
        spriteBala[ i ].x = i*299;
        spriteBala[ i ].y = 0;
        spriteBala[ i ].w = 299;
        spriteBala[ i ].h = 137;
    }

    ancho = 299;
    alto = 137;
}

void VistaBala::render(){
    if (existe){

        angulo = 0;
        flip = SDL_FLIP_NONE;
        if (derecha && arriba && !shotgun){
            flip = SDL_FLIP_NONE;
            angulo = 325;
        }
        else if (derecha && abajo && !shotgun){
            flip = SDL_FLIP_NONE;
            angulo = 45;
        }
        else if (izquierda && arriba && !shotgun){
            flip = SDL_FLIP_HORIZONTAL;
            angulo = 45;
        }
        else if (izquierda && abajo && !shotgun){
            flip = SDL_FLIP_HORIZONTAL;
            angulo = 325;
        }
        else if (!izquierda && !derecha && arriba){
            flip = SDL_FLIP_NONE;
            angulo = 270;
        }
        else if (!izquierda && !derecha && abajo){
            flip = SDL_FLIP_NONE;
            angulo = 90;
        }
        else if (izquierda){
            flip = SDL_FLIP_HORIZONTAL;
        }

        if (shotgun) {
            currentClip = &spriteBala[frame];
            TEXTURA_BALA->render(posx, posy, currentClip, angulo, NULL, flip);
            if (frame == 11) desaparecer();
        }
        else if (bomba){
            if ((posx > 800 || posx < 0 || posy < 0 || posy > 520) && !explotando) {
                explotando = true;
                posxExp = posx;
                posyExp = posy;
            }

            if (explotando) explotar();
            else TEXTURA_BALA->render(posx,posy,NULL,angulo,NULL, flip);
        }
        else {
            TEXTURA_BALA->render(posx,posy,NULL,angulo,NULL, flip);
            if (posx > 800 || posx < 0 || posy < 0 || posy > 520) existe = false;
        }

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
    if (!exist && bomba)
        return;
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

void VistaBala::setIzquierda(int aux) {
    izquierda = (bool) aux;
}

void VistaBala::setArriba(int aux) {
    arriba = (bool) aux;
}

void VistaBala::setAbajo(int aux) {
    abajo = (bool) aux;
}

void VistaBala::setShotgun(bool aux) {
    shotgun = aux;
}

bool VistaBala::isShotgun() {
    return shotgun;
}

void VistaBala::setFrame(int aux) {
    frame = aux;
}

void VistaBala::setBomba(bool aux) {
    bomba = aux;
}

bool VistaBala::isBomba() {
    return bomba;
}

void VistaBala::cargarExplosion(SDL_Renderer* renderer) {

    TEXTURA_EXPLOSION = new Textura(renderer);

    int i;
    if( !TEXTURA_EXPLOSION->cargarImagen( "imag/sprites/R-Shobu/explosion.png") )
    {
        printf( "Fallo sprite gelicoptero derecha\n" );
    }
    else
    {
        for (i = 0; i < ANIMACION_EXPLOSION; i++){
            spriteExplosion[ i ].x = i*150;
            spriteExplosion[ i ].y = 0;
            spriteExplosion[ i ].w = 150;
            spriteExplosion[ i ].h = 66;
        }
    }
}

void VistaBala::explotar() {
    frameExplosion++;
    currentClip = &spriteExplosion[frameExplosion / 4];
    TEXTURA_EXPLOSION->render(posxExp, posyExp, currentClip);
    if (frameExplosion / 4 >= ANIMACION_EXPLOSION) {
        desaparecer();
        frameExplosion = 0;
        explotando = false;
    }
}

void VistaBala::setExplotando(bool aux){
    explotando = aux;
    posxExp = posx;
    posyExp = posy + 50;
}

VistaBala::~VistaBala() {
    if (TEXTURA_EXPLOSION != NULL)
        delete TEXTURA_EXPLOSION;
}
