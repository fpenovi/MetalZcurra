//
// Created by juancho on 07/09/16.
//
#include <string>
#include<iostream>
using namespace std;

#ifndef METALZCURRA_MENSAJE_H
#define METALZCURRA_MENSAJE_H


class Mensaje {
private:
    string enviador;
    string receptor;
    string texto;
public:
    Mensaje(string,string,string);
    string getNameReceptor();
    string getMensaje();

};


#endif //METALZCURRA_MENSAJE_H
