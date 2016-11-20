//
// Created by nestor on 14/11/16.
//

#ifndef METALZCURRA_ARMA_H
#define METALZCURRA_ARMA_H

#include <SDL2/SDL_rect.h>
#include "Textura.h"

class Arma
{
protected:
    string idPj;

    const static int ANIMACION_PARADO = 4;
    const static int ANIMACION_CORRIENDO_TORSO = 12;
    const static int ANIMACION_SALTANDO = 12;
    const static int ANIMACION_DISPARANDO_GUN = 10;
    const static int ANIMACION_DISPARANDO_SHOTGUN = 4;
    const static int ANIMACION_APUNTANDO = 4;
    const static int ANIMACION_DISPARANDO_GUN_ABAJO = 7;
    const static int ANIMACION_DISPARANDO_GUN_ARRIBA = 10;
    const static int ANIMACION_DISPARANDO_SHOTGUN_ABAJO = 4;
    const static int ANIMACION_DISPARANDO_SHOTGUN_ARRIBA = 4;

    int ANIMACION_ACTUAL = 10; //empieza siendo 10 por la gun
    int ANIMACION_ACTUAL_DISPARANDO_ARRIBA = 10; // empieza siendo 10 por la gun
    int ANIMACION_ACTUAL_DISPARANDO_ABAJO = 7;

    Textura* TEXTURA_ARMA_PARADO;
    SDL_Rect spriteParado[ ANIMACION_PARADO ];

    Textura* TEXTURA_ARMA_CORRIENDO;
    SDL_Rect spriteCorriendo[ANIMACION_CORRIENDO_TORSO];

    Textura* TEXTURA_ARMA_SALTANDO;
    SDL_Rect spriteSaltando[ ANIMACION_SALTANDO ];

    Textura* TEXTURA_ARMA_DISPARANDO;
    SDL_Rect spriteDisparando[ ANIMACION_DISPARANDO_GUN];

    Textura* TEXTURA_ARMA_APUNTA_ABAJO;
    SDL_Rect spriteApuntaAbajo[ ANIMACION_APUNTANDO];

    Textura* TEXTURA_ARMA_APUNTA_ARRIBA;
    SDL_Rect spriteApuntaArriba[ ANIMACION_APUNTANDO];

    Textura* TEXTURA_ARMA_DISPARANDO_ABAJO;
    SDL_Rect spriteDisparandoAbajo[ ANIMACION_DISPARANDO_GUN_ABAJO];

    Textura* TEXTURA_ARMA_DISPARANDO_ARRIBA;
    SDL_Rect spriteDisparandoArriba[ ANIMACION_DISPARANDO_GUN_ARRIBA];

    bool arriba = false;
    bool abajo = false;

    SDL_Rect* currentClip;

public:
    Arma(SDL_Renderer* render, int id);

    ~Arma();

    void renderParado(int x, int y, int frame, SDL_RendererFlip flip);

    void renderCorriendo(int x, int y, int frame, SDL_RendererFlip flip);

    void renderSaltando(int x, int y, int frame, SDL_RendererFlip flip);

    void renderDisparando(int x, int y, int frame, SDL_RendererFlip flip);

    void renderDispararArriba(int x, int y, int frame, SDL_RendererFlip flip);

    void renderDispararAbajo(int x, int y, int frame, SDL_RendererFlip flip);

    bool cargarImagen(const char *const pathParado, const char *const pathCorriendo, const char *const pathSaltando, const char *const pathDisparando, const char *const pathApuntarAbajo, const char *const pathApuntarArriba, const char *const pathDisparoAbajo, const char *const pathDisparoArriba);

    void liberar();

    void ponerShotgun();

    void ponerGun();

    void ponerTexturaGris();

    void sacarTexturaGris();

    void apuntarArriba(bool a);

    void apuntarAbajo(bool a);
};


#endif //METALZCURRA_ARMA_H
