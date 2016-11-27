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

    danio = 0;
    puntos = 0;
    idDuenio = 0;
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

    if (envolvente != NULL) delete envolvente;
    envolvente = new Envolvente();
    envolvente->agregarComponente(new Rectangulo(&posx, &posy, ancho, alto));

    if (shotgun || rlauncher){
        if (direccion->isDerecha() || direccion->isIzquierda())
            setDireccion(direccion->isDerecha(), direccion->isIzquierda(), false, false);
        else if (direccion->isArriba() || direccion->isAbajo())
            setDireccion(false, false, direccion->isArriba(), direccion->isAbajo());
    }
    else if (!shotgun && !rlauncher){
        setDireccion(direccion->isDerecha(), direccion->isIzquierda(), direccion->isArriba(), direccion->isAbajo());
    }
}

void Bala::crearBalaEnemiga(int x, int y, bool derecha, int ancho, int alto) {
    existe = true;
    posx = x;
    posy = y;

    if (envolvente != NULL) delete envolvente;
    envolvente = new Envolvente();
    envolvente->agregarComponente(new Rectangulo(&posx, &posy, ancho, alto));

    setDireccion(derecha, !derecha, false, false);
}

void Bala::crearBalaBoss(int x, int y, int ancho, int alto){
    existe = true;
    posx = x;
    posy = y;

    if (envolvente != NULL) delete envolvente;
    envolvente = new Envolvente();
    envolvente->agregarComponente(new Rectangulo(&posx, &posy, ancho, alto));

    setDireccion(false, false, false, true);
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
    setDireccion(false, false, false, false);
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

    if (id <= 50 || (id > 100 && id < 251)){
        unordered_map<int, Enemigo*>* enemigosHash = objectManager->getEnemigosHash();

        for (auto kv : *enemigosHash){
            if (kv.second->getExiste() && !kv.second->estaMuerto()) {
                if ((kv.second->getEnvolvente())->hayColision(envolvente)) {
                    kv.second->morir();
                    (objectManager->getObject(idDuenio))->aumentarPuntos(puntos);
                    if (!shotgun) desaparecer();
                }
            }
        }

        Boss* boss = objectManager->getBoss();

        if (boss->getExiste() && boss->estaVivo()){
            if (boss->getEnvolvente()->hayColision(envolvente)){
                boss->restarVida(danio);
                (objectManager->getObject(idDuenio))->aumentarPuntos(puntos);
                if (!shotgun) desaparecer();
            }
        }

    }
    else {
        unordered_map<int, Personaje*>* personajesHash = objectManager->getPersonajesHash();

        for (auto kv : *personajesHash){
            if (kv.second->getConectado() && kv.second->estaVivo()){
                if ((kv.second->getEnvolvente())->hayColision(envolvente)) {
                    kv.second->restarVida(1);
                    objectManager->handleImpacto(kv.second);
                    desaparecer();
                }
            }
        }
    }

}

void Bala::setShotgun(bool aux) {
    shotgun = aux;
}

void Bala::setRlauncher(bool aux) {
    rlauncher = aux;
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

void Bala::setDanio(int aux){
    danio = aux;
}

Bala::~Bala() {
    if (envolvente != NULL) delete envolvente;
}

void Bala::setPuntos(int aux) {
    puntos = aux;
}
