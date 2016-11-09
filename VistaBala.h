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
    Textura* TEXTURA_BALA;

public:
    VistaBala(SDL_Renderer* renderer);

    ~VistaBala();

    void render();

    bool cargarImagen();

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

};


#endif //METALZCURRA_VISTABALA_H
