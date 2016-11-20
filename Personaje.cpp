//
// Created by juancho on 05/10/16.
//

#include "ObjectManager.h"
#include "Personaje.h"

#define ENVOLVENTE_PARADO 0
#define ENVOLVENTE_CORRIENDO 1
#define ENVOLVENTE_SALTANDO 2

Personaje::Personaje() {

    /*Envolvente* env = new Envolvente();
    env->agregarComponente(new Rectangulo(&sarasa, &sarasa))
    envolventesPosibles.push_back(env);
    env = new Envolvente()
    env->agregarComponente(new Rectangulo(sarasa))
    */
}

void Personaje::moverX() {
    ObjectManager* objectManager = ObjectManager::getInstance();
    int* posX = objectManager->getPosX();

    int pos1 = posCamara;
    int pos2 = *posX;

    //moverlo a derecha o izquierda
    *posX += velx;
    posCamara += velx;

    //Que no salga de la pantalla
    if( ( posCamara  < 0 ) || ( posCamara + ancho > SCREEN_WIDTH *3/4) )  {
        posCamara  -= velx;
    }

    if( ( *posX < 0 ) ) //|| ( *posX + ancho > SCREEN_WIDTH ) )
    {
        *posX -= velx;
    }

    if (velx < 0 || posCamara + ancho + 1 != SCREEN_WIDTH *3/4 || !(objectManager->puedoAvanzar())){
        *posX -= velx;
    }

    if (pos1 == posCamara && pos2 == *posX ) {
        seMovio = false;
        return;
    }

    if (posy == 440) setSprites();
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

    if (posy == 440){
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

int Personaje::getPosCamara() {
    return this->posCamara;
}

void Personaje::setPosCamara(int camara) {
    this->posCamara = camara;
}

void Personaje::inicial(){
    this->posx = 0;
    this->posy = 440;
    this->posCamara=0;
}

int Personaje::getConectado() {
    return conectado;
}

void Personaje::setConectado(int conexion) {
    this->conectado = conexion;
}

void Personaje::setSpriteCorriendo() {
    frameCorriendo++;
    if( frameCorriendo >= ANIMACION_CORRIENDO ) frameCorriendo = 0;
}

int Personaje::getFrameCorriendo() {
    return frameCorriendo;
}

void Personaje::setSpriteSaltando() {
    frameSaltando++;
    if( frameSaltando / 2 >= ANIMACION_SALTANDO ) frameSaltando = 0;
}

int Personaje::getSpriteSaltando() {
    return frameSaltando / 2;
}

void Personaje::setSpriteParado() {
    frameParado++;
    if ( frameParado >= ANIMACION_PARADO ) frameParado = 0;
}

int Personaje::getSpriteParado() {
    return frameParado;
}

void Personaje::setSpriteDisparando() {

    if (arriba) setSpriteDisparandoArriba();

    else if (abajo) setSpriteDisparandoAbajo();

    else {
        frameDisparando++;
        if (frameDisparando >= ANIMACION_ACTUAL) {
            frameDisparando = 0;
            disparando = false;
        }
    }
}

void Personaje::setSpriteDisparandoArriba() {
    frameDisparandoArriba++;
    if( frameDisparandoArriba >= ANIMACION_ACTUAL_DISPARANDO_ARRIBA ){
        frameDisparandoArriba = 0;
        disparando = false;
        arriba = setearArriba;
    }
}

void Personaje::setSpriteDisparandoAbajo(){
    frameDisparandoAbajo++;
    if( frameDisparandoAbajo >= ANIMACION_ACTUAL_DISPARANDO_ABAJO ){
        frameDisparandoAbajo = 0;
        disparando = false;
        abajo = setearAbajo;
    }
}

int Personaje::getSpriteDisparando() {
    if (arriba) return frameDisparandoArriba;

    else if (abajo) return frameDisparandoAbajo;

    else return frameDisparando;
}

int Personaje::getSpriteDisparandoArriba() {
    return frameDisparandoArriba;
}

int Personaje::getSpriteDisparandoAbajo() {
    return frameDisparandoAbajo;
}

void Personaje::resetFrames() {
    frameParado = 0;
    frameSaltando = 0;
    frameCorriendo = 0;
    frameDisparando = 0;
    frameDisparandoArriba = 0;
    frameDisparandoAbajo = 0;
}

void Personaje::setSprites() {
    if (posy != 440) {
        if (disparando) setSpriteDisparando();
        else setSpriteSaltando();
        return;
    }
    if (seMovio){
        if (disparando) setSpriteDisparando();
        else setSpriteCorriendo();
    }
    else {
        if (disparando) setSpriteDisparando();
        else setSpriteParado();
    }

}

int Personaje::getSprites() {
    if (posy != 440) {
        if (disparando) return getSpriteDisparando();
        else return getSpriteSaltando();
    }
    if (seMovio){
        if (disparando) return getSpriteDisparando();
        else return getFrameCorriendo();
    }
    else {
        if (disparando) return getSpriteDisparando();
        else return getSpriteParado();
    }
}

void Personaje::setDisparando(bool aux) {
    disparando = aux;
}

int Personaje::getDisparando() {
    return disparando;
}

void Personaje::setArriba(bool aux){
    if (!disparando) arriba = aux;
    setearArriba = aux;
}

void Personaje::setAbajo(bool aux){
    if (!disparando) abajo = aux;
    setearAbajo = aux;
}

int Personaje::getDireccion(){
    if (arriba) return 1;
    else if (abajo) return 2;
    else return 0;
}

Personaje::~Personaje() {
    for (auto envolvente : this->envolventesPosibles)
        delete envolvente;
}