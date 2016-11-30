//
// Created by franch on 11/30/16.
//

#include "LoseScreen.h"

LoseScreen::LoseScreen(SDL_Renderer* renderer) : FinalScreen(renderer) {
    this->colorFuente = new SDL_Color{206, 39, 24, 0xFF };
    this->fuente = TTF_OpenFont("imag/sprites/letras/MetalSlug.ttf", 32);
    this->textura->loadFromText("YOU LOSE MOTHERFUCKERS", *(this->colorFuente), this->fuente);
}

void LoseScreen::render() {
    this->textura->render(MENSAJE_X, MENSAJE_Y);
}

LoseScreen::~LoseScreen() {
    delete this->colorFuente;
    TTF_CloseFont(this->fuente);
}
