//
// Created by juancho on 02/10/16.
//
#include "Textura.h"

#ifndef METALZCURRA_VISTAMARCO_H
#define METALZCURRA_VISTAMARCO_H

class VistaMarco {

private:
    int posx, posy;
    int ancho, alto;
    const static int ANIMACION_PARADO = 3;
    const static int ANIMACION_CORRIENDO = 9;
    const static int ANIMACION_SALTANDO = 10;
    SDL_Rect spriteParado[ANIMACION_PARADO];
    SDL_Rect spriteCorriendo[ANIMACION_CORRIENDO];
    SDL_Rect spriteSaltando[ANIMACION_SALTANDO];

    int frameCorriendo;
    int frameParado;
    bool derecha;
    bool quieto;
    bool saltando;
    bool subiendo;
    bool bajando;
    int frameSaltando;

    int velx;
    int vely;
    Textura *TEXTURA_PERSONAJE_PARADO;
    Textura *TEXTURA_PERSONAJE_SALTANDO;
    Textura *TEXTURA_PERSONAJE_CORRIENDO;
    const static int Personaje_VEL = 5;
    const static int Personaje_VEL_Y = 4;
    SDL_Renderer *renderizador;
    int id;
    bool seMovio;

public:
    VistaMarco(SDL_Renderer *renderizador2);

    void handleEvent(SDL_Event &e);

    bool estaQuieto();

    bool mover();

    void render(bool seMovio, int camx, int camy);

    void animacionParado(int camx, int camy);

    void animacionCorrer(int camx, int camy);

    int animacionSaltando(int camx, int camy);

    bool cargarImagen();

    void liberarTextura();

    bool estaSaltando();

    int getX();

    int getY();

    int getAncho();

    int getAlto();

    int getId();

    bool getSeMovio();

    void setPosx(int posx);

    void setPosy(int posy);

    void setQuieto(bool quieto);

    void setDerecha(bool derecha);

    void setId(int id);

    void setSeMovio(bool state);
};


#endif //METALZCURRA_VISTAMARCO_H
