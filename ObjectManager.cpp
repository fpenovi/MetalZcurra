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

void ObjectManager::addObject(int id, Personaje* object) {
	objects[id] = object;
}

Personaje* ObjectManager::getObject(int id) {
	return objects[id];
}

void ObjectManager::registerUser(string username) {

	if (!(tablaUsuarios.find(username) == tablaUsuarios.end()))
		return;

	tablaUsuarios[username] = idActual++;
}

int ObjectManager::getIdByUsername(string username) {
	return tablaUsuarios[username];
}

void ObjectManager::crearPersonajes(int cantidad) {

	for (int i = 1 ; i < cantidad+1; i++){
		Personaje* p = new Personaje();
		p->setId(i);
		addObject(i, p);
	}

}

void ObjectManager::enviarPersonajes(int FD) {

	for (auto kv : objects){
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
	objects[id]->setConectado(true);
}

void ObjectManager::desconectarPersonaje(string user){
	int id = getIdByUsername(user);
	objects[id]->setConectado(false);
}

bool ObjectManager::puedoAvanzar() {

	for (auto kv : objects){
		if (kv.second->getPosCamara() == 0 && kv.second->getConectado()){
			return false;
		}
	}
	return true;
}

void ObjectManager::moverCamara(int id){

	for (auto kv : objects){
		if (kv.second->getId() != id && kv.second->getConectado() && kv.second->getPosCamara() != 0){
			kv.second->setPosCamara(kv.second->getPosCamara()-7);
		}
	}
}

void ObjectManager::moverDesconectados() {

	for (auto kv : objects){
		if (kv.second->getPosCamara() == 0 && !(kv.second->getConectado())){
			kv.second->setPosCamara(kv.second->getPosCamara()+7);
		}
	}
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
	msj="";
	for (int i=0; i<4 ; i++ ){
		msj = msj + capas[i];
	}
	msj= msj +"\n";
	const char* mensaje = msj.c_str();
	bytesEscritos = write(FD, mensaje, msj.size());
	if (bytesEscritos < 0)
		perror("ERROR --> No se pudo envair personaje");

}

ObjectManager* ObjectManager::instancia;