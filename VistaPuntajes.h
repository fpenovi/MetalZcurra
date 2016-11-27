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
	const static int MODO_INDIVIDUAL = 0;
	const static int MODO_COLABORATIVO = 1;
	const static int MODO_GRUPAL = 2;

	static VistaPuntajes* NewVistaPuntaje(int cantPlayers, int modoJuego);
	virtual void actualizarPuntaje(int idJugador, int puntos) = 0;
	virtual void render() = 0;
	virtual ~VistaPuntajes();

};


#endif //METALZCURRA_VISTAPUNTAJES_H
