//
// Created by franch on 11/23/16.
//

#ifndef METALZCURRA_RECOVER_H
#define METALZCURRA_RECOVER_H


#include "Bonus.h"

class Recover : public Bonus {

public:
    Recover(int x, int y);
    void aplicarEfecto(Personaje* personaje);
    ~Recover();
};


#endif //METALZCURRA_RECOVER_H
