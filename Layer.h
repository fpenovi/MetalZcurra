//
// Created by duha on 17/10/16.
//

#ifndef METALZCURRA_LAYER_H
#define METALZCURRA_LAYER_H

#include <iostream>
#include <SDL2/SDL_render.h>
#include "Textura.h"

using namespace std;

class Layer {

private:
    Textura* fondo;
    double scroll;
    int ancho;
    int alto;
    int vuelta;
    int ultimaPos;
    double velocidad;


public:
    Layer(SDL_Renderer* renderizador);

    ~Layer();

    int getAncho();

    int getAlto();

    void render(int x, int y );

    void scrollear(int posJugadorx);

    bool cargarImagen(char* path);

    void asignarVelocidad(double anchomax);

};
#endif //METALZCURRA_LAYER_H
