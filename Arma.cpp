//
// Created by nestor on 14/11/16.
//

#include "Arma.h"

Arma::Arma(SDL_Renderer* render){
    TEXTURA_ARMA_PARADO = new Textura(render);
    TEXTURA_ARMA_CORRIENDO = new Textura(render);
    TEXTURA_ARMA_SALTANDO = new Textura(render);
    TEXTURA_ARMA_DISPARANDO = new Textura(render);
    TEXTURA_ARMA_APUNTA_ABAJO = new Textura(render);
    TEXTURA_ARMA_APUNTA_ARRIBA = new Textura(render);
    TEXTURA_ARMA_DISPARANDO_ABAJO = new Textura(render);
    TEXTURA_ARMA_DISPARANDO_ARRIBA = new Textura(render);

    cargarImagen("imag/sprites/player2/gunStill.png",
                 "imag/sprites/player2/gunRun.png",
                 "imag/sprites/player2/gunJump.png",
                 "imag/sprites/player2/gunShoot.png",
                 "imag/sprites/player2/gunPointDown.png",
                 "imag/sprites/player2/gunPointUp.png",
                 "imag/sprites/player2/gunShootDown.png",
                 "imag/sprites/player2/gunShootUp.png");
}

void Arma::renderParado(int x, int y, int frame, SDL_RendererFlip flip){
    if (arriba) {
        currentClip = &spriteApuntaArriba[frame];
        TEXTURA_ARMA_APUNTA_ARRIBA->render(x,y-5,currentClip,0,NULL,flip );
    }
    else if (abajo) {
        currentClip = &spriteApuntaAbajo[frame];
        TEXTURA_ARMA_APUNTA_ABAJO->render(x,y+10,currentClip,0,NULL,flip );
    }
    else{
        currentClip = &spriteParado[frame];
        TEXTURA_ARMA_PARADO->render(x,y,currentClip,0,NULL,flip );
    }
}

void Arma::renderCorriendo(int x, int y, int frame, SDL_RendererFlip flip){

    // CORRECCION DE FRAME CON RESPECTO A LAS PIERNAS
    if (frame > (ANIMACION_CORRIENDO_TORSO - 1))
        frame = frame % (ANIMACION_CORRIENDO_TORSO - 1);

    currentClip = &spriteCorriendo[frame];
    TEXTURA_ARMA_CORRIENDO->render(x,y,currentClip,0,NULL,flip );
}

void Arma::renderSaltando(int x, int y, int frame, SDL_RendererFlip flip){
    currentClip = &spriteSaltando[frame];
    TEXTURA_ARMA_SALTANDO->render(x,y,currentClip,0,NULL,flip );
}

void Arma::renderDisparando(int x, int y, int frame, SDL_RendererFlip flip){
    if (arriba)
        renderDispararArriba(x, y, frame, flip);

    else if (abajo)
        renderDispararAbajo(x, y, frame, flip);

    else {
        SDL_Rect* currentClip = &spriteDisparando[ frame ];
        TEXTURA_ARMA_DISPARANDO->render(x,y,currentClip,0,NULL,flip );
    }
}

void Arma::renderDispararArriba(int x, int y, int frame, SDL_RendererFlip flip){
    //LE TENGO QUE RESTAR A LA Y, NOSE PORQUE, SUPONGO QUE ALGO DE LA IMAGEN

    SDL_Rect* currentClip = &spriteDisparandoArriba[ frame ];
    TEXTURA_ARMA_DISPARANDO_ARRIBA->render(x,y-100,currentClip,0,NULL,flip );
}

void Arma::renderDispararAbajo(int x, int y, int frame, SDL_RendererFlip flip){

    SDL_Rect* currentClip = &spriteDisparandoAbajo[ frame ];
    TEXTURA_ARMA_DISPARANDO_ABAJO->render(x,y,currentClip,0,NULL,flip );
}

