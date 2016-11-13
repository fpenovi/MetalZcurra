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
	derecha = true;
	renderizador = renderizador2;
	TEXTURA_PERSONAJE_PARADO_PIES = new Textura(renderizador);
	TEXTURA_PERSONAJE_PARADO_TORSO = new Textura(renderizador);
	TEXTURA_PERSONAJE_CORRIENDO_PIES = new Textura(renderizador);
	TEXTURA_PERSONAJE_CORRIENDO_TORSO = new Textura(renderizador);
	TEXTURA_PERSONAJE_SALTANDO_PIES = new Textura(renderizador);
	TEXTURA_PERSONAJE_SALTANDO_TORSO = new Textura(renderizador);
	gris = false;
	flip = SDL_FLIP_NONE;

	crearHashSprites();
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
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	else flip = SDL_FLIP_NONE;

	currentClipPies = &spriteParadoPies[ indexPies ];
	currentClipTorso = &spriteParadoTorso[ indexTorso ];
	TEXTURA_PERSONAJE_PARADO_PIES->render(posCamara,posy+48, currentClipPies,0,NULL,flip );
	TEXTURA_PERSONAJE_PARADO_TORSO->render(posCamara,posy, currentClipTorso,0,NULL,flip );

}

void VistaPersonaje::animacionCorrer(){
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	else flip = SDL_FLIP_NONE;

	currentClipPies = &spriteCorriendoPies[ indexPies ];
	currentClipTorso = &spriteCorriendoTorso[ indexTorso ];
	TEXTURA_PERSONAJE_CORRIENDO_PIES->render( posCamara, posy+45, currentClipPies,0,NULL,flip);
	TEXTURA_PERSONAJE_CORRIENDO_TORSO->render( posCamara, posy, currentClipTorso,0,NULL,flip);

}

void VistaPersonaje::animacionSaltando(){
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	else flip = SDL_FLIP_NONE;

	currentClipPies = &spriteSaltandoPies[ indexPies ];
	currentClipTorso = &spriteSaltandoTorso[ indexTorso ];
	TEXTURA_PERSONAJE_SALTANDO_PIES->render( posCamara, posy+45, currentClipPies,0,NULL,flip);
	TEXTURA_PERSONAJE_SALTANDO_TORSO->render( posCamara, posy, currentClipTorso,0,NULL,flip);

}

bool VistaPersonaje::cargarImagen(){
	//Loading success flag
	bool success = true;
	int i;

	//Load sprite sheet texture
	if( !TEXTURA_PERSONAJE_PARADO_PIES->cargarImagen( "imag/sprites/stillLegs.png") )
	{
		printf( "Fallo sprite parado pies\n" );
		success = false;
	}
	else
	{
		for (i = 0;i<ANIMACION_PARADO;i++){
			spriteParadoPies[ i ].x = i*149;
			spriteParadoPies[ i ].y = 0;
			spriteParadoPies[ i ].w = 149;
			spriteParadoPies[ i ].h = 34;
		}
	}
	if( !TEXTURA_PERSONAJE_PARADO_TORSO->cargarImagen( "imag/sprites/stillTorso.png") )
	{
		printf( "Fallo sprite parado torso\n" );
		success = false;
	}
	else
	{
		for (i = 0;i<ANIMACION_PARADO;i++){
			spriteParadoTorso[ i ].x = i*149;
			spriteParadoTorso[ i ].y = 0;
			spriteParadoTorso[ i ].w = 149;
			spriteParadoTorso[ i ].h = 62;
		}
	}

	if( !TEXTURA_PERSONAJE_CORRIENDO_PIES->cargarImagen( "imag/sprites/runLegs.png") )
	{
		printf( "Fallo sprite corriendo pies\n" );
		success = false;
	}
	else{
		for (i = 0;i<ANIMACION_CORRIENDO;i++){
			spriteCorriendoPies[ i ].x = i*149;
			spriteCorriendoPies[ i ].y = 0;
			spriteCorriendoPies[ i ].w = 149;
			spriteCorriendoPies[ i ].h = 43;
		}
	}
	if( !TEXTURA_PERSONAJE_CORRIENDO_TORSO->cargarImagen( "imag/sprites/runTorso.png") )
	{
		printf( "Fallo sprite corriendo torso\n" );
		success = false;
	}
	else{
		for (i = 0;i<ANIMACION_CORRIENDO;i++){
			spriteCorriendoTorso[ i ].x = i*149;
			spriteCorriendoTorso[ i ].y = 0;
			spriteCorriendoTorso[ i ].w = 149;
			spriteCorriendoTorso[ i ].h = 64;
		}
	}

	if( !TEXTURA_PERSONAJE_SALTANDO_PIES->cargarImagen( "imag/sprites/jumpLegs.png") )
	{
		printf( "Fallo sprite saltando pies\n" );
		success = false;
	}
	else{

		for (i = 0;i<ANIMACION_SALTANDO;i++){
			spriteSaltandoPies[ i ].x = i*149;
			spriteSaltandoPies[ i ].y = 0;
			spriteSaltandoPies[ i ].w = 149;
			spriteSaltandoPies[ i ].h = 56;
		}
	}
	if( !TEXTURA_PERSONAJE_SALTANDO_TORSO->cargarImagen( "imag/sprites/jumpTorso.png") )
	{
		printf( "Fallo sprite saltando torso\n" );
		success = false;
	}
	else{

		for (i = 0;i<ANIMACION_SALTANDO;i++){
			spriteSaltandoTorso[ i ].x = i*149;
			spriteSaltandoTorso[ i ].y = 0;
			spriteSaltandoTorso[ i ].w = 149;
			spriteSaltandoTorso[ i ].h = 79;
		}
	}


	return success;
}


void VistaPersonaje::liberarTextura(){
	TEXTURA_PERSONAJE_SALTANDO_PIES->free();
	TEXTURA_PERSONAJE_SALTANDO_TORSO->free();
	TEXTURA_PERSONAJE_CORRIENDO_PIES->free();
	TEXTURA_PERSONAJE_PARADO_PIES->free();
	TEXTURA_PERSONAJE_CORRIENDO_TORSO->free();
	TEXTURA_PERSONAJE_PARADO_TORSO->free();

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

void VistaPersonaje::ponerTexturaGris() {
	TEXTURA_PERSONAJE_PARADO_TORSO->setColor(128,128,128);
	TEXTURA_PERSONAJE_PARADO_PIES->setColor(128,128,128);
	gris = true;
}

void VistaPersonaje::sacarTexturaGris() {
	TEXTURA_PERSONAJE_PARADO_TORSO->setColor(255,255,255);
	TEXTURA_PERSONAJE_PARADO_PIES->setColor(255,255,255);
	gris = false;
}

bool VistaPersonaje::getGris() {
	return gris;
}

void VistaPersonaje::setSpriteIndexTorso(int idx) {
	this->indexTorso = idx;
}

void VistaPersonaje::setSpriteIndexPies(int idx) {
	this->indexPies = idx;
}