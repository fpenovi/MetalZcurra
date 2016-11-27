//
// Created by nestor on 09/11/16.
//

#ifndef METALZCURRA_VISTABALA_H
#define METALZCURRA_VISTABALA_H

#include "Textura.h"

class VistaBala
{
private:
    int posx;
    int posy;
    int velocidad;
    bool existe;
    int id;
    int ancho;
    int alto;
    bool derecha;
    bool izquierda;
    bool arriba;
    bool abajo;
    Textura* TEXTURA_BALA;

    bool bomba = false;
    Textura* TEXTURA_EXPLOSION = NULL;
    const static int ANIMACION_EXPLOSION = 10;
    SDL_Rect spriteExplosion[ANIMACION_EXPLOSION];
    int frameExplosion = 0;
    bool explotando = false;
    int posxExp;
    int posyExp;

    const static int ANIMACION_BALA_SHOTGUN = 12;
    SDL_Rect spriteBala[ ANIMACION_BALA_SHOTGUN ];
    SDL_Rect* currentClip;
    SDL_RendererFlip flip;
    double angulo;
    int frame;
    bool shotgun = false;

public:
    VistaBala(Textura* textura);

    void render();

    void cargarImagen();

    bool existeBala();

    void setID(int nuevoID);

    void crear(int x, int y);

    int getPosx();

    int getPosy();

    int getAncho();

    int getAlto();

    void desaparecer();

    int getId();

    void setPosX(int x);

    void setPosY(int y);

    void setExiste(bool exist);

    void setDerecha(int aux);

    void setIzquierda(int aux);

    void setArriba(int aux);

    void setAbajo(int aux);

    void cargarImagenShotgun();

    void setShotgun(bool aux);

    bool isShotgun();

    void setFrame(int aux);

    void setBomba(bool aux);

    bool isBomba();

    void cargarExplosion(SDL_Renderer* renderer);

    void explotar();

    ~VistaBala();
};


#endif //METALZCURRA_VISTABALA_H
