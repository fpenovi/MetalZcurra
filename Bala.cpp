//
// Created by nestor on 06/11/16.
//

#include "Bala.h"
#include "ObjectManager.h"

Bala::Bala(){
    velocidad = 30;
    existe = false;
    ancho = 0;
    alto = 0;

    derecha = false;
    izquierda = false;
    arriba = false;
    abajo = false;

}

void Bala::mover(){
    if (posx > 800 || posx < 0 || posy < 0 || posy > 520) {
        desaparecer();
        return;
    }

    if (derecha) posx += velocidad;
    else if (izquierda) posx -= velocidad;

    if (arriba) posy -= velocidad;
    else if (abajo) posy += velocidad;
}

bool Bala::existeBala(){
    return existe;
}

void Bala::setID(int nuevoID){
    id = nuevoID;
}

void Bala::crear(int idEmisor, int x, int y, Direccion* direccion, int ancho, int alto){
    existe = true;
    posx = x;
    posy = y;
    idDuenio = idEmisor;
    this->ancho = ancho;
    this->alto = alto;

    envolvente = new Envolvente();
    envolvente->agregarComponente(new Rectangulo(&posx, &posy, ancho, alto));

    setDireccion(direccion->isDerecha(), direccion->isIzquierda(), direccion->isArriba(), direccion->isAbajo());
}

void Bala::crearBalaEnemiga(int x, int y, bool derecha, int ancho, int alto) {
    existe = true;
    posx = x;
    posy = y;

    envolvente = new Envolvente();
    envolvente->agregarComponente(new Rectangulo(&posx, &posy, ancho, alto));

    setDireccion(derecha, !derecha, false, false);
}

int Bala::getPosx(){
    return posx;
}

int Bala::getPosy(){
    return posy;
}

int Bala::getAncho(){
    return ancho;
}

int Bala::getAlto(){
    return alto;
}

void Bala::desaparecer(){
    existe = false;
    idDuenio = 0;
    posy = 0;
    posx = 0;
}

int Bala::getId() {
    return id;
}

void Bala::setIdDuenio(int id) {
    this->idDuenio = id;
}

int Bala::getIdDuenio() {
    return idDuenio;
}

void Bala::setDireccion(bool der, bool izq, bool arr, bool abj) {
    this->derecha = der;
    this->izquierda = izq;
    this->arriba = arr;
    this->abajo = abj;
}

int Bala::getDerecha() {
    return derecha;
}

int Bala::getArriba() {
    return arriba;
}

int Bala::getAbajo() {
    return abajo;
}

int Bala::getIzquierda() {
    return izquierda;
}

void Bala::handleColision(){
    ObjectManager* objectManager = ObjectManager::getInstance();

    if (id <= 50 || id > 100){
        unordered_map<int, Enemigo*>* enemigosHash = objectManager->getEnemigosHash();

        for (auto kv : *enemigosHash){
            if (kv.second->getExiste() && !kv.second->estaMuerto()) {
                if ((kv.second->getEnvolvente())->hayColision(envolvente)) {
                    kv.second->morir();
                    if (!shotgun) desaparecer();
                }
            }
        }
    }

}

void Bala::setShotgun(bool aux) {
    shotgun = aux;
}

bool Bala::isShotgun() {
    return shotgun;
}

void Bala::moverShotgun() {
    ++frameShotgun;
    if( frameShotgun >= ANIMACION_BALA_SHOTGUN ) {
        frameShotgun = 0;
        desaparecer();
    }
}

int Bala::getFrameShotgun() {
    return frameShotgun;
}

Bala::~Bala() {
    if (envolvente != NULL) delete envolvente;
}
