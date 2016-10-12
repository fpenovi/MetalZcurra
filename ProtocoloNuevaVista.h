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
    int spriteId;

public:

    int getObject_id();

    int getX();

    int getY();

    int getSpriteId();

    void setObject_id(int object_id);

    void setX(int x);

    void setY(int y);

    void setSpriteId(int id);

    string toString();

    static void parse(string stream, int* id, int* sprite, int* posx, int* posy);
};


#endif //METALZCURRA_PROTOCOLONUEVAVISTA_H
