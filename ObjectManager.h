//
// Created by franch on 10/10/16.
//

#ifndef METALZCURRA_OBJECTMANAGER_H
#define METALZCURRA_OBJECTMANAGER_H


#include "GameObject.h"
#include "Personaje.h"
#include <unordered_map>


using namespace std;


class ObjectManager {

private:
	unordered_map<int, Personaje*> objects;
	unordered_map<string, int> tablaUsuarios;
	static ObjectManager* instancia;
	int idActual;
	ObjectManager();

public:
	~ObjectManager();
	Personaje* getObject(int id);
	void addObject(Personaje* object);
	void registerUser(string username);
	int getIdByUsername(string username);
	static ObjectManager* getInstance();
};


#endif //METALZCURRA_OBJECTMANAGER_H
