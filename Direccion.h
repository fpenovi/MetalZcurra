//
// Created by nestor on 13/11/16.
//

#ifndef METALZCURRA_DIRECCION_H
#define METALZCURRA_DIRECCION_H

class Direccion {

private:
    bool derecha;
    bool izquierda;
    bool arriba;
    bool abajo;
    bool ultimaDerecha;
    bool solte;

public:
    Direccion();

    bool isDerecha();

    void setDerecha(bool derecha);

    bool isIzquierda();

    void setIzquierda(bool izquierda);

    bool isArriba();

    void setArriba(bool arriba);

    bool isAbajo();

    void setAbajo(bool abajo);

    bool isUltimaDerecha();

    void setUltimaDerecha(bool aux);

    bool isSolte();

    void setSolte(bool aux);

    void disparoAlaIzq();

    void disparoAlaDer();

    void disparoArriba();

    void disparoAbajo();

    void ultimaDireccion();
};

#endif //METALZCURRA_DIRECCION_H