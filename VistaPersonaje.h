//
// Created by juancho on 02/10/16.
//
#include <vector>
#include <unordered_map>
#include "Textura.h"

#ifndef METALZCURRA_VISTAMARCO_H
#define METALZCURRA_VISTAMARCO_H

class VistaPersonaje {

private:
    int posx, posy;
    int ancho, alto;
    bool conectado;
    int posCamara;

	const static int ANIMACION_PARADO=4;
	const static int ANIMACION_CORRIENDO=18;
	const static int ANIMACION_SALTANDO=12;

	Textura* TEXTURA_PERSONAJE_PARADO_PIES;
	Textura* TEXTURA_PERSONAJE_PARADO_TORSO;
	SDL_Rect spriteParadoTorso[ ANIMACION_PARADO ];
	SDL_Rect spriteParadoPies[ ANIMACION_PARADO ];

	Textura* TEXTURA_PERSONAJE_CORRIENDO_PIES;
	Textura* TEXTURA_PERSONAJE_CORRIENDO_TORSO;
	SDL_Rect spriteCorriendoTorso[ ANIMACION_CORRIENDO ];
	SDL_Rect spriteCorriendoPies[ ANIMACION_CORRIENDO ];

	Textura* TEXTURA_PERSONAJE_SALTANDO_PIES;
	Textura* TEXTURA_PERSONAJE_SALTANDO_TORSO;
	SDL_Rect spriteSaltandoTorso[ ANIMACION_SALTANDO ];
	SDL_Rect spriteSaltandoPies[ ANIMACION_SALTANDO ];

	bool derecha;

    SDL_Renderer *renderizador;
    int id;
    bool seMovio;
	bool gris;

    unordered_map<int, vector<string>*> hashSprites;

    string pathQuieto;
    string pathCorriendo;
    string pathSaltando;

	SDL_RendererFlip flip;
	SDL_Rect* currentClipPies;
	SDL_Rect* currentClipTorso;
	int indexTorso = 0;
	int indexPies = 0;

public:
    VistaPersonaje(SDL_Renderer *renderizador2);

    void render(bool seMovio);

    void animacionParado();

    void animacionCorrer();

    void animacionSaltando();

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

	void setSpriteIndexTorso(int idx);

	void setSpriteIndexPies(int idx);
};


#endif //METALZCURRA_VISTAMARCO_H
