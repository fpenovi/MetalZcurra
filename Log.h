//
// Created by juancho on 03/09/16.
//
#ifndef METALZCURRA_LOG_H
#define METALZCURRA_LOG_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Log {
    public:
    Log(int);
    void loggearMsj(string,string,string);
    void loggearConexion(string);
    void loggearDesconexionViolenta(string);
    void loggearDesconexionNormal(string);
    void inicializoServer();
    void cierroServer();
    void error(string);
    void cerrarLog();
    void loggearRecepcion(string);
    private:
    fstream archlog;
    string getHora();
    int maxtext;
};


#endif //METALZCURRA_LOG_H
