/*
 * Mensaje.h
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#ifndef MENSAJE_H_
#define MENSAJE_H_

#include <string>
#include<iostream>
using namespace std;

class Mensaje {

private:
    string enviador;
    string receptor;
    string texto;

public:
    Mensaje(string,string,string);
    string getNameReceptor();
    string getMensaje();
    string getNameEmisor();
	virtual ~Mensaje();
};

#endif /* MENSAJE_H_ */
