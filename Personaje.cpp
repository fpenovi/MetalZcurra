//
// Created by juancho on 05/10/16.
//

#include "ObjectManager.h"
#include "Personaje.h"
#include "NivelManager.h"

Personaje::Personaje() {
    armaActual = GUN;

    envolvente = new Envolvente();
    Rectangulo* rect = new Rectangulo(&posCamara, &posy, 45, 81);
    envolvente->agregarComponente(rect);
    rect = new Rectangulo(&posCamara, &posy, 31, 5); // Rectangulo de los pies
    rect->setOffset(7, 76); // Centro el rectangulo
    envolvente->agregarComponente(rect);

    quieto = true;
    colision = false;
    modoInvencible = false;
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
    if( ( posCamara < 0 )  || (posCamara + ancho > SCREEN_WIDTH *3/4)) posCamara  -= velx;

    //if (posCamara + ancho > SCREEN_WIDTH) posCamara -= velx;

    if( ( *posX < 0 ) ) *posX -= velx;

    if ( (velx < 0 || posCamara + ancho + 1 != SCREEN_WIDTH *3/4 || !(objectManager->puedoAvanzar())))
        *posX -= velx;

    if (*posX > 7200)
        *posX -= velx;

    if (pos1 == posCamara && pos2 == *posX ) {
        seMovio = false;
        return;
    }

    if (!saltando) setSprites();
    seMovio = true;
}

void Personaje::moverY() {
    int pos2 = posy;

    if (NivelManager::getInstance()->hayColisionSalto(this) && posy != ultimaPosy) {
        if (bajando) {
            seMovio = false;
            saltando = false;
            colision = true;
            return;
        }
    }
    else if (NivelManager::getInstance()->hayColision(this) && posy != ultimaPosy && ultimaPosy < 465){
        if (bajando) {
            seMovio = false;
            saltando = false;
            colision = true;
            return;
        }
    }

    posy += vely;

    if( ( posy < 0 ) || ( posy + envolvente->getAlto() > LEVEL_HEIGHT ) ) {
        //Move back
        posy -= vely;
    }

    if (pos2 == posy) {
        seMovio = false;
        saltando = false;
        return;
    }

    if (posy == ultimaPosy){
        seMovio = false;
        saltando = false;
        return;
    }

    saltando = true;
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
    if (saltando) {
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
    if (saltando) {
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

void Personaje::setUltimaPosy(int aux) {
    ultimaPosy = aux;
}

void Personaje::setSaltando(bool aux) {
    saltando = aux;
}

bool Personaje::getSaltando() {
    return saltando;
}

bool Personaje::gravedad() {
    if (!NivelManager::getInstance()->hayColision(this) && posy < 465 && gravity) {
        posy += 15;
        saltando = false;
        return true;
    }
    return false;
}

void Personaje::setGravity(bool aux) {
    gravity = aux;
}

int Personaje::getArmaActual() {
    return armaActual;
}

void Personaje::setArmaActual(int aux) {
    armaActual = aux;
}

void Personaje::setShotGunSprites() {
    ANIMACION_ACTUAL = ANIMACION_DISPARANDO_SHOTGUN;
    ANIMACION_ACTUAL_DISPARANDO_ABAJO = ANIMACION_DISPARANDO_SHOTGUN_ABAJO;
    ANIMACION_ACTUAL_DISPARANDO_ARRIBA = ANIMACION_DISPARANDO_SHOTGUN_ARRIBA;
}

void Personaje::setGunSprites() {
    ANIMACION_ACTUAL = ANIMACION_DISPARANDO_GUN;
    ANIMACION_ACTUAL_DISPARANDO_ABAJO = ANIMACION_DISPARANDO_GUN_ABAJO;
    ANIMACION_ACTUAL_DISPARANDO_ARRIBA = ANIMACION_DISPARANDO_GUN_ARRIBA;
    armaActual = GUN;
}

void Personaje::setCantidadBalas(int nuevo, int extra, int tipo) {
    if (tipo == armaActual) cantidadBalas += extra;
    else cantidadBalas = nuevo;
}

void Personaje::restarBala() {
    if (armaActual != GUN){
        if (cantidadBalas <= 0) {
            setGunSprites();
            return;
        }
        cantidadBalas--;
    }
}

int Personaje::getCantidadBalas() {
    return cantidadBalas;
}

bool Personaje::getCambioDeArma() {
    return cantidadBalas <= 0;
}

int Personaje::getPuntaje() {
    return puntaje;
}

void Personaje::aumentarPuntos(int aux) {
    puntaje += aux;
}

void Personaje::restarVida(int aux) {
    vida -= aux;
    if (vida == 0) conectado = false;
    cout << "VIDA: " << vida << endl;
}

void Personaje::sumarVida(int aux) {
    if (vida < 5) vida += aux;
    cout << "VIDA: " << vida << endl;
}

bool Personaje::estaVivo() {
    return vida > 0;
}

void Personaje::setQuieto(bool aux){
    quieto = aux;
}

bool Personaje::getQuieto(){
    return quieto;
}

bool Personaje::getColision() {
    return colision;
}

void Personaje::setColision(bool aux) {
    colision = aux;
}

void Personaje::setVida(int aux) {
    vida = aux;
}

void Personaje::setModoInvencible(bool aux) {
    modoInvencible = aux;
}

bool Personaje::getModoInvencible() {
    return modoInvencible;
}

Personaje::~Personaje() {
    delete envolvente;
}