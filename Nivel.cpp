/*
 * Nivel.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "Nivel.h"
#include "ParserXML.h"
#include "HMGun.h"
#include "SGun.h"
#include "RLauncher.h"
#include "Rshobu.h"
#include "ObjectManager.h"

Nivel::Nivel(string xmlPath) {

	ParserXML* parser = new ParserXML(xmlPath);
	vector<string> tamNivel = parser->tamNivel();
	vector<string> capas = parser->capas();
	vector<string> plataformas = parser->plataformas();
	vector<string> bonuses = parser->bonuses();
	vector<string> enemigos = parser->enemigos();
	vector<string> boss = parser->boss();

	ObjectManager* OM = ObjectManager::getInstance();

	this->nombre = parser->nombreNivel();
	this->ancho = stoi(tamNivel[0]);
	this->alto = stoi(tamNivel[1]);
	this->capas = capas;
	this->plataformas = crearPlataformas(plataformas);
	// TODO sacar boss de NivelManager luego de crearlo
	//OM.setBonuses = crearBonuses(bonuses);
	// AlgunManager.setBoss(crearBoss(boss));
	OM->crearEnemigos(crearEnemigos(enemigos));
	this->boss = crearBoss(boss);
}

vector<Plataforma*> Nivel::crearPlataformas(vector<string> platformsS) {

	vector<Plataforma*> platforms;
	for (int i=0; i<platformsS.size(); i+=4)
		platforms.push_back(new Plataforma(stoi(platformsS[i]), stoi(platformsS[i+1]), stoi(platformsS[i+2]), stoi(platformsS[i+3])));

	return platforms;
}

vector<Bonus*> Nivel::crearBonuses(vector<string> bonusesStr) {

	vector<Bonus*> bonuses;
	for (int i=0; i<bonusesStr.size(); i+= 3) {
		int x = stoi(bonusesStr[i]);
		int y = stoi(bonusesStr[i+1]);
		string nombre = bonusesStr[i+2];
		Bonus* actual;

		if (nombre.compare(Bonus::HMGun) == 0)
			actual = new HMGun(x, y);

		else if (nombre.compare(Bonus::SGun) == 0)
			actual = new SGun(x, y);

		else if (nombre.compare(Bonus::RLauncher) == 0)
			actual = new RLauncher(x, y);

		bonuses.push_back(actual);
	}

	return bonuses;
}

vector<Enemigo*> Nivel::crearEnemigos(vector<string> enemigosStr) {

	vector<Enemigo*> enemigos;
	for (int i=0; i<enemigosStr.size(); i+=3) {
		int x = stoi(enemigosStr[i]);
		int y = stoi(enemigosStr[i+1]);
		int delta = stoi(enemigosStr[i+2]);
		enemigos.push_back(new Enemigo(x, y, delta));
	}

	return enemigos;
}

Boss* Nivel::crearBoss(vector<string> bossStr) {

	Boss* boss;
	int x = stoi(bossStr[0]);
	int y = stoi(bossStr[1]);
	int delta = stoi(bossStr[2]);

	if (this->nombre.compare("train") == 0)
		boss = new Rshobu(x, y, delta);

	// agregar restantes
	return boss;
}


bool Nivel::hayColision(Personaje *personaje) {
	return false;
}


bool Nivel::haFinalizado() {
	return !ObjectManager::getInstance()->getBoss()->estaVivo();
}


Nivel::~Nivel() {
	for (int i = 0; i < this->plataformas.size(); ++i)
		delete this->plataformas[i];
}

vector<string> Nivel::getCapas() {
	return capas;
}
