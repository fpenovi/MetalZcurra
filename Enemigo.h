//
// Created by nestor on 15/11/16.
//

#ifndef METALZCURRA_ENEMIGO_H
#define METALZCURRA_ENEMIGO_H

using namespace chrono;

class Enemigo
{
private:
    int id;
    int	posx;
    int posy;
    int ancho;
    int	alto;
    int	muerto;
    int existe;
    int velocidad;
    int cantidadPasos;

    time_point<high_resolution_clock> start;
    time_point<high_resolution_clock> actual;

public:
    Enemigo();

    bool mover();

    void morir();

    int getAncho();

    int getAlto();

    int getPosx();

    int getPosy();

    int getId();

    void setId(int id);

    void setPosx(int x);

    int estaMuerto();

    int getExiste();

    void crear();

    void disparar();
};

#endif //METALZCURRA_ENEMIGO_H
