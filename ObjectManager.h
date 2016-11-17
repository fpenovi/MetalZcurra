//
// Created by franch on 10/10/16.
//

#ifndef METALZCURRA_OBJECTMANAGER_H
#define METALZCURRA_OBJECTMANAGER_H

#include <string.h>
#include <unistd.h>
#include "Personaje.h"
#include <unordered_map>
#include "ProtocoloNuevaVista.h"
#include "ParserXML.h"
#include "auxiliares.h"
#include "Mensaje.h"
#include "Bala.h"
#include <list>
#include "BalasManager.h"
#include "Direccion.h"
#include "Enemigo.h"
#include "EnemigosManager.h"

using namespace std;

class ObjectManager {

private:
	unordered_map<int, Bala*> balas;
	unordered_map<int, Enemigo*> enemigos;
	unordered_map<int, Personaje*> personajes;
	unordered_map<string, int> tablaUsuarios;
	unordered_map<int, Direccion*> direccionBalas;
	static ObjectManager* instancia;
	int idActual;
	ObjectManager();
	int posx;
	BalasManager* balasManager;
	EnemigosManager* enemigosManager;
	unordered_map<string, list<Mensaje*>*>* conectadosHash;
	unordered_map<string, pthread_mutex_t>* mutexesHash;

public:
	~ObjectManager();
	Personaje* getObject(int id);
	void addObject(int id, Personaje* object);
	void registerUser(string username);
	int getIdByUsername(string username);
	Direccion* getDireccionById(int id);
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
	void enviarNuevoBackground(ParserXML* parser, string emisor);

	void setConectadosHash(unordered_map<string, list<Mensaje*>*>* conectadosHash);
	void setMutexesHash(unordered_map<string, pthread_mutex_t>* mutexesHash);
	unordered_map<string, list<Mensaje*>*>* getConectadosHash();
	unordered_map<string, pthread_mutex_t>* getMutexesHash();

	// Balas
	void addBala(int id, Bala* bala);
	void crearBalas(int cantidad);
	void inicializarBala(int idEmisor, int posxEmisor, int posyEmisor);
	void crearBalasManager();
	unordered_map<int, Bala*>* getBalasHash();

	// Enemigos
	void addEnemigo(int id, Enemigo* enemigo);
	void crearEnemigos(int cantidad);
	void inicializarEnemigo();
	void crearEnemigosManager();
	unordered_map<int, Enemigo*>* getEnemigosHash();
};


#endif //METALZCURRA_OBJECTMANAGER_H
