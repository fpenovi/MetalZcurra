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
};


#endif //METALZCURRA_VISTABALA_H
