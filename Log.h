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
    void loggearMsj(string,vector<string>,string);
    void loggearConexion(string);
    void loggearDesconexion(string);
    void error();
    void cerrarLog();
    private:
    fstream archlog;
    string procesarVector(vector<string> vector);
    string getHora(const vector<string> &reciben);
    int maxtext;
};


#endif //METALZCURRA_LOG_H
