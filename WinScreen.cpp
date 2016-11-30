//
// Created by franch on 11/30/16.
//

#include "WinScreen.h"

WinScreen::WinScreen(SDL_Renderer* renderer) : FinalScreen(renderer) {
    // ToDo Cargar imagen en vez de un texto si quiero mostrar una imagen piola
    this->colorFuente = new SDL_Color{0xFF, 0xFF, 0xFF, 0xFF };
    this->fuente = TTF_OpenFont("imag/sprites/letras/MetalSlug.ttf", 100);
    this->textura->loadFromText("YOU WIN!", *(this->colorFuente), this->fuente);
}


void WinScreen::render() {
    this->textura->render(MENSAJE_X, MENSAJE_Y);
}


WinScreen::~WinScreen() {
    delete this->colorFuente;
    TTF_CloseFont(this->fuente);
}

