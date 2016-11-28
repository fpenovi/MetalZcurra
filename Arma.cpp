//
// Created by nestor on 14/11/16.
//

#include "Arma.h"
#include "VistaPuntajes.h"

Arma::Arma(SDL_Renderer* render, int id, int modoJuego){
    TEXTURA_ARMA_PARADO = new Textura(render);
    TEXTURA_ARMA_CORRIENDO = new Textura(render);
    TEXTURA_ARMA_SALTANDO = new Textura(render);
    TEXTURA_ARMA_DISPARANDO = new Textura(render);
    TEXTURA_ARMA_APUNTA_ABAJO = new Textura(render);
    TEXTURA_ARMA_APUNTA_ARRIBA = new Textura(render);
    TEXTURA_ARMA_DISPARANDO_ABAJO = new Textura(render);
    TEXTURA_ARMA_DISPARANDO_ARRIBA = new Textura(render);
    TEXTURA_MURIENDO = new Textura(render);

    gameMode = modoJuego;
    idPj = to_string(id);
    string pathComun = "imag/sprites/";
    string pathParado;
    string pathCorriendo;
    string pathSaltando;
    string pathDisparando;
    string pathApuntarAbajo;
    string pathApuntarArriba;
    string pathDisparoAbajo;
    string pathDisparoArriba;
    string pathMuriendo;

    if (modoJuego == VistaPuntajes::MODO_GRUPAL){
        string equipo = to_string((id % VistaPuntajes::CANT_EQUIPOS) + 1);

        pathParado = pathComun + "equipo" + equipo + "/player" + idPj + "/gunStill.png";
        pathCorriendo = pathComun + "equipo" + equipo + "/player" + idPj + "/gunRun.png";
        pathSaltando = pathComun + "equipo" + equipo + "/player" + idPj + "/gunJump.png";
        pathDisparando = pathComun + "equipo" + equipo + "/player" + idPj + "/gunShoot.png";
        pathApuntarAbajo = pathComun + "equipo" + equipo + "/player" + idPj + "/gunPointDown.png";
        pathApuntarArriba = pathComun + "equipo" + equipo + "/player" + idPj + "/gunPointUp.png";
        pathDisparoAbajo = pathComun + "equipo" + equipo + "/player" + idPj + "/gunShootDown.png";
        pathDisparoArriba = pathComun + "equipo" + equipo + "/player" + idPj + "/gunShootUp.png";
        pathMuriendo = pathComun + "equipo" + equipo + "/player" + idPj + "/death.png";
    }
    else {
        pathParado = pathComun + "player" + idPj + "/gunStill.png";
        pathCorriendo = pathComun + "player" + idPj + "/gunRun.png";
        pathSaltando = pathComun + "player" + idPj + "/gunJump.png";
        pathDisparando = pathComun + "player" + idPj + "/gunShoot.png";
        pathApuntarAbajo = pathComun + "player" + idPj + "/gunPointDown.png";
        pathApuntarArriba = pathComun + "player" + idPj + "/gunPointUp.png";
        pathDisparoAbajo = pathComun + "player" + idPj + "/gunShootDown.png";
        pathDisparoArriba = pathComun + "player" + idPj + "/gunShootUp.png";
        pathMuriendo = pathComun + "player" + idPj + "/death.png";
    }

    cargarImagen(pathParado.c_str(),
                 pathCorriendo.c_str(),
                 pathSaltando.c_str(),
                 pathDisparando.c_str(),
                 pathApuntarAbajo.c_str(),
                 pathApuntarArriba.c_str(),
                 pathDisparoAbajo.c_str(),
                 pathDisparoArriba.c_str(),
                 pathMuriendo.c_str());
}

