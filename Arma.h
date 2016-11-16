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
    const static int ANIMACION_PARADO = 4;
    const static int ANIMACION_CORRIENDO_TORSO = 12;
    const static int ANIMACION_SALTANDO = 12;
    const static int ANIMACION_DISPARANDO_GUN = 10;
    const static int ANIMACION_DISPARANDO_SHOTGUN = 4;
    int ANIMACION_ACTUAL = 10; //empieza siendo 10 por la gun

    Textura* TEXTURA_ARMA_PARADO;
    SDL_Rect spriteParado[ ANIMACION_PARADO ];

    Textura* TEXTURA_ARMA_CORRIENDO;
    SDL_Rect spriteCorriendo[ANIMACION_CORRIENDO_TORSO];

    Textura* TEXTURA_ARMA_SALTANDO;
    SDL_Rect spriteSaltando[ ANIMACION_SALTANDO ];

    Textura* TEXTURA_ARMA_DISPARANDO;
    SDL_Rect spriteDisparando[ ANIMACION_DISPARANDO_GUN];

    SDL_Rect* currentClip;

public:
    Arma(SDL_Renderer* render);

    void renderParado(int x, int y, int frame, SDL_RendererFlip flip);

    void renderCorriendo(int x, int y, int frame, SDL_RendererFlip flip);

    void renderSaltando(int x, int y, int frame, SDL_RendererFlip flip);

    void renderDisparando(int x, int y, int frame, SDL_RendererFlip flip);

    bool cargarImagen(const char *const pathParado, const char *const pathCorriendo, const char *const pathSaltando, const char *const pathDisparando);

    void liberar();

    void ponerShotgun();

    void ponerGun();

    void ponerTexturaGris();

    void sacarTexturaGris();
};


#endif //METALZCURRA_ARMA_H
