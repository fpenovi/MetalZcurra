//
// Created by nestor on 05/11/16.
//

#ifndef METALZCURRA_TEXTO_H
#define METALZCURRA_TEXTO_H

#include "Textura.h"
#include <string>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_clipboard.h>

using namespace std;

class Texto {

private:
    Textura* texturaTitulo;
    Textura* texturaInput;
    int ancho;
    int alto;
    string titulo;
    string inputText;
    TTF_Font* gFont;
    SDL_Renderer* renderizador;

public:

    Texto(string tit, SDL_Renderer* render);

    bool cargarTitulo();

    bool pedir();

    string getTexto();

    void renderTitulo(int x, int y);

    ~Texto();
};

#endif //METALZCURRA_TEXTO_H
