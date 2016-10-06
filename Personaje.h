//
// Created by juancho on 05/10/16.
//

#ifndef METALZCURRA_PERSONAJE_H
#define METALZCURRA_PERSONAJE_H
#define LEVEL_WIDTH 4500
#define LEVEL_HEIGHT 480

class Personaje {

private:
    int posx = 0;
    int posy = 240;;
    int ancho = 60;
    int alto = 80;
    int velx = 0;
    int vely = 0;
    const static int Personaje_VEL = 5;
    const static int Personaje_VEL_Y = 4;
    int seMovio;

public:
    void mover();

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
};

#endif //METALZCURRA_PERSONAJE_H
