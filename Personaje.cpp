//
// Created by juancho on 05/10/16.
//

#include "ObjectManager.h"
#include "Personaje.h"
#include "NivelManager.h"

#define ENVOLVENTE_PARADO_DERECHA 0
#define ENVOLVENTE_PARADO_IZQUIERDA 1
#define ENVOLVENTE_CORRIENDO_DERECHA 2
#define ENVOLVENTE_CORRIENDO_IZQUIERDA 3
#define ENVOLVENTE_SALTANDO_DERECHA 4
#define ENVOLVENTE_SALTANDO_IZQUIERDA 5

Personaje::Personaje() {

    Envolvente* env = new Envolvente();
    Rectangulo* rect = new Rectangulo(&posx, &posy, 45, 81);
    env->agregarComponente(rect);
    envolventesPosibles.push_back(env);

    env = new Envolvente();
    rect = new Rectangulo(&posx, &posy, 45, 81);
    rect->setOffset(105, 0);
    env->agregarComponente(rect);
    envolventesPosibles.push_back(env);

    env = new Envolvente();
    rect = new Rectangulo(&posx, &posy, 52, 92);
    env->agregarComponente(rect);
    envolventesPosibles.push_back(env);

    env = new Envolvente();
    rect = new Rectangulo(&posx, &posy, 52, 92);
    rect->setOffset(98, 0);
    env->agregarComponente(rect);
    envolventesPosibles.push_back(env);

    env = new Envolvente();
    rect = new Rectangulo(&posx, &posy, 63, 93);
    rect->setOffset(0, 6);
    env->agregarComponente(rect);
    envolventesPosibles.push_back(env);

    env = new Envolvente();
    rect = new Rectangulo(&posx, &posy, 63, 93);
    rect->setOffset(87, 6);
    env->agregarComponente(rect);
    envolventesPosibles.push_back(env);

    envolvente = envolventesPosibles[ENVOLVENTE_PARADO_DERECHA];
}

void Personaje::moverX() {
    ObjectManager* objectManager = ObjectManager::getInstance();
    int* posX = objectManager->getPosX();

    int pos1 = posCamara;
    int pos2 = *posX;

    //moverlo a derecha o izquierda
    *posX += velx;
    posCamara += velx;

    if (velx > 0) {
        envolvente = envolventesPosibles[ENVOLVENTE_CORRIENDO_DERECHA];
        derecha = true;
    }
    else if (velx == 0){
        if (derecha) envolvente = envolventesPosibles[ENVOLVENTE_PARADO_DERECHA];
        else envolvente = envolventesPosibles[ENVOLVENTE_PARADO_IZQUIERDA];
    }
    else if (velx < 0){
        envolvente = envolventesPosibles[ENVOLVENTE_CORRIENDO_IZQUIERDA];
        derecha = false;
    }

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

    if (posy == ultimaPosy) setSprites();
    seMovio = true;
}

void Personaje::moverY() {
    int pos2 = posy;

    posy += vely;

    if (derecha) envolvente = envolventesPosibles[ENVOLVENTE_SALTANDO_DERECHA];
    else envolvente = envolventesPosibles[ENVOLVENTE_SALTANDO_IZQUIERDA];

    if( ( posy < 0 ) || ( posy + envolvente->getAlto() > LEVEL_HEIGHT ) ) {
        //Move back
        posy -= vely;
    }

    if (pos2 == posy) {
        seMovio = false;
        return;
    }

    if (posy == ultimaPosy){
        seMovio = false;
        return;
    }

    if (NivelManager::getInstance()->hayColision(this)) {
        seMovio = false;
        posy -= vely;
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
    this->posy = 465;
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
    if( frameSaltando / 4 >= ANIMACION_SALTANDO ) frameSaltando = 0;
}

int Personaje::getSpriteSaltando() {
    return frameSaltando / 4;
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
    if (posy != ultimaPosy) {
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
    if (posy != ultimaPosy) {
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

Envolvente* Personaje::getEnvolvente(){
    return envolvente;
}

Personaje::~Personaje() {
    for (auto envolvente : this->envolventesPosibles)
        delete envolvente;
}

void Personaje::setUltimaPosy(int aux) {
    ultimaPosy = aux;
}
