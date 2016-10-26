/*
 * Layer.h
 *
 *  Created on: Oct 25, 2016
 *      Author: franch
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <SDL2/SDL_render.h>
#include "Textura.h"

using namespace std;

class Layer {

private:
    Textura* fondo;
    int scroll;
    int ancho;
    int alto;
    int vuelta;
    int ultimaPos;
    double velocidad;

public:
    Layer(SDL_Renderer* renderizador);

    int getAncho();

    int getAlto();

    void render(int x, int y );

    void scrollear(int posJugadorx);

    bool cargarImagen(char* path);

    void asignarVelocidad(double anchomax);

	virtual ~Layer();
};

#endif /* LAYER_H_ */
