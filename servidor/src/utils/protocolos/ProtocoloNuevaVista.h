/*
 * ProtocoloNuevaVista.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef PROTOCOLONUEVAVISTA_H_
#define PROTOCOLONUEVAVISTA_H_

#include <string>

using namespace std;

class ProtocoloNuevaVista {

private:
    int object_id;
    int x;
    int y;
    int cam;
    int spriteId;
    int conectado;

public:
	ProtocoloNuevaVista();

    int getObject_id();

    int getX();

    int getY();

    int getSpriteId();

    int getCam();

    void setObject_id(int object_id);

    void setX(int x);

    void setY(int y);

    void setSpriteId(int id);

    void setCam(int camara);

    void setConectado(int conectado);

    string toString();

    static void parse(string stream, int* id, int* sprite, int* posx, int* posy, int* camara, int* conectadp);

	virtual ~ProtocoloNuevaVista();
};

#endif /* PROTOCOLONUEVAVISTA_H_ */