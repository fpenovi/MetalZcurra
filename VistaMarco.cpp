//
// Created by juancho on 02/10/16.
//

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "VistaMarco.h"


//Initializes the variables
VistaMarco::VistaMarco(SDL_Renderer* renderizador2){
	posx = 0;
	posy = 360;
	ancho=60;
	alto=80;
	frameCorriendo=0;
	frameParado=0;
	frameSaltando=0;
	velx = 0;
	vely = 0;
	derecha = true;
	quieto = true;
	saltando = false;
	subiendo = false;
	bajando = false;
	frameSaltando=0;
	renderizador = renderizador2;
	TEXTURA_PERSONAJE_PARADO = new Textura(renderizador);
	TEXTURA_PERSONAJE_SALTANDO = new Textura(renderizador);
	TEXTURA_PERSONAJE_CORRIENDO = new Textura(renderizador);
}

bool VistaMarco::estaQuieto(){
	return quieto;
}

void VistaMarco::render(bool seMovio){
	if (estaSaltando()) {
		animacionSaltando();
		return;
	}
	if (seMovio){
		animacionCorrer();
	}
	else {
		animacionParado();
	}

}

void VistaMarco::animacionParado(){
	if (saltando) return;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	SDL_Rect* currentClip = &spriteParado[ frameParado / 81];
	TEXTURA_PERSONAJE_PARADO->render(posx,posy, currentClip,0,NULL,flip );;

	++frameParado;

	if( frameParado / 81 >= ANIMACION_PARADO )
	{
		frameParado = 0;
	}

}
void VistaMarco::animacionCorrer(){
	if (saltando) return;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	SDL_Rect* currentClip = &spriteCorriendo[ frameCorriendo / 27 ];
	TEXTURA_PERSONAJE_CORRIENDO->render( posx, posy, currentClip,0,NULL,flip);
	++frameCorriendo;
	if( frameCorriendo / 27 >= ANIMACION_CORRIENDO ){
		frameCorriendo = 0;
	}
}

int VistaMarco::animacionSaltando(){
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	SDL_Rect* currentClip = &spriteSaltando[ frameSaltando / 44];
	TEXTURA_PERSONAJE_SALTANDO->render( posx, posy, currentClip,0,NULL,flip);

	++frameSaltando;
	if( frameSaltando / 44 == ANIMACION_SALTANDO ){
		frameSaltando = 0;
	}
}

bool VistaMarco::cargarImagen(){
	//Loading success flag
	bool success = true;
	int i;

	//Load sprite sheet texture
	if( !TEXTURA_PERSONAJE_PARADO->cargarImagen( "imag/marco/quieto3.png") )
	{
		printf( "Fallo sprite parado\n" );
		success = false;
	}
	else
	{
		//SPRITE PARADO
		for (i = 0;i<ANIMACION_PARADO;i++){
			spriteParado[ i ].x = i*60;
			spriteParado[ i ].y = 0;
			spriteParado[ i ].w = 60;
			spriteParado[ i ].h = 80;
		}
	}

	if( !TEXTURA_PERSONAJE_CORRIENDO->cargarImagen( "imag/marco/corriendo2.png") )
	{
		printf( "Fallo sprite corriendo\n" );
		success = false;
	}
	else{
		for (i = 0;i<ANIMACION_CORRIENDO;i++){
			spriteCorriendo[ i ].x = i*60;
			spriteCorriendo[ i ].y = 0;
			spriteCorriendo[ i ].w = 60;
			spriteCorriendo[ i ].h = 80;
		}
	}

	if( !TEXTURA_PERSONAJE_SALTANDO->cargarImagen( "imag/marco/saltando3.png") )
	{
		printf( "Fallo sprite saltando\n" );
		success = false;
	}
	else{

		for (i = 0;i<ANIMACION_SALTANDO;i++){
			spriteSaltando[ i ].x = i*60;
			spriteSaltando[ i ].y = 0;
			spriteSaltando[ i ].w = 60;
			spriteSaltando[ i ].h = 80;
		}
	}
	return success;
}


void VistaMarco::liberarTextura(){
	TEXTURA_PERSONAJE_SALTANDO->free();
	TEXTURA_PERSONAJE_CORRIENDO->free();
	TEXTURA_PERSONAJE_PARADO->free();

}
bool VistaMarco::estaSaltando(){
	return (posy != 360);
}
int VistaMarco::getX(){
	return posx;
}
int VistaMarco::getY(){
	return posy;
}
int VistaMarco::getAncho(){
	return ancho;
}
int VistaMarco::getAlto(){
	return alto;
}

int VistaMarco::getId() {
	return id;
}

bool VistaMarco::getSeMovio() {
	return seMovio;
}

void VistaMarco::setPosx(int posx) {
	this->posx = posx;
}

void VistaMarco::setPosy(int posy) {
	this->posy = posy;
}

void VistaMarco::setQuieto(bool quieto) {
	this->quieto = quieto;
}

void VistaMarco::setDerecha(bool derecha){
	this->derecha = derecha;
}

void VistaMarco::setId(int id) {
	this->id = id;
}

void VistaMarco::setSeMovio(bool state) {
	this->seMovio = state;
}