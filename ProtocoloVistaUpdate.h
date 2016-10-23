//
// Created by juancho on 05/10/16.
//

#include <string>

#ifndef METALZCURRA_PROTOCOLOVISTAUPDATE_H
#define METALZCURRA_PROTOCOLOVISTAUPDATE_H

using namespace std;

class ProtocoloVistaUpdate {

private:
    int object_id;
    int estado;
    int x;
    int y;
    int posCamara;
    int conectado;
    int spriteIndex;

public:

    int getObject_id();

    int getEstado();

    int getX();

    int getY();

    int getPosCamara();

    int getSpriteIndex();

    void setObject_id(int object_id);

    void setEstado(int estado);

    void setX(int x);

    void setY(int y);

    void setPosCamara(int posCamara);

    void setConectado(int conectado);

    void setSpriteIndex(int idx);

    string toString();

    static void parse(string stream, int* id, int* state, int* posx, int* posy, int* posCamara, int* conectado, int* spriteIndex);
};

#endif //METALZCURRA_PROTOCOLOVISTAUPDATE_H
