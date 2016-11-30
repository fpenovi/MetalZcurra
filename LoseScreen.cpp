//
// Created by franch on 11/30/16.
//

#include "LoseScreen.h"

LoseScreen::LoseScreen(SDL_Renderer* renderer) : FinalScreen(renderer) {
    this->colorFuente = new SDL_Color{0xFF, 0xFF, 0xFF, 0xFF };
    this->fuente = TTF_OpenFont("imag/sprites/letras/MetalSlug.ttf", 40);
}

void LoseScreen::render() {
    cout << "YOU LOSE!" << endl;
}

LoseScreen::~LoseScreen() {
    delete this->colorFuente;
    TTF_CloseFont(this->fuente);
}
