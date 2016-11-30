//
// Created by nestor on 30/11/16.
//

#include "VistaDiCokka.h"

VistaDiCokka::VistaDiCokka(SDL_Renderer *render) {
    derecha = false;
    ancho=152;
    alto=122;
    girando=NOGIRO;
    disparando=NODISPARANDO;
    existe = false;

    TEXTURA_MUERTE = new Textura(render);
    TEXTURA_MOVER = new Textura(render);
    TEXTURA_SALIDA_TIRO = new Textura(render);
    TEXTURA_DISPARAR = new Textura(render);
    TEXTURA_GIRAR = new Textura(render);
}

bool VistaDiCokka::cargarImagen() {
    int i;
    bool success=true;

    if( !TEXTURA_MUERTE->cargarImagen( "imag/sprites/Di-Cokka/death.png") )
    {
        printf( "Fallo muerte tanque\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_MUERTE;i++){
            spriteMuerte[ i ].x = i*363;
            spriteMuerte[ i ].y = 0;
            spriteMuerte[ i ].w = 363;
            spriteMuerte[ i ].h = 146;
        }
    }
    if( !TEXTURA_MOVER->cargarImagen( "imag/sprites/Di-Cokka/moving.png") )
    {
        printf( "Fallo mover tanque\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_MOVER;i++){
            spriteMover[ i ].x = i*363;
            spriteMover[ i ].y = 0;
            spriteMover[ i ].w = 363;
            spriteMover[ i ].h = 122;
        }
    }
    if( !TEXTURA_SALIDA_TIRO->cargarImagen( "imag/sprites/Di-Cokka/salidaDelCanon.png") )
    {
        printf( "Fallo salida del tiro tanque\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_SALIDA_TIRO;i++){
            spriteSalidaTiro[ i ].x = i*363;
            spriteSalidaTiro[ i ].y = 0;
            spriteSalidaTiro[ i ].w = 363;
            spriteSalidaTiro[ i ].h = 90;
        }
    }
    if( !TEXTURA_DISPARAR->cargarImagen( "imag/sprites/Di-Cokka/shooting.png") )
    {
        printf( "Fallo disparo tanque\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_DISPARAR;i++){
            spriteDisparar[ i ].x = i*363;
            spriteDisparar[ i ].y = 0;
            spriteDisparar[ i ].w = 363;
            spriteDisparar[ i ].h = 133;
        }
    }
    if( !TEXTURA_GIRAR->cargarImagen( "imag/sprites/Di-Cokka/turning.png") )
    {
        printf( "Fallo girar tanque\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_GIRAR;i++){
            spriteGirar[ i ].x = i*363;
            spriteGirar[ i ].y = 0;
            spriteGirar[ i ].w = 363;
            spriteGirar[ i ].h = 122;
        }
    }
    return success;
}

void VistaDiCokka::animacionMover(){
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int x=posx;
    if (derecha) {
        flip = SDL_FLIP_HORIZONTAL;
        x-=(364-ancho);
    }
    SDL_Rect* currentClip = &spriteMover[ frame % 8];
    TEXTURA_MOVER->render(x,posy,currentClip,0,NULL,flip);
}

void VistaDiCokka::animacionMuerte(){
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int x=posx;
    if (derecha) {
        flip = SDL_FLIP_HORIZONTAL;
        x-=(364-ancho);
    }
    SDL_Rect* currentClip = &spriteMuerte[ frame % 16 ];
    TEXTURA_MUERTE->render(x,posy,currentClip,0,NULL,flip);
}

void VistaDiCokka::animacionSalidaTiro(){
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int x=posx-50;
    if (derecha) {
        flip = SDL_FLIP_HORIZONTAL;
        x= x-364+ancho+100;
    }
    SDL_Rect* currentClip = &spriteSalidaTiro[ frame % 9 ];
    TEXTURA_SALIDA_TIRO->render(x,posy-10,currentClip,0,NULL,flip);
}

void VistaDiCokka::animacionDisparar(){
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int x=posx;
    if (derecha) {
        flip = SDL_FLIP_HORIZONTAL;
        x-=(364-ancho);
    }
    SDL_Rect* currentClip = &spriteDisparar[ frame % 7 ];
    TEXTURA_DISPARAR->render(x,posy-10,currentClip,0,NULL,flip);
}

void VistaDiCokka::animacionGirarADerecha(){
    SDL_Rect* currentClip = &spriteGirar[ frame ];
    TEXTURA_GIRAR->render(posx,posy,currentClip);
}

void VistaDiCokka::animacionGirarAIzquierda(){
    SDL_Rect* currentClip = &spriteGirar[ frame ];
    TEXTURA_GIRAR->render(posx,posy,currentClip);
}

void VistaDiCokka::animacionGirar(){
    if (girando==DERECHA) {
        animacionGirarADerecha();
    }
    if (girando==IZQUIERDA) {
        animacionGirarAIzquierda();
    }
}

void VistaDiCokka::render(){
    if (existe) {
        if (girando == NOGIRO) {
            if (disparando == DISPARANDO) animacionDisparar();
            else if (disparando == TERMINADO) {
                animacionSalidaTiro();
                animacionMover();
            }
            else animacionMover();
        }
        else animacionGirar();
    }
}

void VistaDiCokka::setFrame(int aux) {
    frame = aux;
}

void VistaDiCokka::setDisparando(bool aux) {
    disparando = aux;
}

void VistaDiCokka::setPosx(int aux) {
    posx = aux;
}

void VistaDiCokka::setPosy(int aux) {
    posy = aux;
}

void VistaDiCokka::setExiste(bool aux) {
    existe = aux;
}

void VistaDiCokka::setDireccion(int posNueva){
    derecha = posx < posNueva;
}

void VistaDiCokka::morir() {
    existe = false;
}

void VistaDiCokka::setGirando(int aux) {
    girando = aux;
}

void VistaDiCokka::setDisparando(int aux) {
    disparando = aux;
}

void VistaDiCokka::transparentar(Uint8 alpha) {
    TEXTURA_MUERTE->setAlpha(alpha);
    TEXTURA_MOVER->setAlpha(alpha);
    TEXTURA_SALIDA_TIRO->setAlpha(alpha);
    TEXTURA_DISPARAR->setAlpha(alpha);
    TEXTURA_GIRAR->setAlpha(alpha);
}

VistaDiCokka::~VistaDiCokka() {
    delete TEXTURA_MUERTE;
    delete TEXTURA_MOVER;
    delete TEXTURA_SALIDA_TIRO;
    delete TEXTURA_DISPARAR;
    delete TEXTURA_GIRAR;
}
