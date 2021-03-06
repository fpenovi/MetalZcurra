//
// Created by nestor on 06/11/16.
//

#ifndef METALZCURRA_BALA_H
#define METALZCURRA_BALA_H

#include <stddef.h>
#include "Direccion.h"
#include "Envolvente.h"

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
    int danio;
    int puntos;

    bool derecha;
    bool izquierda;
    bool abajo;
    bool arriba;

    Envolvente* envolvente = NULL;

    bool rlauncher = false;

    // Shotgun
    bool shotgun;
    const static int ANIMACION_BALA_SHOTGUN = 12;
    int frameShotgun = 0;
    bool colisiono;

public:
    Bala();

    void mover();

    bool existeBala();

    void setID(int nuevoID);

    void crear(int idEmisor, int x, int y, Direccion* direccion, int ancho, int alto);

    void crearBalaEnemiga(int x, int y, bool derecha, int ancho, int alto);

    void crearBalaBoss(int x, int y, int ancho, int alto);

    void crearBalaTanque(int x, int y, int ancho, int alto, bool derecha);

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

    void handleColision();

    void setShotgun(bool aux);

    void setRlauncher(bool aux);

    bool isShotgun();

    void moverShotgun();

    int getFrameShotgun();

    void setDanio(int aux);

    void setPuntos(int aux);

    bool getColisiono();

    ~Bala();
};

#endif //METALZCURRA_BALA_H
