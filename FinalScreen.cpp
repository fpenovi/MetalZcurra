//
// Created by franch on 11/30/16.
//

#include "FinalScreen.h"

FinalScreen::FinalScreen(SDL_Renderer* renderer) {
    this->renderer = renderer;
    this->textura = new Textura(this->renderer);
}

FinalScreen::~FinalScreen() {
    delete this->textura;
}
