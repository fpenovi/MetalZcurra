//
// Created by juancho on 05/10/16.
//

#ifndef METALZCURRA_PERSONAJE_H
#define METALZCURRA_PERSONAJE_H

#include <iostream>

#define LEVEL_WIDTH 4500
#define LEVEL_HEIGHT 600
#define SCREEN_WIDTH 800

using namespace std;

class Personaje {

private:
    int posx = 0;
    int posy = 440;
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
    bool disparando = false;

    int frameCorriendo = 0;
    int frameSaltando = 0;
    int frameParado = 0;
    int frameDisparando = 0;
    const static int ANIMACION_PARADO = 4;
    const static int ANIMACION_CORRIENDO = 18;
    const static int ANIMACION_SALTANDO = 12;
    const static int ANIMACION_DISPARANDO_GUN = 10;
    const static int ANIMACION_DISPARANDO_SHOTGUN = 4;
    int ANIMACION_ACTUAL = 10;

public:
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
};

#endif //METALZCURRA_PERSONAJE_H
