//
// Created by franch on 25/11/16.
//

#ifndef METALZCURRA_VISTAPUNTAJES_H
#define METALZCURRA_VISTAPUNTAJES_H

#include <string>

using namespace std;

class VistaPuntajes {

protected:
	VistaPuntajes();

public:
	const static string MODO_INDIVIDUAL = "INDIVIDUAL";
	const static string MODO_COLABORATIVO = "COLABORATIVO";
	const static string MODO_GRUPAL = "GRUPAL";

	static VistaPuntajes* NewVistaPuntaje(int cantPlayers, string modoJuego);
	virtual void actualizarPuntaje(string idJugador, int puntos) = 0;
	virtual void render() = 0;
	virtual ~VistaPuntajes();

};


#endif //METALZCURRA_VISTAPUNTAJES_H
