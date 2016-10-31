/*
 * ProtocoloVistaUpdate.h
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#ifndef PROTOCOLOVISTAUPDATE_H_
#define PROTOCOLOVISTAUPDATE_H_

#include <string>

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
	ProtocoloVistaUpdate();

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

	virtual ~ProtocoloVistaUpdate();
};

#endif /* PROTOCOLOVISTAUPDATE_H_ */