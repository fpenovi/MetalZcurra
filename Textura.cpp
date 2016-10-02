//
// Created by juancho on 02/10/16.
//

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "Textura.h"

Textura::Textura(SDL_Renderer* renderizador2){
	textura = NULL;
	ancho = 0;
	alto = 0;
	renderizador = renderizador2;
}

//Deallocates memory
Textura::~Textura(){
	free();
}

//Cargar una imagen en la textura
bool Textura::cargarImagen( std::string path){

	//elimino cualquier textura
	free();

	//Aca voy a guardar la ultima
	SDL_Texture* texturaFinal = NULL;

	//Cargo la imagen
	SDL_Surface* imagenCargada = IMG_Load( path.c_str() );
	if( imagenCargada == NULL ){
		printf( "IMPOSIBLE CARGAR IMAGEN %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Si la imagen tiene un fondito de color se la saca
		SDL_SetColorKey( imagenCargada, SDL_TRUE, SDL_MapRGB( imagenCargada->format,0xFF, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		texturaFinal = SDL_CreateTextureFromSurface( renderizador, imagenCargada );
		if( texturaFinal == NULL )
		{
			printf( "IMPOSIBLE CREAR IMAGEN DESDE %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			ancho = imagenCargada->w;
			alto = imagenCargada->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( imagenCargada );


	//Return success
	textura = texturaFinal;
	return textura != NULL;
	}
}

//Deallocates texture
void Textura::free(){
	//Libera textura si existe
	if( textura != NULL ){
		SDL_DestroyTexture( textura );
		textura = NULL;
		ancho = 0;
		alto = 0;
	}
}

//Renderizar textura en un punto
void Textura::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, ancho, alto };

	//Set clip rendering dimensions
	if( clip != NULL ){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//ESTO LO PONE EN PANTALLA
	SDL_RenderCopyEx( renderizador, textura, clip, &renderQuad, angle, center, flip );
}

int Textura::getAncho(){
	return ancho;
}

int Textura::getAlto(){
	return alto;
}
