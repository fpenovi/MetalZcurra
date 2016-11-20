/*
 * Nivel.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef NIVEL_H_
#define NIVEL_H_

#include <vector>
#include "Plataforma.h"
#include "Envolvente.h"
#include "Bonus.h"
#include "Boss.h"
#include "Enemigo.h"
#include <string>

using namespace std;

class Nivel {

private:
	string nombre;
	int alto;
	int ancho;
	vector<Plataforma*> plataformas;
	vector<string> capas;
	Boss* boss;

	vector<Plataforma*> crearPlataformas(vector<string> platformsStr);
	vector<Bonus*> crearBonuses(vector<string> bonusesStr);
	vector<Enemigo*> crearEnemigos(vector<string> enemigosStr);
	Boss* crearBoss(vector<string> bossStr);

public:
	Nivel(string xmlPath);
	~Nivel();
	bool haFinalizado();
	// bool hayColision(Personaje* personaje);
	vector<string> getCapas();
};

#endif /* NIVEL_H_ */
