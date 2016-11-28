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
	int ultimoPosy = 465;
	int ultimoPosx = 0;
	bool saltando = false;
	bool existe;
	string nombre;

	const static int ANIMACION_PARADO = 4;
	const static int ANIMACION_CORRIENDO_PIES = 18;
	const static int ANIMACION_SALTANDO = 12;
	const static int ANIMACION_MURIENDO = 19;

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
	bool muriendo;
	bool transparente;

	SDL_RendererFlip flip;
	SDL_Rect* currentClipPies;
	SDL_Rect* currentClipTorso;
	int indexTorso = 0;
	int indexPies = 0;
	int frameMuriendo = 0;
	int alan;

public:
    VistaPersonaje(SDL_Renderer *renderizador2, int id, int modoJuego);

	~VistaPersonaje();

    void render();

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

	void setUltimoPosy(int aux);

	int getUltimaPosy();

	void setSaltando(bool aux);

	void morir();

	void animacionMuriendo();

	void titilar();

	void noTitilar();

	bool getExiste();

	void setNombre(string name);

	string getNombre();

	void transparentar(Uint8 alpha);
};


#endif //METALZCURRA_VISTAMARCO_H
