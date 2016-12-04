//
// Created by juancho on 05/10/16.
//

#ifndef METALZCURRA_PERSONAJE_H
#define METALZCURRA_PERSONAJE_H

#include <iostream>
#include "Envolvente.h"

#define LEVEL_WIDTH 4500
#define LEVEL_HEIGHT 600
#define SCREEN_WIDTH 800

using namespace std;

class Personaje {

private:
	int puntaje = 0;
    int posx = 0;
    int posy = 465;
    int ancho = 60;
    int alto = 80;
    int velx = 0;
    int vely = 0;
    const static int Personaje_VEL = 7;
    const static int Personaje_VEL_Y = 5;
    int seMovio;
    int id;
    bool bajando = false;
    int posCamara = 0;
    int conectado = false;
	int amplitudSalto = 120;
	int ultimaPosy = 465;

	bool gravity = true;
	bool saltando = false;
	bool derecha = true;
    bool disparando = false;
    bool arriba = false;
    bool abajo = false;
    bool setearArriba = false;
    bool setearAbajo = false;

    int frameCorriendo = 0;
    int frameSaltando = 0;
    int frameParado = 0;
    int frameDisparando = 0;
    int frameDisparandoArriba = 0;
    int frameDisparandoAbajo = 0;
	Envolvente* envolvente;

    const static int ANIMACION_PARADO = 4;
	const static int ANIMACION_CORRIENDO = 18;
	const static int ANIMACION_SALTANDO = 12;
	const static int ANIMACION_DISPARANDO_GUN = 10;
	const static int ANIMACION_DISPARANDO_SHOTGUN = 4;
	const static int ANIMACION_APUNTANDO = 4;
	const static int ANIMACION_DISPARANDO_GUN_ABAJO = 7;
	const static int ANIMACION_DISPARANDO_GUN_ARRIBA = 10;
	const static int ANIMACION_DISPARANDO_SHOTGUN_ABAJO = 4;
	const static int ANIMACION_DISPARANDO_SHOTGUN_ARRIBA = 4;

    int ANIMACION_ACTUAL = 10;
	int ANIMACION_ACTUAL_DISPARANDO_ARRIBA = 10; // empieza siendo 10 por la gun
	int ANIMACION_ACTUAL_DISPARANDO_ABAJO = 7;
	int armaActual;
	int cantidadBalas = 0;
	int vida = 5;
	bool quieto;
	bool colision;
	bool modoInvencible;
	bool existe;

public:
	const static int GUN = 0;
	const static int HMGUN = 1;
	const static int SHOTGUN = 2;
	const static int RLAUNCHER = 3;

	Personaje();

    void moverX();

    void moverY();

    int getPosx();

    void setPosx(int posx);

    int getPosy() ;

    void setPosy(int posy);

    int getVelx();

    void setVelx(int velx);

    int getVely();

    void setVely(int vely);

    int getSeMovio();

    void setSeMovio(int seMovio);

    int getPersonaje_VEL();

    int getPersonaje_VEL_Y();

    int getId();

    void setId(int id);

    bool getBajando();

    void setBajando(bool bajando);

    int getPosCamara();

    void setPosCamara(int camara);

    void inicial();

    int getConectado();

    void setConectado(int conexion);

    void setSpriteCorriendo();

    int getFrameCorriendo();

    void setSpriteSaltando();

    int getSpriteSaltando();

    void setSpriteParado();

    int getSpriteParado();

    void setSpriteDisparando();

    int getSpriteDisparando();

    void resetFrames();

    void setSprites();

    int getSprites();

    void setDisparando(bool aux);

    int getDisparando();

    void setArriba(bool aux);

    void setAbajo(bool aux);

    void setSpriteDisparandoArriba();

    void setSpriteDisparandoAbajo();

    int getSpriteDisparandoArriba();

    int getSpriteDisparandoAbajo();

    int getDireccion();

	void setUltimaPosy(int aux);

	Envolvente *getEnvolvente();

	void setSaltando(bool aux);

	bool getSaltando();

	bool gravedad();

	void setGravity(bool aux);

	int getArmaActual();

	void setArmaActual(int aux);

	void setShotGunSprites();

	void setGunSprites();

	void setCantidadBalas(int nuevas, int extra, int tipo);

	void restarBala();

	int getCantidadBalas();

	bool getCambioDeArma();

	int getPuntaje();

	void aumentarPuntos(int aux);

	void restarVida(int aux);

	void sumarVida(int aux);

	bool estaVivo();

	void setQuieto(bool aux);

	bool getQuieto();

	bool getColision();

	void setColision(bool aux);

	void setVida(int aux);

	void setModoInvencible(bool aux);

	bool getModoInvencible();

	~Personaje();
};

#endif //METALZCURRA_PERSONAJE_H
