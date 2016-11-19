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
	posy = 440;
	ancho = 60;
	alto = 80;
	derecha = true;
	renderizador = renderizador2;
	TEXTURA_PERSONAJE_PARADO_PIES = new Textura(renderizador);
	TEXTURA_PERSONAJE_CORRIENDO_PIES = new Textura(renderizador);
	TEXTURA_PERSONAJE_SALTANDO_PIES = new Textura(renderizador);
	arma = new Arma(renderizador);
	gris = false;
	flip = SDL_FLIP_NONE;
	disparar = false;
	//crearHashSprites();
}

void VistaPersonaje::render(bool seMovio){
	if (estaSaltando()) {
		animacionSaltandoPiernas();
		if (disparar) animacionDisparar();
		else animacionSaltandoTorso();
		return;
	}
	if (seMovio){
		animacionCorrerPiernas();
		if (disparar) animacionDisparar();
		else animacionCorrerTorso();
	}
	else {
		animacionParadoPiernas();
		if (disparar) animacionDisparar();
		else animacionParadoTorso();
	}

}

void VistaPersonaje::animacionParadoPiernas(){
	flip = SDL_FLIP_NONE;
	int x = posCamara;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
		x-=80;
	}

	if (disparar) indexPies = 0;
	currentClipPies = &spriteParadoPies[ indexPies ];
	TEXTURA_PERSONAJE_PARADO_PIES->render(x, posy+48, currentClipPies , 0, NULL, flip );

}

void VistaPersonaje::animacionParadoTorso(){
	flip = SDL_FLIP_NONE;
	int x = posCamara;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
		x-=80;
	}
	arma->renderParado(x,posy,indexTorso,flip);
}

void VistaPersonaje::animacionCorrerPiernas(){
	flip = SDL_FLIP_NONE;
	int x = posCamara;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
		x-=80;
	}

	currentClipPies = &spriteCorriendoPies[ indexPies ];
	TEXTURA_PERSONAJE_CORRIENDO_PIES->render( x, posy+45, currentClipPies,0,NULL,flip);
}

void VistaPersonaje::animacionCorrerTorso(){
	flip = SDL_FLIP_NONE;
	int x = posCamara;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
		x-=80;
	}
	arma->renderCorriendo(x, posy, indexTorso, flip);
}

void VistaPersonaje::animacionSaltandoPiernas(){
	flip = SDL_FLIP_NONE;
	int x = posCamara;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
		x-=80;
	}
	currentClipPies = &spriteSaltandoPies[ indexPies ];
	TEXTURA_PERSONAJE_SALTANDO_PIES->render( x, posy+45, currentClipPies, 0, NULL, flip);
}

void VistaPersonaje::animacionSaltandoTorso(){
	flip = SDL_FLIP_NONE;
	int x = posCamara;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
		x-=80;
	}
	arma->renderSaltando(x, posy, indexTorso, flip);
}

void VistaPersonaje::animacionDisparar(){
	flip = SDL_FLIP_NONE;
	int x = posCamara;
	if (!derecha) {
		flip = SDL_FLIP_HORIZONTAL;
		x-=80;
	}
	arma->renderDisparando(x, posy, indexTorso, flip);
}


bool VistaPersonaje::cargarImagen(){
	//Loading success flag
	bool success = true;
	int i;

	//Load sprite sheet texture
	if( !TEXTURA_PERSONAJE_PARADO_PIES->cargarImagen( "imag/sprites/commons/stillLegs.png") )
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

	if( !TEXTURA_PERSONAJE_CORRIENDO_PIES->cargarImagen( "imag/sprites/commons/runLegs.png") )
	{
		printf( "Fallo sprite corriendo pies\n" );
		success = false;
	}
	else{
		for (i = 0; i < ANIMACION_CORRIENDO_PIES; i++){
			spriteCorriendoPies[ i ].x = i*149;
			spriteCorriendoPies[ i ].y = 0;
			spriteCorriendoPies[ i ].w = 149;
			spriteCorriendoPies[ i ].h = 43;
		}
	}

	if( !TEXTURA_PERSONAJE_SALTANDO_PIES->cargarImagen( "imag/sprites/commons/jumpLegs.png") )
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

	return success;
}

void VistaPersonaje::liberarTextura(){
	TEXTURA_PERSONAJE_SALTANDO_PIES->free();
	TEXTURA_PERSONAJE_CORRIENDO_PIES->free();
	TEXTURA_PERSONAJE_PARADO_PIES->free();
	arma->liberar();
	delete arma;
}

VistaPersonaje::~VistaPersonaje() {
	delete TEXTURA_PERSONAJE_PARADO_PIES;
	delete TEXTURA_PERSONAJE_CORRIENDO_PIES;
	delete TEXTURA_PERSONAJE_SALTANDO_PIES;
	delete arma;
}

bool VistaPersonaje::estaSaltando(){
	return (posy != 440);
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
	arma->ponerTexturaGris();
	TEXTURA_PERSONAJE_PARADO_PIES->setColor(128,128,128);
	gris = true;
}

void VistaPersonaje::sacarTexturaGris() {
	arma->sacarTexturaGris();
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

void VistaPersonaje::ponerShotgun(){
	arma->ponerShotgun();
}

void VistaPersonaje::ponerGun(){
	arma->ponerGun();
}

void VistaPersonaje::setDisparar(bool aux) {
	disparar = aux;
}

bool VistaPersonaje::getDisparar() {
	return disparar;
}

void VistaPersonaje::apuntarAbajo(){
	arma->apuntarArriba(false);
	arma->apuntarAbajo(true);
}

void VistaPersonaje::apuntarArriba(){
	arma->apuntarAbajo(false);
	arma->apuntarArriba(true);
}

void VistaPersonaje::noApuntar(){
	arma->apuntarAbajo(false);
	arma->apuntarArriba(false);
}

void VistaPersonaje::apuntar(int aim){
	if (aim == 1) apuntarArriba();
	else if (aim == 2) apuntarAbajo();
	else noApuntar();
}