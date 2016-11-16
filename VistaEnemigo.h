//
// Created by nestor on 16/11/16.
//

#ifndef METALZCURRA_VISTAENEMIGO_H
#define METALZCURRA_VISTAENEMIGO_H

#include "Textura.h"

class VistaEnemigo
{
private:
    int posx;
    int posy;
    int velocidad;
    bool existe;
    int id;
    int ancho;
    int alto;
    int	muerto;
    Textura* TEXTURA_ENEMIGO;

public:
    VistaEnemigo(SDL_Renderer* renderer);

    ~VistaEnemigo();

    void render();

    bool cargarImagen();

    bool existeEnemigo();

    void setID(int nuevoID);

    int getPosx();

    int getPosy();

    int getAncho();

    int getAlto();

    void morir();

    int getId();

    void setPosX(int x);

    void setPosY(int y);

    void setExiste(bool exist);
};

#endif //METALZCURRA_VISTAENEMIGO_H
