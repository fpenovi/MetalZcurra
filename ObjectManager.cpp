//
// Created by franch on 10/10/16.
//

#include "ObjectManager.h"

ObjectManager::ObjectManager() {
	idActual = 1;
}

ObjectManager::~ObjectManager() {
	for (auto kv : personajes)
		delete kv.second;
	for (auto kv : balas)
		delete kv.second;
	for (auto kv : direccionBalas)
		delete kv.second;
	for (auto kv : enemigos)
		delete kv.second;
	balasManager->Off();
	delete balasManager;
	enemigosManager->Off();
	delete enemigosManager;
}

ObjectManager* ObjectManager::getInstance() {
	if (!instancia)
		instancia = new ObjectManager();

	return instancia;
}

void ObjectManager::addObject(int id, Personaje* object) {
	personajes[id] = object;
}

void ObjectManager::addBala(int id, Bala* bala) {
	balas[id] = bala;
}

void ObjectManager::addEnemigo(int id, Enemigo* enemigo) {
	enemigos[id] = enemigo;
}

Personaje* ObjectManager::getObject(int id) {
	return personajes[id];
}

void ObjectManager::registerUser(string username) {

	if (!(tablaUsuarios.find(username) == tablaUsuarios.end()))
		return;

	direccionBalas[idActual] = new Direccion();
	tablaUsuarios[username] = idActual++;
}

int ObjectManager::getIdByUsername(string username) {
	return tablaUsuarios[username];
}

Direccion* ObjectManager::getDireccionById(int id) {
	return direccionBalas[id];
}

void ObjectManager::crearPersonajes(int cantidad) {

	for (int i = 1 ; i < cantidad+1; i++){
		Personaje* p = new Personaje();
		p->setId(i);
		addObject(i, p);
	}

}

void ObjectManager::crearBalas(int cantidad) {
	for (int i = 1 ; i < cantidad+1; i++){
		Bala* b = new Bala();
		b->setID(i);
		addBala(i, b);
	}
}

void ObjectManager::crearEnemigos(int cantidad) {
	int posicion = 900;
	for (int i = 1; i < cantidad+1; i++){
		Enemigo* enemigo = new Enemigo();
		enemigo->setId(i);
		enemigo->setPosx(posicion);
		addEnemigo(i, enemigo);
		posicion += 200;
	}
}

void ObjectManager::inicializarBala(int idEmisor, int posxEmisor, int posyEmisor) {

	for (auto kv : balas){
		if (!kv.second->existeBala() && (kv.second->getId() <= 50)){
			kv.second->crear(idEmisor, posxEmisor, posyEmisor, getDireccionById(idEmisor));
			return;
		}
	}

}

void ObjectManager::inicializarBalaEnemiga(int posx, int posy) {

	for (auto kv : balas){
		if (!kv.second->existeBala() && (kv.second->getId() > 50)){
			kv.second->crearBalaEnemiga(posx, posy, false);
			return;
		}
	}

}

void ObjectManager::inicializarEnemigo() {

	for (auto kv : enemigos){
		if (!(kv.second->getExiste()) && !(kv.second->estaMuerto())){
			kv.second->crear();
		}
	}
}

void ObjectManager::enviarPersonajes(int FD) {

	for (auto kv : personajes){
		ProtocoloNuevaVista protocolo;

		protocolo.setObject_id(kv.first);
		protocolo.setSpriteId(kv.first);
		protocolo.setX(posx);
		protocolo.setY(kv.second->getPosy());
		protocolo.setCam(kv.second->getPosCamara());
		protocolo.setConectado(kv.second->getConectado());

		string msj = protocolo.toString();
		const char* mensajeChar = msj.c_str();

		cout << msj;

		ssize_t bytesEscritos = write(FD, mensajeChar, msj.size());
		if (bytesEscritos < 0)
			perror("ERROR --> No se pudo envair personaje");

	}

	write(FD, "$\n", 2);

}

int* ObjectManager::getPosX() {
	return &posx;
}

void ObjectManager::conectarPersonaje(string user) {
	int id = getIdByUsername(user);
	personajes[id]->setConectado(true);
}

void ObjectManager::desconectarPersonaje(string user){
	int id = getIdByUsername(user);
	personajes[id]->setConectado(false);
}

bool ObjectManager::puedoAvanzar() {

	for (auto kv : personajes){
		if (kv.second->getPosCamara() == 0 && kv.second->getConectado()){
			return false;
		}
	}
	return true;
}

