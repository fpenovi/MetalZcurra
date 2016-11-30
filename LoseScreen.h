//
// Created by franch on 11/30/16.
//

#ifndef METALZCURRA_LOSESCREEN_H
#define METALZCURRA_LOSESCREEN_H


#include "FinalScreen.h"

class LoseScreen : public FinalScreen {

private:
    TTF_Font* fuente;
    SDL_Color* colorFuente;

    const int MENSAJE_X = 70;
    const int MENSAJE_Y = 150;

public:
    LoseScreen(SDL_Renderer* renderer);
    void render();
    ~LoseScreen();
};


#endif //METALZCURRA_LOSESCREEN_H
