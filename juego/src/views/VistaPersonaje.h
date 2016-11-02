/*
 * VistaPersonaje.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef VISTAPERSONAJE_H_
#define VISTAPERSONAJE_H_

#include <vector>
#include <unordered_map>
#include "Textura.h"

class VistaPersonaje {

private:
    int posx, posy;
    int ancho, alto;
    bool conectado;
    int posCamara;
    const static int ANIMACION_PARADO = 3;
    const static int ANIMACION_CORRIENDO = 9;
    const static int ANIMACION_SALTANDO = 10;
    SDL_Rect spriteParado[ANIMACION_PARADO];
    SDL_Rect spriteCorriendo[ANIMACION_CORRIENDO];
    SDL_Rect spriteSaltando[ANIMACION_SALTANDO];
	int frameDivider;

    int frameCorriendo;
    int frameParado;
    bool derecha;
    bool quieto;
    bool saltando;
    bool subiendo;
    bool bajando;
    int frameSaltando;

    int velx;
    int vely;
    Textura *TEXTURA_PERSONAJE_PARADO;
    Textura *TEXTURA_PERSONAJE_SALTANDO;
    Textura *TEXTURA_PERSONAJE_CORRIENDO;
    const static int Personaje_VEL = 5;
    const static int Personaje_VEL_Y = 4;
    SDL_Renderer *renderizador;
    int id;
    bool seMovio;
	bool gris;

    unordered_map<int, vector<string>*> hashSprites;

    string pathQuieto;
    string pathCorriendo;
    string pathSaltando;

	SDL_RendererFlip flip;
	SDL_Rect* currentClip;
	int index = 0;

public:
	VistaPersonaje(SDL_Renderer *renderizador2);

    void handleEvent(SDL_Event &e);

    bool estaQuieto();

    bool mover();

    void render(bool seMovio);

    void animacionParado();

    void animacionCorrer();

    int animacionSaltando();

    bool cargarImagen();

    void liberarTextura();

    bool estaSaltando();

    int getX();

    int getY();

    int getAncho();

    int getAlto();

    int getId();

    bool getSeMovio();

    void setPosx(int posx);

    void setPosy(int posy);

    void setQuieto(bool quieto);

    void setDerecha(bool derecha);

    void setId(int id);

    void setSeMovio(bool state);

    void setPosCamara(int camara);

    int getPosCamara();

    void crearHashSprites();

    void setearSprites(int id);

    int getConectado();

    void setConectado(bool conexion);

	void ponerTexturaGris();

	bool getGris();

	void sacarTexturaGris();

	void setSpriteIndex(int idx);

	virtual ~VistaPersonaje();
};

#endif /* VISTAPERSONAJE_H_ */
