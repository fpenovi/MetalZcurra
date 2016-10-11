//
// Created by franch on 10/10/16.
//

#include "ObjectManager.h"


ObjectManager::ObjectManager() {
	idActual = 1;
}

ObjectManager::~ObjectManager() {
	for (auto kv : objects)
		delete(kv.second);
}

ObjectManager* ObjectManager::getInstance() {
	if (!instancia)
		instancia = new ObjectManager();

	return instancia;
}

void ObjectManager::addObject(Personaje* object) {
	objects[idActual++] = object;
}

Personaje* ObjectManager::getObject(int id) {
	return objects[id];
}

void ObjectManager::registerUser(string username) {

	if (!(tablaUsuarios.find(username) == tablaUsuarios.end()))
		return;

	tablaUsuarios[username] = idActual;
	Personaje* p = new Personaje();
	p->setId(idActual);
	addObject(p);
}

int ObjectManager::getIdByUsername(string username) {
	return tablaUsuarios[username];
}

ObjectManager* ObjectManager::instancia;