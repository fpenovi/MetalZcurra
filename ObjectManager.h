//
// Created by franch on 10/10/16.
//

#ifndef METALZCURRA_OBJECTMANAGER_H
#define METALZCURRA_OBJECTMANAGER_H

#include <string.h>
#include <unistd.h>
#include "GameObject.h"
#include "Personaje.h"
#include <unordered_map>
#include "ProtocoloNuevaVista.h"

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
	~ObjectManager();
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
};


#endif //METALZCURRA_OBJECTMANAGER_H
