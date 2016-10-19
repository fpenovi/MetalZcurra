//
// Created by juancho on 02/10/16.
//

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "VistaPersonaje.h"


//Initializes the variables
VistaPersonaje::VistaPersonaje(SDL_Renderer* renderizador2){
	posx = 0;
	posy = 360;
	ancho=60;
	alto=80;
	frameCorriendo=0;
	frameParado=0;
	frameSaltando=0;
	frameDivider=3;
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
	gris = false;

	crearHashSprites();
}

bool VistaPersonaje::estaQuieto(){
	return quieto;
}

void VistaPersonaje::render(bool seMovio){
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

void VistaPersonaje::animacionParado(){
	if (saltando) return;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	SDL_Rect* currentClip = &spriteParado[ frameParado / frameDivider];
	TEXTURA_PERSONAJE_PARADO->render(posCamara,posy, currentClip,0,NULL,flip );;

	++frameParado;

	if( frameParado / (frameDivider) >= ANIMACION_PARADO )
	{
		frameParado = 0;
	}

}
void VistaPersonaje::animacionCorrer(){
	if (saltando) return;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	SDL_Rect* currentClip = &spriteCorriendo[ frameCorriendo / (frameDivider*3) ];
	TEXTURA_PERSONAJE_CORRIENDO->render( posCamara, posy, currentClip,0,NULL,flip);
	++frameCorriendo;
	if( frameCorriendo / (frameDivider*3) >= ANIMACION_CORRIENDO ){
		frameCorriendo = 0;
	}
}

int VistaPersonaje::animacionSaltando(){
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	SDL_Rect* currentClip = &spriteSaltando[ frameSaltando / ((frameDivider+1)*3)];
	TEXTURA_PERSONAJE_SALTANDO->render( posCamara, posy, currentClip,0,NULL,flip);

	++frameSaltando;
	if( frameSaltando / ((frameDivider+1)*3) == ANIMACION_SALTANDO ){
		frameSaltando = 0;
	}
}

bool VistaPersonaje::cargarImagen(){
	//Loading success flag
	bool success = true;
	int i;

	//Load sprite sheet texture
	if( !TEXTURA_PERSONAJE_PARADO->cargarImagen( pathQuieto) )
	{
		printf( "Fallo sprite parado\n" );
		TEXTURA_PERSONAJE_PARADO->cargarImagen( "imag/cruz/quieto.png");
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

	if( !TEXTURA_PERSONAJE_CORRIENDO->cargarImagen( pathCorriendo) )
	{
		printf( "Fallo sprite corriendo\n" );
		TEXTURA_PERSONAJE_PARADO->cargarImagen( "imag/cruz/corriendo.png");
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

	if( !TEXTURA_PERSONAJE_SALTANDO->cargarImagen( pathSaltando) )
	{
		printf( "Fallo sprite saltando\n" );
		TEXTURA_PERSONAJE_PARADO->cargarImagen( "imag/cruz/saltando.png");
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


void VistaPersonaje::liberarTextura(){
	TEXTURA_PERSONAJE_SALTANDO->free();
	TEXTURA_PERSONAJE_CORRIENDO->free();
	TEXTURA_PERSONAJE_PARADO->free();

}
bool VistaPersonaje::estaSaltando(){
	return (posy != 360);
}
int VistaPersonaje::getX(){
	return posx;
}
int VistaPersonaje::getY(){
	return posy;
}
int VistaPersonaje::getAncho(){
	return ancho;
}
int VistaPersonaje::getAlto(){
	return alto;
}

int VistaPersonaje::getId() {
	return id;
}

bool VistaPersonaje::getSeMovio() {
	return seMovio;
}

void VistaPersonaje::setPosx(int posx) {
	this->posx = posx;
}

void VistaPersonaje::setPosy(int posy) {
	this->posy = posy;
}

void VistaPersonaje::setQuieto(bool quieto) {
	this->quieto = quieto;
}

void VistaPersonaje::setDerecha(bool derecha){
	this->derecha = derecha;
}

void VistaPersonaje::setId(int id) {
	this->id = id;
}

void VistaPersonaje::setSeMovio(bool state) {
	this->seMovio = state;
}

void VistaPersonaje::setPosCamara(int camara) {
	this->posCamara = camara;
}

int VistaPersonaje::getPosCamara(){
	return posCamara;
}

int VistaPersonaje::getConectado() {
	return conectado;
}

void VistaPersonaje::setConectado(bool conexion) {
	this->conectado = conexion;
}

void VistaPersonaje::crearHashSprites() {

	hashSprites[1] = new vector<string>;
	hashSprites[1]->push_back("imag/marco/quieto.png");
	hashSprites[1]->push_back("imag/marco/corriendo.png");
	hashSprites[1]->push_back("imag/marco/saltando.png");

	hashSprites[2] = new vector<string>;
	hashSprites[2]->push_back("imag/goku/quieto.png");
	hashSprites[2]->push_back("imag/goku/corriendo.png");
	hashSprites[2]->push_back("imag/goku/saltando.png");

	hashSprites[3] = new vector<string>;
	hashSprites[3]->push_back("imag/jackson/quieto.png");
	hashSprites[3]->push_back("imag/jackson/corriendo.png");
	hashSprites[3]->push_back("imag/jackson/saltando.png");

	hashSprites[4] = new vector<string>;
	hashSprites[4]->push_back("imag/megaman/quieto.png");
	hashSprites[4]->push_back("imag/megaman/corriendo.png");
	hashSprites[4]->push_back("imag/megaman/saltando.png");

}

void VistaPersonaje::setearSprites(int id) {

	pathQuieto = (*hashSprites[id])[0];
	pathCorriendo = (*hashSprites[id])[1];
	pathSaltando = (*hashSprites[id])[2];

}

void VistaPersonaje::setFrameDivider(int divisor) {

	if (divisor <= 0)
		divisor = 3;

	else if (divisor > 90)
		divisor = 90;

	this->frameDivider = divisor;
}

int VistaPersonaje::getFrameDivider() {
	return this->frameDivider;
}

void VistaPersonaje::ponerTexturaGris() {
	TEXTURA_PERSONAJE_PARADO->setColor(128,128,128);
	gris = true;
}

void VistaPersonaje::sacarTexturaGris() {
	TEXTURA_PERSONAJE_PARADO->setColor(255,255,255);
	gris = false;
}

bool VistaPersonaje::getGris() {
	return gris;
}