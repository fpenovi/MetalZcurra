//
// Created by franch on 11/23/16.
//

#ifndef METALZCURRA_KILLALL_H
#define METALZCURRA_KILLALL_H


#include "Bonus.h"

class KillAll : public Bonus {

public:
    KillAll(int x, int y);
    void aplicarEfecto(Personaje* personaje);
    ~KillAll();
};


#endif //METALZCURRA_KILLALL_H
