/*
 * ProtocoloComando.h
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#ifndef PROTOCOLOCOMANDO_H_
#define PROTOCOLOCOMANDO_H_

#include <string>

using namespace std;


class ProtocoloComando {

private:
    int scancode;
    int type;

public:
	ProtocoloComando();

    int getScancode();

    int getType();

    void setScancode(int scancode);

    void setType(int type);

    string toString();

    static void parse(string stream, int* key, int* pressed);

	virtual ~ProtocoloComando();
};

#endif /* PROTOCOLOCOMANDO_H_ */
