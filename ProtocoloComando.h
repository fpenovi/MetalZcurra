//
// Created by duha on 04/10/16.
//

#include <string>

#ifndef METALZCURRA_PROTOCOLOCOMANDO_H
#define METALZCURRA_PROTOCOLOCOMANDO_H

using namespace std;

class ProtocoloComando {

private:
    int scancode;
    int type;

public:
    int getScancode();

    int getType();

    void setScancode(int scancode);

    void setType(int type);

    string toString();

    static void parse(string stream, int* key, int* pressed);
};

#endif //METALZCURRA_PROTOCOLOCOMANDO_H
