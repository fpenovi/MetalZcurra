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

public:

    int getObject_id();

    int getEstado();

    int getX();

    int getY();

    int getPosCamara();

    void setObject_id(int object_id);

    void setEstado(int estado);

    void setX(int x);

    void setY(int y);

    void setPosCamara(int posCamara);

    string toString();

    static void parse(string stream, int* id, int* state, int* posx, int* posy, int* posCamara);
};

#endif //METALZCURRA_PROTOCOLOVISTAUPDATE_H
