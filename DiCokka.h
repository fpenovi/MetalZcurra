//
// Created by nestor on 30/11/16.
//

#ifndef METALZCURRA_DICOKKA_H
#define METALZCURRA_DICOKKA_H

#define NOGIRO 0
#define IZQUIERDA 1
#define DERECHA 2

#define NODISPARANDO 0
#define DISPARANDO 3
#define TERMINADO 4

#include <chrono>
#include "Boss.h"

using namespace chrono;

class DiCokka : public Boss
{
private:
    int posy;
    int alto;
    int ancho;

    bool derecha;
    int girando;
    int disparando;

    const static int ANIMACION_MUERTE = 16;
    const static int ANIMACION_MOVER = 8;
    const static int ANIMACION_SALIDA_TIRO = 9;
    const static int ANIMACION_DISPARAR = 7;
    const static int ANIMACION_GIRAR = 11;

    int frameMuerte=0;
    int frameMover=0;
    int frameSalidaTiro=0;
    int frameDisparar=0;
    int frameGirar=0;

    time_point<high_resolution_clock> start;
    time_point<high_resolution_clock> actual;

public:
    DiCokka(int x, int y, int delta);
    virtual ~DiCokka();

    void mover();

    void setSprites();

    int getSprites();

    void crear();

    bool disparar();

    void animacionMover();

    void animacionMuerte();

    void animacionSalidaTiro();

    void animacionDisparar();

    void animacionGirarADerecha();

    void animacionGirarAIzquierda();

    void animacionGirar();

    void moverDerecha();

    void moverIzquierda();

    void spriteDisparar();

    int getGirando();

    int getDisparando();

    int getSpriteMover();
};

#endif //METALZCURRA_DICOKKA_H
