//
// Created by duha on 17/10/16.
//

#ifndef METALZCURRA_BACKGROUND_H
#define METALZCURRA_BACKGROUND_H

#include <iostream>
#include <SDL2/SDL_render.h>
#include <vector>
#include "Layer.h"

using namespace std;

class Background {

private:
    int cantCapas;
    int contadorCapas = 0;
    //Layer capas[5]; //HARDODEADO
    vector<Layer*> capas;
    int anchoMax = 0;
    SDL_Renderer* renderizador;

public:
    Background(SDL_Renderer *renderizador);

    ~Background();

    void render( int personajex);

    bool agregar(char* path);

    void prepararEscenario();

    int anchoMaximo();


};

#endif //METALZCURRA_BACKGROUND_H
