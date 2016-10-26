/*
 * Log.h
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Log {
private:
	fstream archlog;
	string getHora();
	int maxtext;

public:
    Log(int);
    void loggearMsj(string cliente , string receptor, string texto);
    void loggearConexion(string cliente);
    void loggearDesconexionViolenta(string cliente);
    void loggearDesconexionNormal(string cliente);
    void inicializoServer();
    void cierroServer();
    void error(string texto);
    void cerrarLog();
    void loggearRecepcion(string cliente);
	virtual ~Log();
};

#endif /* LOG_H_ */
