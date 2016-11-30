//
// Created by nestor on 29/11/16.
//

#include "VistaDaiManji.h"

VistaDaiManji::VistaDaiManji(SDL_Renderer *render) {
    derecha = false;
    ancho = 160;
    alto = 117;
    existe = false;

    TEXTURA_CARGA_TIRO = new Textura(render);
    TEXTURA_PUERTA = new Textura(render);
    TEXTURA_SACANDO_LASER = new Textura(render);
    TEXTURA_DISPARO = new Textura(render);
    TEXTURA_MOVIMIENTO = new Textura(render);
}

bool VistaDaiManji::cargarImagen() {
    int i;
    bool success=true;

    if( !TEXTURA_CARGA_TIRO->cargarImagen( "imag/sprites/Dai-Manji/chargeShot.png") )
    {
        printf( "Fallo cargar tiro\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_CARGA_TIRO;i++){
            spriteCargandoTiro[ i ].x = i*170;
            spriteCargandoTiro[ i ].y = 0;
            spriteCargandoTiro[ i ].w = 170;
            spriteCargandoTiro[ i ].h = 58;
        }
    }
    if( !TEXTURA_PUERTA->cargarImagen( "imag/sprites/Dai-Manji/openLaserDoor.png") )
    {
        printf( "Fallo cargar puerta\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_PUERTA;i++){
            spritePuerta[ i ].x = i*170;
            spritePuerta[ i ].y = 0;
            spritePuerta[ i ].w = 170;
            spritePuerta[ i ].h = 11;
        }
    }
    if( !TEXTURA_SACANDO_LASER->cargarImagen( "imag/sprites/Dai-Manji/sacandoLaser.png") )
    {
        printf( "Fallo cargar sacandoLaser\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_LASER;i++){
            spriteSacandoLaser[ i ].x = i*170;
            spriteSacandoLaser[ i ].y = 0;
            spriteSacandoLaser[ i ].w = 170;
            spriteSacandoLaser[ i ].h = 34;
        }
    }
    if( !TEXTURA_DISPARO->cargarImagen( "imag/sprites/Dai-Manji/disparoLaser.png") )
    {
        printf( "Fallo cargar disparo\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_DISPARO;i++){
            spriteDisparo[ i ].x = i*170;
            spriteDisparo[ i ].y = 0;
            spriteDisparo[ i ].w = 170;
            spriteDisparo[ i ].h = 175;
        }
    }
    if( !TEXTURA_MOVIMIENTO->cargarImagen( "imag/sprites/Dai-Manji/movimiento.png") )
    {
        printf( "Fallo cargar movimiento ovni\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_MOVIMIENTO;i++){
            spriteMovimiento[ i ].x = i*170;
            spriteMovimiento[ i ].y = 0;
            spriteMovimiento[ i ].w = 170;
            spriteMovimiento[ i ].h = 117;
        }
    }
    return success;
}


void VistaDaiManji::setFrame(int aux) {
    frame = aux;
}

void VistaDaiManji::setDisparando(bool aux) {
    disparando = aux;
}

void VistaDaiManji::setPosx(int aux) {
    posx = aux;
}

void VistaDaiManji::setPosy(int aux) {
    posy = aux;
}

void VistaDaiManji::setExiste(bool aux) {
    existe = aux;
}

void VistaDaiManji::setDireccion(int posNueva){
    derecha = posx < posNueva;
}

void VistaDaiManji::morir() {
    existe = false;
}

void VistaDaiManji::transparentar(Uint8 alpha) {
    TEXTURA_CARGA_TIRO->setAlpha(alpha);
    TEXTURA_PUERTA->setAlpha(alpha);
    TEXTURA_SACANDO_LASER->setAlpha(alpha);
    TEXTURA_DISPARO->setAlpha(alpha);
    TEXTURA_MOVIMIENTO->setAlpha(alpha);
}

VistaDaiManji::~VistaDaiManji() {
    delete TEXTURA_CARGA_TIRO;
    delete TEXTURA_PUERTA;
    delete TEXTURA_SACANDO_LASER;
    delete TEXTURA_DISPARO;
    delete TEXTURA_MOVIMIENTO;
}

void VistaDaiManji::animacionMover(){
    SDL_Rect* currentClip = &spriteMovimiento[ frame ];
    TEXTURA_MOVIMIENTO->render(posx,posy,currentClip);
}

void VistaDaiManji::animacionAbrirPuerta(){
    SDL_Rect* currentClip = &spritePuerta[ frame % 23 ];
    TEXTURA_PUERTA->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::animacionCerrarPuerta(){
    SDL_Rect* currentClip = &spritePuerta[ frame % 23 ];
    TEXTURA_PUERTA->render(posx,posy+alto-7,currentClip); //7 hardcodeado para qe qede bien
}

void VistaDaiManji::animacionSacarLaser(){
    SDL_Rect* currentClip = &spriteSacandoLaser[ frame % 15 ];
    TEXTURA_SACANDO_LASER->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::renderLaserFuera(){
    SDL_Rect* currentClip = &spriteSacandoLaser[ frame % 15];
    TEXTURA_SACANDO_LASER->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::renderLaserDentro(){
    SDL_Rect* currentClip = &spriteSacandoLaser[0];
    TEXTURA_SACANDO_LASER->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::animacionGuardarLaser(){
    SDL_Rect* currentClip = &spriteSacandoLaser[ frame % 15];
    TEXTURA_SACANDO_LASER->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::animacionCargarTiro(){
    SDL_Rect* currentClip = &spriteCargandoTiro[ frame % 20 ];
    TEXTURA_CARGA_TIRO->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::animacionDisparo(){
    SDL_Rect* currentClip = &spriteDisparo[ frame % 20];
    TEXTURA_DISPARO->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::renderPuertaCerrada(){
    SDL_Rect* currentClip = &spritePuerta[0];    //ESTE ES PUERTA CERRADA
    TEXTURA_PUERTA->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::renderPuertaAbierta(){
    SDL_Rect* currentClip = &spritePuerta[ANIMACION_PUERTA-1]; //ESTE ES PUERTA ABIERTA
    TEXTURA_PUERTA->render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
}

void VistaDaiManji::render(){
    if (existe) {
        animacionMover();

        if (puerta == ABRIENDO) animacionAbrirPuerta();
        else if (puerta == ABIERTO) renderPuertaAbierta();
        else if (puerta == CERRANDO) animacionCerrarPuerta();
        else if (puerta == CERRADO) renderPuertaCerrada();

        if (laser == ABRIENDO) animacionSacarLaser();
        else if (laser == ABIERTO) renderLaserFuera();
        else if (laser == CERRANDO) animacionGuardarLaser();
        else if (laser == CERRADO) renderLaserDentro();

        if (disparando == PREPARANDOSE) animacionCargarTiro();
        else if ((disparando == PREPARADO) || (disparando == DISPARANDO)) animacionDisparo();
    }
}

void VistaDaiManji::setPuerta(int aux){
    puerta = aux;
}

void VistaDaiManji::setLaser(int aux){
    laser = aux;
}

void VistaDaiManji::setDisparando(int aux) {
    disparando = aux;
}