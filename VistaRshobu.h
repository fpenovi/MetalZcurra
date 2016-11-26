//
// Created by nestor on 26/11/16.
//

#ifndef METALZCURRA_VISTARSHOBU_H
#define METALZCURRA_VISTARSHOBU_H

#include <SDL2/SDL_rect.h>
#include "Textura.h"
#include "VistaBoss.h"

class VistaRshobu : public VistaBoss
{
private:
    Textura* TEXTURA_HELICOPTERO_DERECHA;
    Textura* TEXTURA_HELICOPTERO_IZQUIERDA;
    Textura* TEXTURA_HELICE_DERECHA;
    Textura* TEXTURA_HELICE_IZQUIERDA;

    const static int ANIMACION_HELICOPTERO_DERECHA = 12;
    const static int ANIMACION_HELICOPTERO_IZQUIERDA = 7;
    const static int ANIMACION_HELICE = 5;

    SDL_Rect spriteHelicopteroDerecha[ANIMACION_HELICOPTERO_DERECHA];
    SDL_Rect spriteHelicopteroIzquierda[ANIMACION_HELICOPTERO_IZQUIERDA];
    SDL_Rect spriteHeliceDerecha[ANIMACION_HELICE];
    SDL_Rect spriteHeliceIzquierda[ANIMACION_HELICE];

    int ancho;
    int alto;

    SDL_Rect* currentClipHelice;
    SDL_Rect* currentClipHelicoptero;
    int frame;

public:
    VistaRshobu(SDL_Renderer* render);

    bool cargarImagen();

    void render();

    void animacionDerecha();

    void animacionIzquierda();

    void setDisparando(bool aux);

    void setPosx(int aux);

    void setPosy(int aux);

    void setFrame(int aux);

    void setExiste(bool aux);

    ~VistaRshobu();

    void setDireccion(int posNueva);
};

#endif //METALZCURRA_VISTARSHOBU_H
