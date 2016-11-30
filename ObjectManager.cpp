//
// Created by franch on 10/10/16.
//

#include "ObjectManager.h"
#include "NivelManager.h"

#define GUN 0
#define HMGUN 1
#define SHOTGUN 2
#define RLAUNCHER 3

ObjectManager::ObjectManager() {
	idActual = 1;
	idBonus = 1;
}

ObjectManager::~ObjectManager() {
	balasManager->Off();
	delete balasManager;
	enemigosManager->Off();
	delete enemigosManager;
	bonusManager->Off();
	delete bonusManager;
	personajesManager->Off();
	delete personajesManager;
	gravedadManager->Off();
	delete gravedadManager;

	for (auto kv : personajes)
		delete kv.second;
	for (auto kv : balas)
		delete kv.second;
	for (auto kv : direccionBalas)
		delete kv.second;
	for (auto kv : enemigos)
		delete kv.second;
	for (auto kv : bonuses)
		delete kv.second;
	delete boss;
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

void ObjectManager::addBonus(int id, Bonus* bonus) {
	bonuses[id] = bonus;
}

Personaje* ObjectManager::getObject(int id) {
	return personajes[id];
}

void ObjectManager::registerUser(string username) {

	if (!(tablaUsuarios.find(username) == tablaUsuarios.end()))
		return;

	if (!existeDireccion(idActual))
		direccionBalas[idActual] = new Direccion();

	tablaUsuarios[username] = idActual++;
}

bool ObjectManager::existeDireccion(int id){
	auto it = direccionBalas.find(id);
	return it != direccionBalas.end();
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

void ObjectManager::crearEnemigos(vector<Enemigo*> enemigos) {
	for (int i = 0; i < enemigos.size(); i++){
		Enemigo* enemigo = enemigos[i];
		enemigo->setId(i + 1);
		addEnemigo(i+1, enemigo);
	}
}

void ObjectManager::setBonuses(vector<Bonus*> bonuses) {
	cout << "CREO NUEVOS BONUSES" << endl;
	for (int i = 0; i < bonuses.size(); i++){
		Bonus* bonus = bonuses[i];
		bonus->setId(idBonus);
		addBonus(idBonus, bonus);
		idBonus++;
	}
}

void ObjectManager::inicializarBala(int idEmisor, int posxEmisor, int posyEmisor, int tipoArma) {

	if (tipoArma == GUN) {
		for (auto kv : balas) {
			if (!kv.second->existeBala() && (kv.second->getId() <= 50)) {
				kv.second->crear(idEmisor, posxEmisor, posyEmisor, getDireccionById(idEmisor), 13, 13);
				kv.second->setDanio(10);
				kv.second->setPuntos(5);
				return;
			}
		}
	}
	else if (tipoArma == HMGUN){
		int offsetY = - 15;
		int offsetX = 15;
		for (int i = 0; i < 3 ; i++) {
			for (auto kv : balas) {
				if (!kv.second->existeBala() && (kv.second->getId() > 100) && (kv.second->getId() < 151)) {
					kv.second->crear(idEmisor, posxEmisor + offsetX, posyEmisor + offsetY, getDireccionById(idEmisor), 64, 11);
					kv.second->setDanio(20);
					kv.second->setPuntos(15);
					break;
				}
			}
			offsetY += 15;
			offsetX -= 15;
		}
	}
	else if (tipoArma == SHOTGUN){
		for (auto kv : balas) {
			if (!kv.second->existeBala() && (kv.second->getId() > 150) && (kv.second->getId() < 201)) {
				kv.second->crear(idEmisor, posxEmisor, posyEmisor, getDireccionById(idEmisor), 299, 137);
				kv.second->setDanio(50);
				kv.second->setPuntos(35);
				kv.second->setShotgun(true);
				return;
			}
		}
	}
	else if (tipoArma == RLAUNCHER){
		for (auto kv : balas) {
			if (!kv.second->existeBala() && (kv.second->getId() > 200) && (kv.second->getId() < 251)) {
				kv.second->crear(idEmisor, posxEmisor, posyEmisor, getDireccionById(idEmisor), 107, 17);
				kv.second->setDanio(80);
				kv.second->setPuntos(40);
				kv.second->setRlauncher(true);
				return;
			}
		}
	}
}

void ObjectManager::inicializarBalaEnemiga(int posx, int posy) {

	for (auto kv : balas){
		if (!kv.second->existeBala() && (kv.second->getId() > 50) && (kv.second->getId() < 101)){
			kv.second->crearBalaEnemiga(posx, posy, false, 42, 7);
			return;
		}
	}

}

void ObjectManager::inicializarBalaBoss(int posx, int posy){

	for (auto kv : balas){
		if (!kv.second->existeBala() && (kv.second->getId() > 250)){
			kv.second->crearBalaBoss(posx, posy, 26, 54);
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

void ObjectManager::enviarPersonajes(int FD, string user) {

	int idUser = getIdByUsername(user);

	for (auto kv : personajes){
		ProtocoloNuevaVista protocolo;

		protocolo.setObject_id(kv.first);
		protocolo.setSpriteId(kv.second->estaVivo());
		protocolo.setX(posx);
		protocolo.setY(kv.second->getPosy());
		protocolo.setCam(kv.second->getPosCamara());
		protocolo.setConectado(kv.second->getConectado());
		protocolo.setDuenio(idUser);

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

	for (auto kv : enemigos)
		if (kv.second->getExiste())
			kv.second->setPosx(kv.second->getPosx()-7);

	for (auto kv : bonuses)
		if (kv.second->getExiste())
			kv.second->setPosicion(kv.second->getPosx()-7, kv.second->getPosy());

	NivelManager::getInstance()->moverPlataformas();
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

void ObjectManager::enviarEscenario(int FD, int cantUsers, int modo) {

	string msj = tamVentana[0] + "$" + tamVentana[1] + "$" + to_string(cantUsers) + "$" + to_string(modo) +  "\n";
	const char* mensajeChar = msj.c_str();

	ssize_t bytesEscritos = write(FD, mensajeChar, msj.size());
	if (bytesEscritos < 0)
		perror("ERROR --> No se pudo envair tam");

	vector<string> capas = NivelManager::getInstance()->getCapas();

	for (int i = 0 ; i < capas.size() ; i++){
		msj = "";
		msj = capas[i] + "\n";
		const char* mensaje = msj.c_str();

		bytesEscritos = write(FD, mensaje, msj.size());

		if (bytesEscritos < 0)
			perror("ERROR --> No se pudo envair capa");

	}

	write(FD, "$\n", 2);

	for (auto kv : bonuses){
		msj = "";
		msj = to_string(kv.second->getId()) + "$" + to_string(kv.second->getTipoArma()) + "\n";
		const char* mensaje = msj.c_str();

		bytesEscritos = write(FD, mensaje, msj.size());

		if (bytesEscritos < 0)
			perror("ERROR --> No se pudo envair bonus");
	}
	write(FD, "$\n", 2);

	msj = "";
	msj = to_string(enemigos.size()) + "\n";
	const char* mensaje = msj.c_str();

	bytesEscritos = write(FD, mensaje, msj.size());

	if (bytesEscritos < 0)
		perror("ERROR --> No se pudo envair bonus");
}

void ObjectManager::enviarNuevoBackground(string emisor) {
	cout << "ENVIO NUEVO BACKGROUND" << endl;

	string msj;
	int result;

	vector<string> capas = NivelManager::getInstance()->getCapas();

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

void ObjectManager::crearBonusManager() {

	bonusManager = new BonusManager();
	bonusManager->On();
}

void ObjectManager::crearPersonajesManager(int cantUsers) {
	cantidadUsuarios = cantUsers;

	personajesManager = new PersonajesManager();
	personajesManager->On();
}

void ObjectManager::crearGravedadManager() {

	gravedadManager = new GravedadManager();
	gravedadManager->On();
}

Boss* ObjectManager::getBoss() {
	return this->boss;
}

void ObjectManager::setBoss(Boss *boss) {
	this->boss = boss;
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

void ObjectManager::setTamVentana(vector<string> tamVentana) {
	this->tamVentana = tamVentana;
}

void ObjectManager::liberarEnemigos() {
	cout << "LIBERO ENEMIGOS" << endl;
	for (auto kv : enemigos)
		delete kv.second;
	enemigos.clear();
	delete boss;
	boss = NULL;
}

void ObjectManager::reiniciarBonuses() {
	cout << "REINICIO BONUS" << endl;
	for (auto kv : bonuses)
		if (kv.second != NULL) delete kv.second;
	bonuses.clear();
	idBonus = 1;
}

unordered_map<int, Bonus *>* ObjectManager::getBonusesHash() {
	return &bonuses;
}

unordered_map<int, Personaje *> *ObjectManager::getPersonajesHash() {
	return &personajes;
}

int ObjectManager::getIdBonus() {
	return idBonus;
}

void ObjectManager::setIdBonus(int aux) {
	idBonus = aux;
}

void ObjectManager::agregarDropeable(Bonus* dropeable) {
	dropeable->setId(idBonus);
	addBonus(idBonus, dropeable);

	ProtocoloVistaUpdate update;

	update.setTipoObjeto(5);
	update.setEstado(0);
	update.setX(0);
	update.setY(0);
	update.setObject_id(idBonus);
	update.setPosCamara(0);
	update.setConectado(0);
	update.setSpriteIndex(1);
	update.setApuntando(0);
	update.setSaltando(dropeable->getTipoDropeable());
	update.setPuntaje(0);

	int result;
	string mensaje = update.toString();

	for (auto kv : *conectadosHash) {

		Mensaje* mensajeNuevo = new Mensaje("Server", kv.first, mensaje);

		result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

		kv.second->push_back(mensajeNuevo);

		result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
	}
	idBonus++;
}

void ObjectManager::handleImpacto(Personaje* personaje, Bala* bala){
	ProtocoloVistaUpdate update;

	update.setTipoObjeto(7);
	update.setEstado(personaje->estaVivo());
	update.setX(0);
	update.setY(0);
	update.setObject_id(personaje->getId());
	update.setPosCamara(0);
	update.setConectado(0);
	update.setSpriteIndex(0);
	update.setApuntando(0);
	update.setSaltando(0);

	if (bala->getId() > 250) update.setPuntaje(bala->getId());
	else update.setPuntaje(0);


	int result;
	string mensaje = update.toString();

	for (auto kv : *conectadosHash) {

		Mensaje* mensajeNuevo = new Mensaje("Server", kv.first, mensaje);

		result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

		kv.second->push_back(mensajeNuevo);

		result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
	}
}

void ObjectManager::killAll(){
	for (auto kv : enemigos){
		if (kv.second->getExiste())
			kv.second->morir();
	}
}

void ObjectManager::pausarManagers() {
	balasManager->Pause();
	enemigosManager->Pause();
	bonusManager->Pause();
	personajesManager->Pause();
	gravedadManager->Pause();
}

void ObjectManager::reanudarManagers() {
	balasManager->Resume();
	enemigosManager->Resume();
	bonusManager->Resume();
	personajesManager->Resume();
	gravedadManager->Resume();
}

int ObjectManager::getCantidadUsuarios() {
	return cantidadUsuarios;
}

unordered_map<string, int>* ObjectManager::getTablaUsuarios() {
	return &tablaUsuarios;
}

void ObjectManager::enviarPuntaje(int idPj){
	Personaje* pj = getObject(idPj);

	ProtocoloVistaUpdate update;

	update.setTipoObjeto(9);
	update.setEstado(0);
	update.setX(0);
	update.setY(0);
	update.setObject_id(pj->getId());
	update.setPosCamara(0);
	update.setConectado(0);
	update.setSpriteIndex(0);
	update.setApuntando(0);
	update.setSaltando(0);
	update.setPuntaje(pj->getPuntaje());


	int result;
	string mensaje = update.toString();

	for (auto kv : *conectadosHash) {

		Mensaje* mensajeNuevo = new Mensaje("Server", kv.first, mensaje);

		result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

		kv.second->push_back(mensajeNuevo);

		result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
		if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
	}
}

void ObjectManager::modoInvencible(){
	for (auto kv : personajes){
		if (!kv.second->getModoInvencible()){
			kv.second->setModoInvencible(true);
			kv.second->setVida(100000);
		}
		else {
			kv.second->setModoInvencible(false);
			kv.second->setVida(5);
		}
	}
}

ObjectManager* ObjectManager::instancia;

void ObjectManager::removeBonus(int id) {
	bonuses.erase(id);
}
