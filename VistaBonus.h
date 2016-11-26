//
// Created by nestor on 25/11/16.
//

#ifndef METALZCURRA_VISTABONUS_H
#define METALZCURRA_VISTABONUS_H

#include "Textura.h"

class VistaBonus
{
private:
    int x;
    int y;
    bool existe;
    int id;
    Textura* TEXTURA_BONUS;

public:
    VistaBonus(Textura* textura);

    void render();

    void setPosx(int aux);

    void setPosy(int aux);

    void setExiste(bool aux);

    void setId(int id);

    bool getExiste();

    int getPosx();
};

#endif //METALZCURRA_VISTABONUS_H
