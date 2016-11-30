//
// Created by nestor on 30/11/16.
//

#ifndef METALZCURRA_DAIMANJI_H
#define METALZCURRA_DAIMANJI_H

#define CERRADO 0
#define CERRANDO 1
#define ABRIENDO 2
#define ABIERTO 3

#define NODISPARANDO 0
#define PREPARANDOSE 1
#define PREPARADO 2
#define DISPARANDO 3
#define TERMINADO 4

#include <chrono>
#include "Boss.h"

using namespace chrono;

class DaiManji: public Boss {

private:
    int puerta = CERRADO;
    int laser = CERRADO;
    int disparando = NODISPARANDO;

    const static int ANIMACION_CARGA_TIRO = 20;
    const static int ANIMACION_PUERTA = 23;
    const static int ANIMACION_LASER = 15;
    const static int ANIMACION_DISPARO = 20;
    const static int ANIMACION_MOVIMIENTO = 24;

    int framePuerta;
    int frameCarga;
    int frameLaser;
    int frameTiro;
    int frameMovimiento;

    bool quieto = false;

    time_point<high_resolution_clock> start;
    time_point<high_resolution_clock> actual;

public:
    DaiManji(int x, int y, int delta);
    virtual ~DaiManji();

    void moverIzquierda();

    void moverDerecha();

    void mover();

    void setSprites();

    int getSprites();

    void crear();

    bool disparar();

    void animacionMover();

    void animacionAbrirPuerta();

    void animacionCerrarPuerta();

    void animacionSacarLaser();

    void animacionGuardarLaser();

    void animacionCargarTiro();

    void animacionDisparo();

    int getSpriteMovimiento();

    int getPuerta();

    int getLaser();

    int getDisparando();

    void spriteDisparar();
};

#endif //METALZCURRA_DAIMANJI_H
