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
#include "ParserXML.h"
#include "auxiliares.h"
#include "Mensaje.h"
#include <list>

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
	void enviarEscenario(ParserXML* parser, int FD);
	void conectarPersonaje(string user);
	void desconectarPersonaje(string user);
	void moverDesconectados();
	void setPosX(int i);
	void reinicializarEscenario();
	void enviarNuevoBackground(ParserXML* parser, unordered_map<string, list<Mensaje*>*>* conectadosHash, unordered_map<string, pthread_mutex_t>* mutexesHash, string emisor );
};


#endif //METALZCURRA_OBJECTMANAGER_H