void Arma::renderParado(int x, int y, int frame, SDL_RendererFlip flip){
    if (arriba) {
        currentClip = &spriteApuntaArriba[frame];
        if (shotgun) TEXTURA_ARMA_APUNTA_ARRIBA->render(x,y-50,currentClip,0,NULL,flip );
        else TEXTURA_ARMA_APUNTA_ARRIBA->render(x,y-5,currentClip,0,NULL,flip );
    }
    else if (abajo) {
        currentClip = &spriteApuntaAbajo[frame];
        if (shotgun) TEXTURA_ARMA_APUNTA_ABAJO->render(x,y+5,currentClip,0,NULL,flip );
        else TEXTURA_ARMA_APUNTA_ABAJO->render(x,y+10,currentClip,0,NULL,flip );
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

void Arma::renderMuriendo(int x, int y, int frame, SDL_RendererFlip flip) {

    currentClip = &spriteMuriendo[ frame ];
    TEXTURA_MURIENDO->render(x, y, currentClip, 0, NULL, flip);
}

bool Arma::cargarImagen(const char *const pathParado, const char *const pathCorriendo, const char *const pathSaltando, const char *const pathDisparando, const char *const pathApuntarAbajo, const char *const pathApuntarArriba, const char *const pathDisparoAbajo, const char *const pathDisparoArriba, const char *const pathMuriendo){
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
    if( !TEXTURA_MURIENDO->cargarImagen( pathMuriendo) )
    {
        printf( "Fallo sprite muriendo\n" );
        success = false;
    }
    else{

        for (i = 0; i < ANIMACION_MURIENDO; i++){
            spriteMuriendo[ i ].x = i*149;
            spriteMuriendo[ i ].y = 0;
            spriteMuriendo[ i ].w = 149;
            spriteMuriendo[ i ].h = 86;
        }
    }

    return success;
}

void Arma::liberar(){
    TEXTURA_ARMA_DISPARANDO->free();
    TEXTURA_ARMA_CORRIENDO->free();
    TEXTURA_ARMA_SALTANDO->free();
    TEXTURA_ARMA_PARADO->free();
    TEXTURA_ARMA_APUNTA_ABAJO->free();
    TEXTURA_ARMA_APUNTA_ARRIBA->free();
    TEXTURA_ARMA_DISPARANDO_ABAJO->free();
    TEXTURA_ARMA_DISPARANDO_ARRIBA->free();
    TEXTURA_MURIENDO->free();
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
    delete TEXTURA_MURIENDO;
}

void Arma::ponerShotgun(){
    liberar();
    ANIMACION_ACTUAL = ANIMACION_DISPARANDO_SHOTGUN;
    ANIMACION_ACTUAL_DISPARANDO_ABAJO=ANIMACION_DISPARANDO_SHOTGUN_ABAJO;
    ANIMACION_ACTUAL_DISPARANDO_ARRIBA=ANIMACION_DISPARANDO_SHOTGUN_ARRIBA;

    string pathComun = "imag/sprites/";
    string pathParado;
    string pathCorriendo;
    string pathSaltando;
    string pathDisparando;
    string pathApuntarAbajo;
    string pathApuntarArriba;
    string pathDisparoAbajo;
    string pathDisparoArriba;
    string pathMuriendo;

    if (gameMode == VistaPuntajes::MODO_GRUPAL){
        string equipo = to_string((stoi(idPj) % VistaPuntajes::CANT_EQUIPOS) + 1);

        pathParado = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunStill.png";
        pathCorriendo = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunRun.png";
        pathSaltando = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunJump.png";
        pathDisparando = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunShoot.png";
        pathApuntarAbajo = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunPointDown.png";
        pathApuntarArriba = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunPointUp.png";
        pathDisparoAbajo = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunShootDown.png";
        pathDisparoArriba = pathComun + "equipo" + equipo + "/player" + idPj + "/shotgunShootUp.png";
        pathMuriendo = pathComun + "equipo" + equipo + "/player" + idPj + "/death.png";
    }
    else {
        pathParado = pathComun + "player" + idPj + "/shotgunStill.png";
        pathCorriendo = pathComun + "player" + idPj + "/shotgunRun.png";
        pathSaltando = pathComun + "player" + idPj + "/shotgunJump.png";
        pathDisparando = pathComun + "player" + idPj + "/shotgunShoot.png";
        pathApuntarAbajo = pathComun + "player" + idPj + "/shotgunPointDown.png";
        pathApuntarArriba = pathComun + "player" + idPj + "/shotgunPointUp.png";
        pathDisparoAbajo = pathComun + "player" + idPj + "/shotgunShootDown.png";
        pathDisparoArriba = pathComun + "player" + idPj + "/shotgunShootUp.png";
        pathMuriendo = pathComun + "player" + idPj + "/death.png";
    }

    cargarImagen(pathParado.c_str(),
                 pathCorriendo.c_str(),
                 pathSaltando.c_str(),
                 pathDisparando.c_str(),
                 pathApuntarAbajo.c_str(),
                 pathApuntarArriba.c_str(),
                 pathDisparoAbajo.c_str(),
                 pathDisparoArriba.c_str(),
                 pathMuriendo.c_str());

    shotgun = true;
}

void Arma::ponerGun(){
    if (shotgun) {

        liberar();
        ANIMACION_ACTUAL = ANIMACION_DISPARANDO_GUN;
        ANIMACION_ACTUAL_DISPARANDO_ABAJO = ANIMACION_DISPARANDO_GUN_ABAJO;
        ANIMACION_ACTUAL_DISPARANDO_ARRIBA = ANIMACION_DISPARANDO_GUN_ARRIBA;

        string pathComun = "imag/sprites/";
        string pathParado;
        string pathCorriendo;
        string pathSaltando;
        string pathDisparando;
        string pathApuntarAbajo;
        string pathApuntarArriba;
        string pathDisparoAbajo;
        string pathDisparoArriba;
        string pathMuriendo;

        if (gameMode == VistaPuntajes::MODO_GRUPAL){
            string equipo = to_string((stoi(idPj) % VistaPuntajes::CANT_EQUIPOS) + 1);

            pathParado = pathComun + "equipo" + equipo + "/player" + idPj + "/gunStill.png";
            pathCorriendo = pathComun + "equipo" + equipo + "/player" + idPj + "/gunRun.png";
            pathSaltando = pathComun + "equipo" + equipo + "/player" + idPj + "/gunJump.png";
            pathDisparando = pathComun + "equipo" + equipo + "/player" + idPj + "/gunShoot.png";
            pathApuntarAbajo = pathComun + "equipo" + equipo + "/player" + idPj + "/gunPointDown.png";
            pathApuntarArriba = pathComun + "equipo" + equipo + "/player" + idPj + "/gunPointUp.png";
            pathDisparoAbajo = pathComun + "equipo" + equipo + "/player" + idPj + "/gunShootDown.png";
            pathDisparoArriba = pathComun + "equipo" + equipo + "/player" + idPj + "/gunShootUp.png";
            pathMuriendo = pathComun + "equipo" + equipo + "/player" + idPj + "/death.png";
        }
        else {
            pathParado = pathComun + "player" + idPj + "/gunStill.png";
            pathCorriendo = pathComun + "player" + idPj + "/gunRun.png";
            pathSaltando = pathComun + "player" + idPj + "/gunJump.png";
            pathDisparando = pathComun + "player" + idPj + "/gunShoot.png";
            pathApuntarAbajo = pathComun + "player" + idPj + "/gunPointDown.png";
            pathApuntarArriba = pathComun + "player" + idPj + "/gunPointUp.png";
            pathDisparoAbajo = pathComun + "player" + idPj + "/gunShootDown.png";
            pathDisparoArriba = pathComun + "player" + idPj + "/gunShootUp.png";
            pathMuriendo = pathComun + "player" + idPj + "/death.png";
        }

        cargarImagen(pathParado.c_str(),
                     pathCorriendo.c_str(),
                     pathSaltando.c_str(),
                     pathDisparando.c_str(),
                     pathApuntarAbajo.c_str(),
                     pathApuntarArriba.c_str(),
                     pathDisparoAbajo.c_str(),
                     pathDisparoArriba.c_str(),
                     pathMuriendo.c_str());

        shotgun = false;
    }
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
    TEXTURA_MURIENDO->setColor(128,128,128);
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
    TEXTURA_MURIENDO->setColor(255,255,255);
}
void Arma::apuntarAbajo(bool a){
    abajo = a;
}

void Arma::apuntarArriba(bool a){
    arriba = a;
}

void Arma::titilar() {
    TEXTURA_ARMA_CORRIENDO->setAlpha(80);
    TEXTURA_ARMA_DISPARANDO->setAlpha(80);
    TEXTURA_ARMA_PARADO->setAlpha(80);
    TEXTURA_ARMA_SALTANDO->setAlpha(80);
    TEXTURA_ARMA_APUNTA_ABAJO->setAlpha(80);
    TEXTURA_ARMA_APUNTA_ARRIBA->setAlpha(80);
    TEXTURA_ARMA_DISPARANDO_ABAJO->setAlpha(80);
    TEXTURA_ARMA_DISPARANDO_ARRIBA->setAlpha(80);
    TEXTURA_MURIENDO->setAlpha(80);
}

void Arma::noTitilar() {
    TEXTURA_ARMA_CORRIENDO->setAlpha(255);
    TEXTURA_ARMA_DISPARANDO->setAlpha(255);
    TEXTURA_ARMA_PARADO->setAlpha(255);
    TEXTURA_ARMA_SALTANDO->setAlpha(255);
    TEXTURA_ARMA_APUNTA_ABAJO->setAlpha(255);
    TEXTURA_ARMA_APUNTA_ARRIBA->setAlpha(255);
    TEXTURA_ARMA_DISPARANDO_ABAJO->setAlpha(255);
    TEXTURA_ARMA_DISPARANDO_ARRIBA->setAlpha(255);
    TEXTURA_MURIENDO->setAlpha(255);
}