void ObjectManager::moverCamara(int id){

	for (auto kv : personajes){
		if (kv.second->getId() != id && kv.second->getConectado() && kv.second->getPosCamara() != 0){
			kv.second->setPosCamara(kv.second->getPosCamara()-7);
		}
		if (!(kv.second->getConectado()) && kv.second->getPosCamara() != 7 && kv.second->getPosCamara() != 0){
			kv.second->setPosCamara(kv.second->getPosCamara()-7);
		}
	}

	for (auto kv : enemigos){
		if (kv.second->getExiste())
			kv.second->setPosx(kv.second->getPosx()-7);
	}
}

void ObjectManager::moverDesconectados() {

	for (auto kv : personajes){
		if (kv.second->getPosCamara() == 0 && !(kv.second->getConectado())){
			kv.second->setPosCamara(kv.second->getPosCamara()+7);
		}
	}
}

void ObjectManager::reinicializarEscenario() {
	for (auto kv : personajes){
		kv.second->inicial();
	}
	setPosX(0);
}

void ObjectManager::enviarEscenario(ParserXML *parser, int FD) {
	string tamVentana = parser->TamVentana();
	string tamNivel = parser->tamNivel();
	vector <string> sprites = parser->spritesPlayers();
	vector<string> capas = parser->capas();
	if (tamVentana.length() == 0 || tamNivel.length() == 0 || sprites.empty() || capas.empty() ) {
		parser->setearDefecto();
		tamVentana = parser->TamVentana();
		tamNivel = parser->tamNivel();
		sprites = parser->spritesPlayers();
		capas = parser->capas();
	}
	string msj = tamVentana + tamNivel + "\n";
	const char* mensajeChar = msj.c_str();

	ssize_t bytesEscritos = write(FD, mensajeChar, msj.size());
	if (bytesEscritos < 0)
		perror("ERROR --> No se pudo envair personaje");

	for (int i = 0 ; i < capas.size() ; i++){
		msj = "";
		msj = capas[i] + "\n";
		const char* mensaje = msj.c_str();

		bytesEscritos = write(FD, mensaje, msj.size());

		if (bytesEscritos < 0)
			perror("ERROR --> No se pudo envair personaje");

	}

	write(FD, "$\n", 2);

}

void ObjectManager::enviarNuevoBackground(ParserXML* parser, string emisor) {
	string tamVentana = parser->TamVentana();
	string tamNivel = parser->tamNivel();
	vector <string> sprites = parser->spritesPlayers();
	vector<string> capas = parser->capas();
	if (tamVentana.length() == 0 || tamNivel.length() == 0 || sprites.empty() || capas.empty() ) {
		parser->setearDefecto();
		tamVentana = parser->TamVentana();
		tamNivel = parser->tamNivel();
		sprites = parser->spritesPlayers();
		capas = parser->capas();
	}

	string msj;
	int result;

	for (int i = 0 ; i < capas.size() ; i++){
		msj = "";
		msj = capas[i] + "\n";

		for (auto kv : *conectadosHash) {

			Mensaje *mensajeNuevo = new Mensaje(emisor, kv.first, msj);

			result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
			if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

			kv.second->push_back(mensajeNuevo);

			result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
			if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
		}
	}

	string fin = "$\n";
	for (auto kv : *conectadosHash) {

		Mensaje *mensajeNuevo = new Mensaje(emisor, kv.first, fin);

		result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

		kv.second->push_back(mensajeNuevo);

		result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
	}
}

void ObjectManager::setPosX(int i){
	posx=i;
}

void ObjectManager::crearBalasManager() {

	balasManager = new BalasManager();
	balasManager->On();

}

void ObjectManager::crearEnemigosManager() {

	enemigosManager = new EnemigosManager();
	enemigosManager->On();

}

unordered_map<int, Bala*>* ObjectManager::getBalasHash() {
	return &balas;
}

unordered_map<int, Enemigo*>* ObjectManager::getEnemigosHash() {
	return &enemigos;
}

void ObjectManager::setConectadosHash(unordered_map<string, list<Mensaje*>*>* hash) {
	conectadosHash = hash;
}

void ObjectManager::setMutexesHash(unordered_map<string, pthread_mutex_t>* mutexesHash) {
	this->mutexesHash = mutexesHash;
}

unordered_map<string, list<Mensaje*>*>* ObjectManager::getConectadosHash() {
	return conectadosHash;
}

unordered_map<string, pthread_mutex_t>* ObjectManager::getMutexesHash() {
	return mutexesHash;
}

ObjectManager* ObjectManager::instancia;