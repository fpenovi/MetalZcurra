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

public:
    WinScreen(SDL_Renderer* renderer);
    void render();
    ~WinScreen();
};


#endif //METALZCURRA_WINSCREEN_H
