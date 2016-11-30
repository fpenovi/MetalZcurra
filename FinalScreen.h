//
// Created by franch on 11/30/16.
//

#ifndef METALZCURRA_FINALSCREEN_H
#define METALZCURRA_FINALSCREEN_H


#include "Textura.h"

class FinalScreen {

protected:
    SDL_Renderer* renderer;
    Textura* textura;

public:
    FinalScreen(SDL_Renderer* renderer);
    virtual void render() = 0;
    virtual ~FinalScreen();

};


#endif //METALZCURRA_FINALSCREEN_H
