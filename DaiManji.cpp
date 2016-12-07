//
// Created by nestor on 30/11/16.
//

#include "DaiManji.h"
#include "ObjectManager.h"

DaiManji::DaiManji(int x, int y, int delta) : Boss(x, y, delta) {
    this->velocidad = 7;
    this->vida = 2000;
    this->id = 2;
    this->envolvente->agregarComponente(new Rectangulo(&this->x, &this->y, 160, 117));
}

void DaiManji::moverIzquierda(){
    x -= velocidad;
    setSprites();
    if (x < 0) {
        derecha = true;
        frameMovimiento = 0;
    }
}

void DaiManji::moverDerecha(){
    x += velocidad;
    setSprites();
    if (x + envolvente->getAncho() > 800) {
        derecha = false;
        frameMovimiento = 0;
    }
}

void DaiManji::mover(){
    if (!estaVivo()) existe = false;
    if (disparando == NODISPARANDO) disparar();
    if (derecha) moverDerecha();
    else moverIzquierda();
}

void DaiManji::setSprites() {
    animacionMover();

    if (puerta==ABRIENDO) animacionAbrirPuerta();
    else if (puerta==CERRANDO) animacionCerrarPuerta();

    if (laser==ABRIENDO) animacionSacarLaser();
    else if (laser==CERRANDO) animacionGuardarLaser();

    if (disparando==PREPARANDOSE) animacionCargarTiro();
    else if((disparando==PREPARADO) || (disparando==DISPARANDO)) animacionDisparo();
}

int DaiManji::getSpriteMovimiento(){
    return frameMovimiento;
}

int DaiManji::getPuerta(){
    return puerta;
}

int DaiManji::getLaser(){
    return laser;
}

int DaiManji::getDisparando(){
    return disparando;
}

void DaiManji::crear() {
    ObjectManager* objectManager = ObjectManager::getInstance();
    if ((*(objectManager->getPosX()) + 800) > envolvente->getX() ) {
        existe = true;
        x = 800;
    }
}

bool DaiManji::disparar() {
    microseconds intervalo(2000000);	// 2s
    actual = high_resolution_clock::now();

    auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
    auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

    if (elapsed_ms.count() >= intervalo.count() && estaVivo()) {
        spriteDisparar();
        ObjectManager* objectManager = ObjectManager::getInstance();
        objectManager->tirarEnemigo(x - 50, y + 117);
        start = chrono::system_clock::now();
    }
}

void DaiManji::spriteDisparar(){
    puerta=ABRIENDO;
    laser=ABRIENDO;
}

void DaiManji::animacionMover() {
    ++frameMovimiento;
    if( frameMovimiento>= ANIMACION_MOVIMIENTO ) {
        frameMovimiento = 0;
    }

}

void DaiManji::animacionAbrirPuerta() {
    ++framePuerta;

    if( framePuerta >= ANIMACION_PUERTA )
    {
        framePuerta--;
        puerta=ABIERTO;
    }

}

void DaiManji::animacionCerrarPuerta() {
    --framePuerta;

    if( framePuerta <= 0 )
    {
        framePuerta=0;
        puerta=CERRADO;
    }

}

void DaiManji::animacionSacarLaser() {
    ++frameLaser;

    if( frameLaser >= ANIMACION_LASER )
    {
        frameLaser--;
        laser=ABIERTO;
        disparando=PREPARANDOSE;
    }
}

void DaiManji::animacionGuardarLaser() {
    --frameLaser;

    if( frameLaser >= ANIMACION_LASER )
    {
        frameLaser=0;
        laser=CERRADO;
    }
}

void DaiManji::animacionCargarTiro() {
    ++frameCarga;

    if( frameCarga >= ANIMACION_CARGA_TIRO )
    {
        frameCarga=0;
        disparando=PREPARADO;
    }
}

void DaiManji::animacionDisparo() {
    ++frameTiro;

    if( frameTiro>= ANIMACION_DISPARO )
    {
        frameTiro=0;
        disparando=NODISPARANDO;
        laser=CERRANDO;
        puerta=CERRANDO;
    }
}

int DaiManji::getSprites() {
    return 0;
}

DaiManji::~DaiManji() {
    // TODO Auto-generated destructor stub
}
