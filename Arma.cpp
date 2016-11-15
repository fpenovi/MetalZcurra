//
// Created by nestor on 14/11/16.
//

#include "Arma.h"

Arma::Arma(SDL_Renderer* render){
    TEXTURA_ARMA_PARADO = new Textura(render);
    TEXTURA_ARMA_CORRIENDO = new Textura(render);
    TEXTURA_ARMA_SALTANDO = new Textura(render);
    TEXTURA_ARMA_DISPARANDO = new Textura(render);

    cargarImagen("imag/sprites/gunStill.png",
                 "imag/sprites/gunRun.png",
                 "imag/sprites/gunJump.png",
                 "imag/sprites/gunShoot.png");
}

void Arma::renderParado(int x, int y, int frame, SDL_RendererFlip flip){
    currentClip = &spriteParado[frame];
    TEXTURA_ARMA_PARADO->render(x,y,currentClip,0,NULL,flip );
}

void Arma::renderCorriendo(int x, int y, int frame, SDL_RendererFlip flip){
    currentClip = &spriteCorriendo[frame];
    TEXTURA_ARMA_CORRIENDO->render(x,y,currentClip,0,NULL,flip );
}

void Arma::renderSaltando(int x, int y, int frame, SDL_RendererFlip flip){
    currentClip = &spriteSaltando[frame];
    TEXTURA_ARMA_SALTANDO->render(x,y,currentClip,0,NULL,flip );
}

void Arma::renderDisparando(int x, int y, int frame, SDL_RendererFlip flip){
    currentClip = &spriteDisparando[frame];
    TEXTURA_ARMA_DISPARANDO->render(x,y,currentClip,0,NULL,flip );
}

bool Arma::cargarImagen(const char *const pathParado, const char *const pathCorriendo, const char *const pathSaltando, const char *const pathDisparando){
    bool success = true;
    int i;

    if( !TEXTURA_ARMA_PARADO->cargarImagen( pathParado) )
    {
        printf( "Fallo sprite arma parado\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_PARADO;i++){
            spriteParado[ i ].x = i*149;
            spriteParado[ i ].y = 0;
            spriteParado[ i ].w = 149;
            spriteParado[ i ].h = 62;
        }
    }

    if( !TEXTURA_ARMA_CORRIENDO->cargarImagen( pathCorriendo) )
    {
        printf( "Fallo sprite arma corriendo\n" );
        success = false;
    }
    else{
        for (i = 0;i<ANIMACION_CORRIENDO;i++){
            spriteCorriendo[ i ].x = i*149;
            spriteCorriendo[ i ].y = 0;
            spriteCorriendo[ i ].w = 149;
            spriteCorriendo[ i ].h = 64;
        }
    }

    if( !TEXTURA_ARMA_SALTANDO->cargarImagen( pathSaltando) )
    {
        printf( "Fallo sprite arma saltando\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_SALTANDO;i++){
            spriteSaltando[ i ].x = i*149;
            spriteSaltando[ i ].y = 0;
            spriteSaltando[ i ].w = 149;
            spriteSaltando[ i ].h = 79;
        }
    }

    if( !TEXTURA_ARMA_DISPARANDO->cargarImagen( pathDisparando) )
    {
        printf( "Fallo sprite disparando\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_ACTUAL;i++){
            spriteDisparando[ i ].x = i*149;
            spriteDisparando[ i ].y = 0;
            spriteDisparando[ i ].w = 149;
            spriteDisparando[ i ].h = 62;
        }
    }

    return success;
}

void Arma::liberar(){
    TEXTURA_ARMA_DISPARANDO->free();
    TEXTURA_ARMA_CORRIENDO->free();
    TEXTURA_ARMA_SALTANDO->free();
    TEXTURA_ARMA_PARADO->free();
}

void Arma::ponerShotgun(){
    liberar();
    ANIMACION_ACTUAL = ANIMACION_DISPARANDO_SHOTGUN;
    cargarImagen("imag/sprites/shotgunStill.png",
                 "imag/sprites/shotgunRun.png",
                 "imag/sprites/shotgunJump.png",
                 "imag/sprites/shotgunShoot.png");
}

void Arma::ponerGun(){
    liberar();
    ANIMACION_ACTUAL = ANIMACION_DISPARANDO_GUN;
    cargarImagen("imag/sprites/gunStill.png",
                 "imag/sprites/gunRun.png",
                 "imag/sprites/gunJump.png",
                 "imag/sprites/gunShoot.png");
}

void Arma::ponerTexturaGris() {
    TEXTURA_ARMA_CORRIENDO->setColor(128,128,128);
    TEXTURA_ARMA_DISPARANDO->setColor(128,128,128);
    TEXTURA_ARMA_PARADO->setColor(128,128,128);
    TEXTURA_ARMA_SALTANDO->setColor(128,128,128);
}


void Arma::sacarTexturaGris() {
    TEXTURA_ARMA_CORRIENDO->setColor(255,255,255);
    TEXTURA_ARMA_DISPARANDO->setColor(255,255,255);
    TEXTURA_ARMA_PARADO->setColor(255,255,255);
    TEXTURA_ARMA_SALTANDO->setColor(255,255,255);
}