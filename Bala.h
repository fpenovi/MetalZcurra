//
// Created by nestor on 06/11/16.
//

#ifndef METALZCURRA_BALA_H
#define METALZCURRA_BALA_H

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

public:
    Bala();

    void mover();

    bool existeBala();

    void setID(int nuevoID);

    void crear(int idEmisor, int x, int y);

    int getPosx();

    int getPosy();

    int getAncho();

    int getAlto();

    void desaparecer();

    int getId();

    int getIdDuenio();

    void setIdDuenio(int id);
};

#endif //METALZCURRA_BALA_H
