//
// Created by nestor on 06/11/16.
//

#ifndef METALZCURRA_BALA_H
#define METALZCURRA_BALA_H

#include "Direccion.h"

class Bala
{
private:
    int posx;
    int posy;
    int velocidad;
    bool existe;
    int id;
    int ancho;
    int alto;
    int idDuenio;

    bool derecha;
    bool izquierda;
    bool abajo;
    bool arriba;

public:
    Bala();

    void mover();

    bool existeBala();

    void setID(int nuevoID);

    void crear(int idEmisor, int x, int y, Direccion* direccion);

    void crearBalaEnemiga(int x, int y, bool derecha);

    int getPosx();

    int getPosy();

    int getAncho();

    int getAlto();

    void desaparecer();

    int getId();

    int getIdDuenio();

    void setIdDuenio(int id);

    void setDireccion(bool der, bool izq, bool arr, bool abj);

    int getDerecha();

    int getArriba();

    int getAbajo();

    int getIzquierda();
};

#endif //METALZCURRA_BALA_H
