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
#include "Recover.h"
#include "KillAll.h"
#include "DaiManji.h"
#include "DiCokka.h"
#include <random>

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
	OM->setBonuses(crearBonuses(bonuses));
	OM->setBoss(crearBoss(boss));
	OM->crearEnemigos(crearEnemigos(enemigos));
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
		Enemigo* enemigo = new Enemigo(x, y, delta);
		enemigo->setBonus(makeBonusOrNull());
		enemigos.push_back(enemigo);
	}

	return enemigos;
}

Boss* Nivel::crearBoss(vector<string> bossStr) {

	Boss* boss = NULL;
	int x = stoi(bossStr[0]);
	int y = stoi(bossStr[1]);
	int delta = stoi(bossStr[2]);

	if (this->nombre.compare("train") == 0)
		boss = new Rshobu(x, y, delta);

	if (this->nombre.compare("city") == 0)
		boss = new DaiManji(x, y, delta);

	if (this->nombre.compare("hangar") == 0)
		boss = new DiCokka(x, y, delta);

	return boss;
}


bool Nivel::hayColision(Personaje *personaje) {

	Envolvente* envolvente = personaje->getEnvolvente();

	for (Plataforma* plataforma : plataformas){
		if (envolvente->hayColisionConPlataforma(plataforma->getEnvolvente())) return true;
	}
	return false;
}


bool Nivel::haFinalizado() {
	return !ObjectManager::getInstance()->getBoss()->estaVivo();
}

void Nivel::moverPlataformas(){
	for (Plataforma* plataforma : plataformas)
		plataforma->moverPlataforma();
}

Nivel::~Nivel() {
	for (int i = 0; i < this->plataformas.size(); i++)
		delete this->plataformas[i];
}

vector<string> Nivel::getCapas() {
	return capas;
}


Bonus* Nivel::makeBonusOrNull() {

	random_device randomGenerator;
	// Ponderado 70% NULL, 15% KillAll, 15% Recover

	int randomInt = randomGenerator() % 100;

	if (randomInt > 85)
		return new KillAll(0, 0);

	if (randomInt > 70)
		return new Recover(0, 0);

	return NULL;
}

bool Nivel::hayColisionSalto(Personaje* personaje) {

	Envolvente* envolvente = personaje->getEnvolvente();

	for (Plataforma* plataforma : plataformas){
		if (envolvente->hayColisionConPlataformaSalto(plataforma->getEnvolvente())) return true;
	}
	return false;

}
