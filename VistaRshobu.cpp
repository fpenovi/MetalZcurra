//
// Created by nestor on 26/11/16.
//

#include "VistaRshobu.h"

VistaRshobu::VistaRshobu(SDL_Renderer* render){
    derecha = false;
    alto = 140;
    ancho = 190;
    existe = false;

    TEXTURA_HELICOPTERO_DERECHA = new Textura(render);
    TEXTURA_HELICOPTERO_IZQUIERDA = new Textura(render);
    TEXTURA_HELICE_DERECHA = new Textura(render);
    TEXTURA_HELICE_IZQUIERDA = new Textura(render);

}

bool VistaRshobu::cargarImagen() {
    bool success = true;
    int i;

    if( !TEXTURA_HELICOPTERO_DERECHA->cargarImagen( "imag/sprites/R-Shobu/moveRight.png") )
    {
        printf( "Fallo sprite gelicoptero derecha\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_HELICOPTERO_DERECHA;i++){
            spriteHelicopteroDerecha[ i ].x = i*299;
            spriteHelicopteroDerecha[ i ].y = 0;
            spriteHelicopteroDerecha[ i ].w = 299;
            spriteHelicopteroDerecha[ i ].h = 128;
        }
    }

    if( !TEXTURA_HELICOPTERO_IZQUIERDA->cargarImagen( "imag/sprites/R-Shobu/moveLeft.png") )
    {
        printf( "Fallo sprite gelicoptero izq\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_HELICOPTERO_IZQUIERDA;i++){
            spriteHelicopteroIzquierda[ i ].x = i*299;
            spriteHelicopteroIzquierda[ i ].y = 0;
            spriteHelicopteroIzquierda[ i ].w = 299;
            spriteHelicopteroIzquierda[ i ].h = 146;
        }
    }

    if( !TEXTURA_HELICE_DERECHA->cargarImagen( "imag/sprites/R-Shobu/aspasRight.png") )
    {
        printf( "Fallo sprite helice derecha\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_HELICE;i++){
            spriteHeliceDerecha[ i ].x = i*299;
            spriteHeliceDerecha[ i ].y = 0;
            spriteHeliceDerecha[ i ].w = 299;
            spriteHeliceDerecha[ i ].h = 19;
        }
    }

    if( !TEXTURA_HELICE_IZQUIERDA->cargarImagen( "imag/sprites/R-Shobu/aspasLeft.png") )
    {
        printf( "Fallo sprite HELICE izq\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_HELICE;i++){
            spriteHeliceIzquierda[ i ].x = i*299;
            spriteHeliceIzquierda[ i ].y = 0;
            spriteHeliceIzquierda[ i ].w = 299;
            spriteHeliceIzquierda[ i ].h = 34;
        }
    }

    return success;
}

VistaRshobu::~VistaRshobu(){
    delete TEXTURA_HELICOPTERO_DERECHA;
    delete TEXTURA_HELICOPTERO_IZQUIERDA;
    delete TEXTURA_HELICE_DERECHA;
    delete TEXTURA_HELICE_IZQUIERDA;
}

void VistaRshobu::render(){
    if (existe) {
        if (derecha) animacionDerecha();
        else animacionIzquierda();
    }
}

void VistaRshobu::animacionDerecha(){
    currentClipHelice = &spriteHeliceDerecha[ frame % 5 ];
    TEXTURA_HELICE_DERECHA->render(posx,posy,currentClipHelice);

    currentClipHelicoptero = &spriteHelicopteroDerecha[ frame ];
    TEXTURA_HELICOPTERO_DERECHA->render(posx,posy+10,currentClipHelicoptero);
}

void VistaRshobu::animacionIzquierda(){
    currentClipHelice = &spriteHeliceIzquierda[ frame % 5 ];
    TEXTURA_HELICE_IZQUIERDA->render(posx,posy,currentClipHelice);

    currentClipHelicoptero = &spriteHelicopteroIzquierda[ frame ];
    TEXTURA_HELICOPTERO_IZQUIERDA->render(posx,posy+10,currentClipHelicoptero);
}

void VistaRshobu::setFrame(int aux) {
    frame = aux;
}

void VistaRshobu::setDisparando(bool aux) {
    disparando = aux;
}

void VistaRshobu::setPosx(int aux) {
    posx = aux;
}

void VistaRshobu::setPosy(int aux) {
    posy = aux;
}

void VistaRshobu::setExiste(bool aux) {
    existe = aux;
}

void VistaRshobu::setDireccion(int posNueva){
    derecha = posx < posNueva;
}

void VistaRshobu::morir() {
    existe = false;
}

void VistaRshobu::transparentar(Uint8 alpha) {
    TEXTURA_HELICOPTERO_DERECHA->setAlpha(alpha);
    TEXTURA_HELICOPTERO_IZQUIERDA->setAlpha(alpha);
    TEXTURA_HELICE_DERECHA->setAlpha(alpha);
    TEXTURA_HELICE_IZQUIERDA->setAlpha(alpha);
}
