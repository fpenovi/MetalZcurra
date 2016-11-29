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
#include "Boss.h"
#include "Bonus.h"
#include "BonusManager.h"
#include "PersonajesManager.h"

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
	vector<string> tamVentana;
	Boss* boss;
	unordered_map<int, Bonus*> bonuses;
	BonusManager* bonusManager;
	int idBonus;
	int cantidadUsuarios;
	PersonajesManager* personajesManager;

public:
	~ObjectManager();
	Personaje* getObject(int id);
	void addObject(int id, Personaje* object);
	void registerUser(string username);
	int getIdByUsername(string username);
	Direccion* getDireccionById(int id);
	static ObjectManager* getInstance();
	void crearPersonajes(int cantidad);
	void enviarPersonajes(int FD, string user);
	int* getPosX();
	bool puedoAvanzar();
	void moverCamara(int id);
	void enviarEscenario(int FD, int cantUsers, int modo);
	void conectarPersonaje(string user);
	void desconectarPersonaje(string user);
	void moverDesconectados();
	void setPosX(int i);
	void reinicializarEscenario();
	void enviarNuevoBackground(string emisor);
	void setTamVentana(vector<string> tamVentana);
	unordered_map<int, Personaje*>* getPersonajesHash();
	unordered_map<string, int>* getTablaUsuarios();

	void setConectadosHash(unordered_map<string, list<Mensaje*>*>* conectadosHash);
	void setMutexesHash(unordered_map<string, pthread_mutex_t>* mutexesHash);
	unordered_map<string, list<Mensaje*>*>* getConectadosHash();
	unordered_map<string, pthread_mutex_t>* getMutexesHash();
	void pausarManagers();
	void reanudarManagers();

	// Balas
	void addBala(int id, Bala* bala);
	void crearBalas(int cantidad);
	void inicializarBala(int idEmisor, int posxEmisor, int posyEmisor, int tipoArma);
	void inicializarBalaEnemiga(int posx, int posy);
	void crearBalasManager();
	unordered_map<int, Bala*>* getBalasHash();
	void inicializarBalaBoss(int posx, int posy);

	// Enemigos
	void addEnemigo(int id, Enemigo* enemigo);
	void crearEnemigos(vector<Enemigo*> enemigos);
	void inicializarEnemigo();
	void crearEnemigosManager();
	unordered_map<int, Enemigo*>* getEnemigosHash();
	void liberarEnemigos();
	Boss* getBoss();
	void setBoss(Boss* boss);

	// Bonuses
	unordered_map<int, Bonus*>* getBonusesHash();
	void setBonuses(vector<Bonus*> bonuses);
	void addBonus(int id, Bonus* bonus);
	void crearBonusManager();
	void agregarDropeable(Bonus* dropeable);
	int getIdBonus();
	void setIdBonus(int aux);
	void killAll();
	void reiniciarBonuses();

	void handleImpacto(Personaje *personaje, Bala* bala);
	int getCantidadUsuarios();
	void crearPersonajesManager(int cantUsers);
	void enviarPuntaje(int idPj);
	bool existeDireccion(int id);
};


#endif //METALZCURRA_OBJECTMANAGER_H
