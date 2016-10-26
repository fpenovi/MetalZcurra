/*
 * Background.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

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

    void render( int personajex);

    bool agregar(char* path);

    void prepararEscenario();

    int anchoMaximo();

	virtual ~Background();
};

#endif /* BACKGROUND_H_ */
