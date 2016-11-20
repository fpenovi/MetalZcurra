//
// Created by nestor on 16/11/16.
//

#include "VistaEnemigo.h"

VistaEnemigo::VistaEnemigo(vector<Textura*> TEXTURAS_ENEMIGOS) {
    existe = false;
    muerto = false;
    disparando = false;
    cantPasos = 0;

    TEXTURA_ENEMIGO_MUERTE1 = TEXTURAS_ENEMIGOS[0];
    TEXTURA_ENEMIGO_MUERTE2 = TEXTURAS_ENEMIGOS[1];
    TEXTURA_ENEMIGO_MIRANDO = TEXTURAS_ENEMIGOS[2];
    TEXTURA_ENEMIGO_CORRIENDO = TEXTURAS_ENEMIGOS[3];
    TEXTURA_ENEMIGO_DISPARANDO = TEXTURAS_ENEMIGOS[4];
    TEXTURA_ENEMIGO_QUIETO = TEXTURAS_ENEMIGOS[5];
}

bool VistaEnemigo::cargarImagen(){
    bool success=true;
    int i;
    //Load sprite sheet texture
    if( !TEXTURA_ENEMIGO_MUERTE1->cargarImagen( "imag/sprites/soldier/death1.png") )
    {
        printf( "Fallo sprite MUERTE1\n" );
        success = false;
    }
    else
    {
        for (i = 0;i<ANIMACION_MUERTE1;i++){
            spriteMuerte1[ i ].x = i*149;
            spriteMuerte1[ i ].y = 0;
            spriteMuerte1[ i ].w = 149;
            spriteMuerte1[ i ].h = 96;
        }
    }


    if( !TEXTURA_ENEMIGO_MUERTE2->cargarImagen( "imag/sprites/soldier/death2.png") )
    {
        printf( "Fallo sprite MUERTE2\n" );
        success = false;
    }
    else{
        for (i = 0;i<ANIMACION_MUERTE2;i++){
            spriteMuerte2[ i ].x = i*149;
            spriteMuerte2[ i ].y = 0;
            spriteMuerte2[ i ].w = 149;
            spriteMuerte2[ i ].h = 79;
        }
    }

    if( !TEXTURA_ENEMIGO_MIRANDO->cargarImagen( "imag/sprites/soldier/looking.png") )
    {
        printf( "Fallo sprite LOOKING\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_MIRANDO;i++){
            spriteMirando[ i ].x = i*149;
            spriteMirando[ i ].y = 0;
            spriteMirando[ i ].w = 149;
            spriteMirando[ i ].h = 79;
        }
    }
    if( !TEXTURA_ENEMIGO_CORRIENDO->cargarImagen( "imag/sprites/soldier/run.png") )
    {
        printf( "Fallo sprite SOLDIER RUN\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_CORRIENDO;i++){
            spriteCorriendo[ i ].x = i*149;
            spriteCorriendo[ i ].y = 0;
            spriteCorriendo[ i ].w = 149;
            spriteCorriendo[ i ].h = 86;
        }
    }
    if( !TEXTURA_ENEMIGO_DISPARANDO->cargarImagen( "imag/sprites/soldier/shoot.png") )
    {
        printf( "Fallo sprite SHOOT ENEMIGO\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_DISPARANDO;i++){
            spriteDisparando[ i ].x = i*149;
            spriteDisparando[ i ].y = 0;
            spriteDisparando[ i ].w = 149;
            spriteDisparando[ i ].h = 81;
        }
    }
    if( !TEXTURA_ENEMIGO_QUIETO->cargarImagen( "imag/sprites/soldier/toying.png") )
    {
        printf( "Fallo sprite QUIETO ENEMIGO\n" );
        success = false;
    }
    else{

        for (i = 0;i<ANIMACION_QUIETO;i++){
            spriteQuieto[ i ].x = i*149;
            spriteQuieto[ i ].y = 0;
            spriteQuieto[ i ].w = 149;
            spriteQuieto[ i ].h = 98;
        }
    }
    return success;
}

void VistaEnemigo::render(){
    if (existe){
        if (muerto) animacionMuerte1();
        else if (disparando) animacionDisparando();
        else if (cantPasos > 0) animacionCorriendo();
        else animacionQuieto();
    }
}

bool VistaEnemigo::existeEnemigo(){
    return existe;
}

void VistaEnemigo::setID(int nuevoID){
    id = nuevoID;
}

int VistaEnemigo::getPosx(){
    return posx;
}

int VistaEnemigo::getPosy(){
    return posy;
}

int VistaEnemigo::getAncho(){
    return ancho;
}

int VistaEnemigo::getAlto(){
    return alto;
}

void VistaEnemigo::morir(){
    muerto = true;
}

void VistaEnemigo::setExiste(bool exist){
    existe = exist;
}

int VistaEnemigo::getId() {
    return id;
}

void VistaEnemigo::setPosX(int x) {
    posx = x;
}

void VistaEnemigo::setPosY(int y) {
    posy = y;
}

void VistaEnemigo::animacionMuerte1(){
    flip = SDL_FLIP_HORIZONTAL;
    currentClip = &spriteMuerte1[ frame ];
    TEXTURA_ENEMIGO_MUERTE1->render( posx, posy, currentClip,0,NULL,flip);
    if (frame == 14) existe = false;
}

void VistaEnemigo::animacionMuerte2(){
    flip = SDL_FLIP_HORIZONTAL;
    currentClip = &spriteMuerte2[ frame ];
    TEXTURA_ENEMIGO_MUERTE2->render( posx, posy, currentClip,0,NULL,flip);
}

void VistaEnemigo::animacionMirando(){
    flip = SDL_FLIP_HORIZONTAL;
    currentClip = &spriteMirando[ frame ];
    TEXTURA_ENEMIGO_MIRANDO->render( posx, posy, currentClip,0,NULL,flip);
}

void VistaEnemigo::animacionCorriendo(){
    flip = SDL_FLIP_HORIZONTAL;
    currentClip = &spriteCorriendo[ frame ];
    TEXTURA_ENEMIGO_CORRIENDO->render( posx, posy, currentClip,0,NULL,flip);
}

void VistaEnemigo::animacionDisparando(){
    flip = SDL_FLIP_HORIZONTAL;
    currentClip = &spriteDisparando[ frame ];
    TEXTURA_ENEMIGO_DISPARANDO->render( posx, posy, currentClip,0,NULL,flip);
}

void VistaEnemigo::animacionQuieto(){
    flip = SDL_FLIP_HORIZONTAL;
    currentClip = &spriteQuieto[ frame ];
    TEXTURA_ENEMIGO_QUIETO->render( posx, posy - 10, currentClip,0,NULL,flip);
}

void VistaEnemigo::setFrame(int aux){
    frame = aux;
}

void VistaEnemigo::setDisparando(int aux){
    disparando = (bool) aux;
}

void VistaEnemigo::setCantPasos(int aux) {
    cantPasos = aux;
}

void VistaEnemigo::revivir(){
    existe = false;
    muerto = false;
}

VistaEnemigo::~VistaEnemigo() {}