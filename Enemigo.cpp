//
// Created by nestor on 15/11/16.
//

#include "ObjectManager.h"
#include "Enemigo.h"

Enemigo::Enemigo(int x, int y, int delta){
    posx = x;
    posy = y;
    this->delta = delta;
    ancho = 60;
    alto = 80;
    existe = false;
    muerto = false;
    disparando = false;
    velocidad = 7;
    cantidadPasos = 40;
    alan = 0;

    start = high_resolution_clock::now();
}

bool Enemigo::mover() {
    if (posx < 0) existe = false;
    //if (velocidad == -7 && posx > 800) velocidad = 7;
    if (cantidadPasos > 0){
        posx -= velocidad;
        cantidadPasos--;
        setSprite();
        return true;
    }
    else if (!disparando){
        disparar();
        if (verificarAlan()){
            setSprite();
            alan = 0;
            return true;
        }
        else {
            alan++;
            return false;
        }
    }
    else {
        if (disparando){
            setSprite();
            alan++;
            return true;
        }
        else {
            alan++;
            return false;
        }
    }
}

void Enemigo::morir(){
    muerto = true;
    existe = false;
}

int Enemigo::getAncho(){
    return ancho;
}

int Enemigo::getAlto(){
    return alto;
}

int Enemigo::getPosx(){
    return posx;
}

int Enemigo::getPosy(){
    return posy;
}

int Enemigo::getId() {
    return id;
}

void Enemigo::setId(int id) {
    this->id = id;
}

void Enemigo::setPosx(int x) {
    posx = x;
}

int Enemigo::estaMuerto() {
    return muerto;
}

int Enemigo::getExiste() {
    return existe;
}

void Enemigo::crear() {
    ObjectManager* objectManager = ObjectManager::getInstance();
    if ( posx < 0 ) morir();
    else if ((*(objectManager->getPosX()) + 800) > posx ) existe = true;
}

bool Enemigo::disparar() {
    microseconds intervalo(2000000);	// 2s
    actual = high_resolution_clock::now();

    auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
    auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

    if (elapsed_ms.count() >= intervalo.count()) {
        ObjectManager* objectManager = ObjectManager::getInstance();
        objectManager->inicializarBalaEnemiga(posx, posy + 20);
        disparando = true;
        start = chrono::system_clock::now();
     }
}

void Enemigo::animacionMuerte1(){
    ++frameMuerte1;
    if( frameMuerte1 >= ANIMACION_MUERTE1 ) frameMuerte1 = 0;
}

void Enemigo::animacionMuerte2(){
    ++frameMuerte2;
    if( frameMuerte2 >= ANIMACION_MUERTE2 ) frameMuerte2 = 0;
}

void Enemigo::animacionMirando(){
    ++frameMirando;
    if( frameMirando >= ANIMACION_MIRANDO ) frameMirando = 0;
}

void Enemigo::animacionCorriendo(){
    ++frameCorriendo;
    if( frameCorriendo >= ANIMACION_CORRIENDO ) frameCorriendo = 0;
}

void Enemigo::animacionDisparando(){
    ++frameDisparando;
    if( frameDisparando >= ANIMACION_DISPARANDO ) {
        frameDisparando = 0;
        disparando = false;
    }
}

void Enemigo::animacionQuieto(){
    ++frameQuieto;
    if( frameQuieto >= ANIMACION_QUIETO ) frameQuieto = 0;
}

void Enemigo::setSprite(){
    if (disparando) animacionDisparando();
    else if (cantidadPasos > 0) animacionCorriendo();
    else animacionQuieto();
}

int Enemigo::getSprite(){
    if (disparando) return frameDisparando;
    else if (cantidadPasos > 0) return frameCorriendo;
    else return frameQuieto;
}

bool Enemigo::isDisparando(){
    return disparando;
}

int Enemigo::getCantidadPasos(){
    return cantidadPasos;
}

bool Enemigo::verificarAlan(){
    return (alan > 2);
}