//
// Created by juancho on 02/10/16.
//
#include <vector>
#include <unordered_map>
#include "Textura.h"
#include "Arma.h"

#ifndef METALZCURRA_VISTAMARCO_H
#define METALZCURRA_VISTAMARCO_H

class VistaPersonaje {

private:
    int posx, posy;
    int ancho, alto;
    bool conectado;
    int posCamara;

	const static int ANIMACION_PARADO = 4;
	const static int ANIMACION_CORRIENDO_PIES = 18;
	const static int ANIMACION_SALTANDO = 12;

	Textura* TEXTURA_PERSONAJE_PARADO_PIES;
	SDL_Rect spriteParadoPies[ ANIMACION_PARADO ];

	Textura* TEXTURA_PERSONAJE_CORRIENDO_PIES;
	SDL_Rect spriteCorriendoPies[ANIMACION_CORRIENDO_PIES];

	Textura* TEXTURA_PERSONAJE_SALTANDO_PIES;
	SDL_Rect spriteSaltandoPies[ ANIMACION_SALTANDO ];

	Arma* arma;
	bool derecha;

    SDL_Renderer *renderizador;
    int id;
    bool seMovio;
	bool gris;
	bool disparar;

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

	~VistaPersonaje();

    void render(bool seMovio);

    void animacionParadoPiernas();

	void animacionParadoTorso();

    void animacionCorrerPiernas();

    void animacionCorrerTorso();

    void animacionSaltandoPiernas();

	void animacionSaltandoTorso();

	void animacionDisparar();

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

	void ponerGun();

	void ponerShotgun();

	void setDisparar(bool aux);

	bool getDisparar();

	void apuntarAbajo();

	void apuntarArriba();

	void apuntar(int aim);

	void noApuntar();
};


#endif //METALZCURRA_VISTAMARCO_H
