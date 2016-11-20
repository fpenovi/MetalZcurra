//
// Created by nestor on 12/10/16.
//

#include <string>

#ifndef METALZCURRA_PROTOCOLONUEVAVISTA_H
#define METALZCURRA_PROTOCOLONUEVAVISTA_H


using namespace std;

class ProtocoloNuevaVista {

private:
    int object_id;
    int x;
    int y;
    int cam;
    int spriteId;
    int conectado;
    int duenio;

public:

    int getObject_id();

    int getX();

    int getY();

    int getSpriteId();

    int getCam();

    int getDuenio();

    void setObject_id(int object_id);

    void setX(int x);

    void setY(int y);

    void setSpriteId(int id);

    void setCam(int camara);

    void setConectado(int conectado);

    void setDuenio(int aux);

    string toString();

    static void parse(string stream, int* id, int* sprite, int* posx, int* posy, int* camara, int* conectadp, int* owner);
};


#endif //METALZCURRA_PROTOCOLONUEVAVISTA_H
