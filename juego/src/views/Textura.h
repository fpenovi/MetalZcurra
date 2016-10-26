/*
 * Textura.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef TEXTURA_H_
#define TEXTURA_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;


class Textura {

private:
    //The actual hardware texture
    SDL_Texture* textura;
    int ancho;
    int alto;
    SDL_Renderer* renderizador;

public:
    Textura(SDL_Renderer* renderizador);
    bool cargarImagen(string path);
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    int getAncho();
    int getAlto();
    void free();
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
	virtual ~Textura();
};

#endif /* TEXTURA_H_ */
