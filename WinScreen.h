//
// Created by franch on 11/30/16.
//

#ifndef METALZCURRA_WINSCREEN_H
#define METALZCURRA_WINSCREEN_H


#include "FinalScreen.h"

class WinScreen : public FinalScreen {

private:
    TTF_Font* fuente;
    SDL_Color* colorFuente;

    const int MENSAJE_X = 200;
    const int MENSAJE_Y = 90;

public:
    WinScreen(SDL_Renderer* renderer);
    void render();
    ~WinScreen();
};


#endif //METALZCURRA_WINSCREEN_H
