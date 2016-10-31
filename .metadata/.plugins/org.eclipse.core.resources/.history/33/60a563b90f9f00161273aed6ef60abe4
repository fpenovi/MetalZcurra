/*
 * Personaje.h
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#ifndef PERSONAJE_H_
#define PERSONAJE_H_
#define LEVEL_WIDTH 4500
#define LEVEL_HEIGHT 480
#define SCREEN_WIDTH 800

using namespace std;


class Personaje {

private:
    int posx = 0;
    int posy = 360;
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

    int frameCorriendo = 0;
    int frameSaltando = 0;
    const static int ANIMACION_CORRIENDO = 9;
    const static int ANIMACION_SALTANDO = 10;

public:
	Personaje();

public:
    void moverX(bool avanzar, int* posX);

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

	virtual ~Personaje();
};

#endif /* PERSONAJE_H_ */
