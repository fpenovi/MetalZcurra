/*
 * ObjectManager.h
 *
 *  Created on: Oct 23, 2016
 *      Author: franch
 */

#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include <string>
#include <unordered_map>
#include "../model/Personaje.h"
#include "ParserXML.h"


using namespace std;

class ObjectManager {

private:
	unordered_map<int, Personaje*> objects;
	unordered_map<string, int> tablaUsuarios;
	static ObjectManager* instancia;
	int idActual;
	ObjectManager();
	int posx;

public:
	virtual ~ObjectManager();
	Personaje* getObject(int id);
	void addObject(int id, Personaje* object);
	void registerUser(string username);
	int getIdByUsername(string username);
	static ObjectManager* getInstance();
	void crearPersonajes(int cantidad);
	void enviarPersonajes(int FD);
	int* getPosX();
	bool puedoAvanzar();
	void moverCamara(int id);
	void enviarEscenario(ParserXML* parser, int FD);
	void conectarPersonaje(string user);
	void desconectarPersonaje(string user);
	void moverDesconectados();
	void setPosX(int i);
};

#endif /* OBJECTMANAGER_H_ */
