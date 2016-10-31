/*
 * VistaPersonaje.cpp
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#include "VistaPersonaje.h"

//Initializes the variables
VistaPersonaje::VistaPersonaje(SDL_Renderer* renderizador2){
	posx = 0;
	posy = 360;
	ancho=60;
	alto=80;
	frameParado=0;
	frameDivider=30;
	velx = 0;
	vely = 0;
	derecha = true;
	quieto = true;
	saltando = false;
	subiendo = false;
	bajando = false;
	renderizador = renderizador2;
	TEXTURA_PERSONAJE_PARADO = new Textura(renderizador);
	TEXTURA_PERSONAJE_SALTANDO = new Textura(renderizador);
	TEXTURA_PERSONAJE_CORRIENDO = new Textura(renderizador);
	gris = false;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

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
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	else flip = SDL_FLIP_NONE;

	int index = frameParado / frameDivider;
	if( index >= ANIMACION_PARADO ) frameParado = 0;

	currentClip = &spriteParado[ index ];
	TEXTURA_PERSONAJE_PARADO->render(posCamara,posy, currentClip,0,NULL,flip );

	frameParado++;
}

void VistaPersonaje::animacionCorrer(){
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	else flip = SDL_FLIP_NONE;

	currentClip = &spriteCorriendo[ index ];
	TEXTURA_PERSONAJE_CORRIENDO->render( posCamara, posy, currentClip,0,NULL,flip);

}

int VistaPersonaje::animacionSaltando(){
	if (!derecha) flip = SDL_FLIP_HORIZONTAL;
	else flip = SDL_FLIP_NONE;

	currentClip = &spriteSaltando[ index ];
	TEXTURA_PERSONAJE_SALTANDO->render( posCamara, posy, currentClip,0,NULL,flip);
}

bool VistaPersonaje::cargarImagen(){
	//Loading success flag
	bool success = true;
	int i;

	//Load sprite sheet texture
	if( !TEXTURA_PERSONAJE_PARADO->cargarImagen( pathQuieto) )
	{
		printf( "Fallo sprite parado\n" );
		if( !TEXTURA_PERSONAJE_PARADO->cargarImagen( "imag/cruz/quieto.png"))
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
		if (!TEXTURA_PERSONAJE_PARADO->cargarImagen( "imag/cruz/corriendo.png"))
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
		if (!TEXTURA_PERSONAJE_PARADO->cargarImagen( "imag/cruz/saltando.png"))
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

void VistaPersonaje::setSpriteIndex(int idx) {
	this->index = idx;
}

VistaPersonaje::~VistaPersonaje() {
	// TODO Auto-generated destructor stub
}
