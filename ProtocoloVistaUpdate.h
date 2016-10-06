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

public:

    int getObject_id();

    int getEstado();

    int getX();

    int getY();

    void setObject_id(int object_id);

    void setEstado(int estado);

    void setX(int x);

    void setY(int y);

    string toString();

    void parse(string stream, int* id, int* state, int* posx, int* posy);
};

#endif //METALZCURRA_PROTOCOLOVISTAUPDATE_H