bool Arma::cargarImagen(const char *const pathParado, const char *const pathCorriendo, const char *const pathSaltando, const char *const pathDisparando, const char *const pathApuntarAbajo, const char *const pathApuntarArriba, const char *const pathDisparoAbajo, const char *const pathDisparoArriba){
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
        for (i = 0; i < ANIMACION_CORRIENDO_TORSO; i++){
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

    if( !TEXTURA_ARMA_APUNTA_ABAJO->cargarImagen( pathApuntarAbajo) )
    {
        printf( "Fallo sprite apunta abajo\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_APUNTANDO;i++){
            spriteApuntaAbajo[ i ].x = i*149;
            spriteApuntaAbajo[ i ].y = 0;
            spriteApuntaAbajo[ i ].w = 149;
            spriteApuntaAbajo[ i ].h = 75;
        }
    }
    if( !TEXTURA_ARMA_APUNTA_ARRIBA->cargarImagen( pathApuntarArriba) )
    {
        printf( "Fallo sprite apunta arriba\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_APUNTANDO;i++){
            spriteApuntaArriba[ i ].x = i*149;
            spriteApuntaArriba[ i ].y = 0;
            spriteApuntaArriba[ i ].w = 149;
            spriteApuntaArriba[ i ].h = 100;
        }
    }
    if( !TEXTURA_ARMA_DISPARANDO_ARRIBA->cargarImagen( pathDisparoArriba) )
    {
        printf( "Fallo sprite disparo arriba\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_ACTUAL_DISPARANDO_ARRIBA;i++){
            spriteDisparandoArriba[ i ].x = i*149;
            spriteDisparandoArriba[ i ].y = 0;
            spriteDisparandoArriba[ i ].w = 149;
            spriteDisparandoArriba[ i ].h = 154;
        }
    }
    if( !TEXTURA_ARMA_DISPARANDO_ABAJO->cargarImagen( pathDisparoAbajo) )
    {
        printf( "Fallo sprite disparo abajo\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_ACTUAL_DISPARANDO_ABAJO;i++){
            spriteDisparandoAbajo[ i ].x = i*149;
            spriteDisparandoAbajo[ i ].y = 0;
            spriteDisparandoAbajo[ i ].w = 149;
            spriteDisparandoAbajo[ i ].h = 135;
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

Arma::~Arma() {
    delete TEXTURA_ARMA_DISPARANDO;
    delete TEXTURA_ARMA_CORRIENDO;
    delete TEXTURA_ARMA_SALTANDO;
    delete TEXTURA_ARMA_PARADO;
    delete TEXTURA_ARMA_APUNTA_ABAJO;
    delete TEXTURA_ARMA_APUNTA_ARRIBA;
    delete TEXTURA_ARMA_DISPARANDO_ABAJO;
    delete TEXTURA_ARMA_DISPARANDO_ARRIBA;
}

void Arma::ponerShotgun(){
    liberar();
    ANIMACION_ACTUAL = ANIMACION_DISPARANDO_SHOTGUN;
    ANIMACION_ACTUAL_DISPARANDO_ABAJO=ANIMACION_DISPARANDO_SHOTGUN_ABAJO;
    ANIMACION_ACTUAL_DISPARANDO_ARRIBA=ANIMACION_DISPARANDO_SHOTGUN_ARRIBA;
    cargarImagen("imag/sprites/player2/shotgunStill.png",
                 "imag/sprites/player2/shotgunRun.png",
                 "imag/sprites/player2/shotgunJump.png",
                 "imag/sprites/player2/shotgunShoot.png",
                 "imag/sprites/player2/shotgunPointDown.png",
                 "imag/sprites/player2/shotgunPointUp.png",
                 "imag/sprites/player2/shotgunShootDown.png",
                 "imag/sprites/player2/shotgunShootUp.png");
}

void Arma::ponerGun(){
    liberar();
    ANIMACION_ACTUAL = ANIMACION_DISPARANDO_GUN;
    ANIMACION_ACTUAL_DISPARANDO_ABAJO=ANIMACION_DISPARANDO_GUN_ABAJO;
    ANIMACION_ACTUAL_DISPARANDO_ARRIBA=ANIMACION_DISPARANDO_GUN_ARRIBA;
    cargarImagen("imag/sprites/player2/gunStill.png",
                 "imag/sprites/player2/gunRun.png",
                 "imag/sprites/player2/gunJump.png",
                 "imag/sprites/player2/gunShoot.png",
                 "imag/sprites/player2/gunPointDown.png",
                 "imag/sprites/player2/gunPointUp.png",
                 "imag/sprites/player2/gunShootDown.png",
                 "imag/sprites/player2/gunShootUp.png");
}

void Arma::ponerTexturaGris() {
    TEXTURA_ARMA_CORRIENDO->setColor(128,128,128);
    TEXTURA_ARMA_DISPARANDO->setColor(128,128,128);
    TEXTURA_ARMA_PARADO->setColor(128,128,128);
    TEXTURA_ARMA_SALTANDO->setColor(128,128,128);
    TEXTURA_ARMA_APUNTA_ABAJO->setColor(128,128,128);
    TEXTURA_ARMA_APUNTA_ARRIBA->setColor(128,128,128);
    TEXTURA_ARMA_DISPARANDO_ABAJO->setColor(128,128,128);
    TEXTURA_ARMA_DISPARANDO_ARRIBA->setColor(128,128,128);
}


void Arma::sacarTexturaGris() {
    TEXTURA_ARMA_CORRIENDO->setColor(255,255,255);
    TEXTURA_ARMA_DISPARANDO->setColor(255,255,255);
    TEXTURA_ARMA_PARADO->setColor(255,255,255);
    TEXTURA_ARMA_SALTANDO->setColor(255,255,255);
    TEXTURA_ARMA_APUNTA_ABAJO->setColor(255,255,255);
    TEXTURA_ARMA_APUNTA_ARRIBA->setColor(255,255,255);
    TEXTURA_ARMA_DISPARANDO_ABAJO->setColor(255,255,255);
    TEXTURA_ARMA_DISPARANDO_ARRIBA->setColor(255,255,255);
}

void Arma::apuntarAbajo(bool a){
    abajo = a;
}
void Arma::apuntarArriba(bool a){
    arriba = a;
}