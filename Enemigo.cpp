//
// Created by nestor on 15/11/16.
//

#include "ObjectManager.h"
#include "Enemigo.h"

Enemigo::Enemigo(){
    posy = 440;
    posx = 600;
    ancho = 60;
    alto = 80;
    existe = false;
    muerto = false;
    velocidad = 7;
    cantidadPasos = 20;

    start = high_resolution_clock::now();
}

bool Enemigo::mover() {
    if (posx < 0) existe = false;
    //if (velocidad == -7 && posx > 800) velocidad = 7;
    if (cantidadPasos > 0){
        posx -= velocidad;
        cantidadPasos--;
        return true;
    }
    else return false;
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
    if ((*(objectManager->getPosX()) + 800) > posx ) existe = true;
    if ( posx < 0 ) existe = false;
}

void Enemigo::disparar() {
    microseconds intervalo(2000000);	// 2s
    actual = high_resolution_clock::now();

    auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
    auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

    if (elapsed_ms.count() >= intervalo.count()) {
        ObjectManager* objectManager = ObjectManager::getInstance();
        objectManager->inicializarBalaEnemiga(posx, posy + 20);
        start = chrono::system_clock::now();
    }

}