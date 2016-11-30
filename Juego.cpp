#include <chrono>
#include "Juego.h"
#include "ProtocoloComando.h"
#include "ProtocoloNuevaVista.h"
#include "VistaRshobu.h"
#include "ProtocoloVistaUpdate.h"
#include "VistaDaiManji.h"

using namespace std;
using namespace chrono;


Juego::Juego() {
	renderizador = NULL;
	ventana = NULL;
	lastKeyPressed = 0;
	nivelActual = 1;
}

void Juego::close() {
	//Free loaded images
	delete puntajes;

	for (auto kv : vistasPersonajes)
		delete kv.second;

	for (auto kv : vistasBalas)
		delete kv.second;

	for (auto kv : vistasEnemigos)
		delete kv.second;

	for (auto kv: visitasBonuses)
		delete kv.second;

	for (auto kv : vistasBoss)
		delete kv.second;

	for (auto text : TEXTURAS_ENEMIGOS)
		delete text;

	//Destroy window
	SDL_DestroyRenderer( renderizador );
	SDL_DestroyWindow( ventana );
	ventana = NULL;
	renderizador = NULL;
	delete TEXTURA_BALA;
	delete TEXTURA_BALA_ENEMIGA;
	delete TEXTURA_BALA_HMGUN;
	delete TEXTURA_BALA_SHOTGUN;
	delete TEXTURA_BALA_RLAUNCHER;
	delete TEXTURA_HMGUN;
	delete TEXTURA_SGUN;
	delete TEXTURA_RLAUNCHER;
	delete TEXTURA_KILLALL;
	delete TEXTURA_RECOVER;
	delete TEXTURA_BALA_RSHOBU;
	delete fondo;
	delete grayOutHandler;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Juego::presentacion(){
	//ESTE METODO ES EL QUE PIDE IP PUERTO Y NOMBRE
	neoGeo = new Textura(renderizador);

	Uint32 start = 0;
	textoip = new Texto("IP: ", renderizador);
	textoip->cargarTitulo();
	textopuerto = new Texto("Puerto: ", renderizador);
	textopuerto->cargarTitulo();
	textonombre = new Texto("Nombre: ", renderizador);
	textonombre->cargarTitulo();

	SDL_Event e;
	bool quit = false;

	if( !neoGeo->cargarImagen("imag/entrada/neogeo.png"))
	{
		printf( "Failed to load presentacion!\n" );
	}
	else{

		while(!quit)
		{
			SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0 );
			SDL_RenderClear( renderizador );
			neoGeo->render( 100, 50,NULL, 0.0, NULL, SDL_FLIP_NONE);
			quit=textoip->pedir();
			SDL_RenderPresent( renderizador );
		}
		ip = textoip->getTexto();
		quit=false;
		while(!quit)
		{
			SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0 );
			SDL_RenderClear( renderizador );
			neoGeo->render( 100, 50,NULL, 0.0, NULL, SDL_FLIP_NONE);
			quit=textopuerto->pedir();
			SDL_RenderPresent( renderizador );
		}
		puerto = textopuerto->getTexto();
		quit=false;
		while(!quit)
		{
			SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0 );
			SDL_RenderClear( renderizador );
			neoGeo->render( 100, 50,NULL, 0.0, NULL, SDL_FLIP_NONE);
			quit=textonombre->pedir();
			SDL_RenderPresent( renderizador );
		}
		nombre = textonombre->getTexto();
		quit=false;

		while( !quit )
		{
			//MANEJA LA COLA DE EVENTOS
			while( SDL_PollEvent( &e ) != 0 )
			{
				if( e.type == SDL_QUIT ){
					quit = true;
				}
				if( e.type == SDL_KEYDOWN )
				{
					if (e.key.keysym.sym == SDLK_RETURN) quit = true;
				}
			}
			SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0 );
			SDL_RenderClear( renderizador );

			//Render background
			neoGeo->render( 100, 50,NULL, 0.0, NULL, SDL_FLIP_NONE);

			SDL_RenderPresent( renderizador );
		}
	}
}

void Juego::liberarPresentacion(){
	delete neoGeo;
	delete textoip;
	delete textopuerto;
	delete textonombre;
}

void Juego::cargarEntrada(){
	fondoInicial = new Textura(renderizador);
	TEXTURA_EXPLOSION1 = new Textura(renderizador);
	TEXTURA_EXPLOSION2 = new Textura(renderizador);
	TEXTURA_EXPLOSION3 = new Textura(renderizador);
	TEXTURA_METAL = new Textura(renderizador);

	//Loading success flag
	int i;

	//Load sprite sheet texture
	if( !TEXTURA_EXPLOSION1->cargarImagen( "imag/entrada/entrada1.png") )
	{
		printf( "Fallo sprite EXPLOSION1\n" );
	}
	else
	{
		for (i = 0;i<10;i++){
			spriteEntrada1[ i ].x = i*800;
			spriteEntrada1[ i ].y = 0;
			spriteEntrada1[ i ].w = 800;
			spriteEntrada1[ i ].h = 600;
		}
	}

	//Load sprite sheet texture
	if( !TEXTURA_EXPLOSION2->cargarImagen( "imag/entrada/entrada2.png") )
	{
		printf( "Fallo sprite EXPLOSION2\n" );
	}
	else
	{
		for (i = 0;i<10;i++){
			spriteEntrada2[ i ].x = i*800;
			spriteEntrada2[ i ].y = 0;
			spriteEntrada2[ i ].w = 800;
			spriteEntrada2[ i ].h = 600;
		}
	}
	if( !TEXTURA_EXPLOSION3->cargarImagen( "imag/entrada/entrada3.png") )
	{
		printf( "Fallo sprite EXPLOSION3\n" );
	}
	else
	{
		for (i = 0;i<10;i++){
			spriteEntrada3[ i ].x = i*800;
			spriteEntrada3[ i ].y = 0;
			spriteEntrada3[ i ].w = 800;
			spriteEntrada3[ i ].h = 600;
		}
	}

	if (!fondoInicial->cargarImagen("imag/entrada/fondito.png")){
		cout << "error cargando fondito"<<endl;
	}
	if (!TEXTURA_METAL->cargarImagen("imag/entrada/MetalSlug.png")){
		cout << "error cargando metal slug"<<endl;
	}
	else
	{
		for (i = 0;i<7;i++){
			spriteMetal[ i ].x = i*640;
			spriteMetal[ i ].y = 0;
			spriteMetal[ i ].w = 640;
			spriteMetal[ i ].h = 377;
		}
	}
}

void Juego::entrada(){
	//ESTE METODO HACE LA PRESENTACION Y LA SALA DE ESPERA
	//PRIMERO HAY QUE LLAMAR A CARGARENTRADA()

	SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderizador );
	int frame=0;
	int contador=0;
	while (frame < 10){
		//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( renderizador );
		fondoInicial->render(0,0,NULL, 0.0, NULL, SDL_FLIP_NONE);
		SDL_Rect* currentClip = &spriteEntrada1[ frame];
		TEXTURA_EXPLOSION1->render( 0, 0,currentClip, 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent( renderizador );
		if (contador % 5 == 0) frame++;
		contador++;
	}
	frame=0;
	while (frame < 10){
		//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( renderizador );
		fondoInicial->render(0,0,NULL, 0.0, NULL, SDL_FLIP_NONE);
		SDL_Rect* currentClip = &spriteEntrada2[ frame ];
		TEXTURA_EXPLOSION2->render( 0, 0, currentClip, 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent( renderizador );
		if (contador % 5 == 0) frame++;
		contador++;
	}
	frame=0;
	while (frame < 10){
		//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( renderizador );
		fondoInicial->render(0,0,NULL, 0.0, NULL, SDL_FLIP_NONE);
		SDL_Rect* currentClip = &spriteEntrada3[ frame ];
		TEXTURA_EXPLOSION3->render( 0, 0, currentClip, 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent( renderizador );
		if (contador%5 == 0) frame++;
		contador++;
	}

	frame=0;
	while (frame < 7){
		//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( renderizador );
		fondoInicial->render(0,0,NULL, 0.0, NULL, SDL_FLIP_NONE);
		SDL_Rect* currentClip = &spriteMetal[ frame ];
		TEXTURA_METAL->render( 150, 0, currentClip, 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent( renderizador );
		if (contador % 4 == 0) frame++;
		contador++;
	}

	bool quit = false;
	SDL_Event e;
	esperandoTexto = new Texto("Esperando jugadores ", renderizador);
	esperandoTexto->cargarTitulo();
	puntitos = new Texto(".", renderizador);
	puntitos->cargarTitulo();

	int vueltas=0;

	while (!quit){
		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
				continue;
			}
			else if( e.type == SDL_KEYDOWN )
			{
				if (e.key.keysym.sym == SDLK_RETURN){
					quit = true;
					continue;
				}
			}
		}

		SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( renderizador );
		fondoInicial->render(0,0,NULL, 0.0, NULL, SDL_FLIP_NONE);
		SDL_Rect* currentClip = &spriteMetal[ frame-1 ];
		TEXTURA_METAL->render( 115, 0, currentClip, 0.0, NULL, SDL_FLIP_NONE);
		//esperando.render(180,400);
		esperandoTexto->renderTitulo(180,400);
		//puntitos.renderTitulo(550,400);
		if (vueltas % 100 <= 33) puntitos->renderTitulo(550,400);
		else if (vueltas % 100<= 66){
			puntitos->renderTitulo(550,400);
			puntitos->renderTitulo(560,400);
		}
		else {
			puntitos->renderTitulo(550,400);
			puntitos->renderTitulo(560,400);
			puntitos->renderTitulo(570,400);
		}
		SDL_RenderPresent( renderizador );
		vueltas++;
	}

}

void Juego::liberarEntrada(){
	delete esperandoTexto;
	delete puntitos;
	delete fondoInicial;
	delete TEXTURA_EXPLOSION1;
	delete TEXTURA_EXPLOSION2;
	delete TEXTURA_EXPLOSION3;
	delete TEXTURA_METAL;
}

bool Juego::iniciar() {
	//flag
	bool success = true;

	//SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "NO SE INICIALIZO SDL! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Set texture filtering to linear
		if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//VENTANA
		ventana = SDL_CreateWindow( "Metal Slug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN );
		if ( ventana == NULL ) {
			printf( "NO SE PUDO CREAR LA VENTANA! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}

		else {
			//Create vsynced renderer for window
			renderizador = SDL_CreateRenderer( ventana, -1, SDL_RENDERER_ACCELERATED);
			if ( renderizador == NULL ) {
				printf( "NO SE PUDO HACER EL RENDER! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}

			else {
				//DRAWCOLOR ASI PONE TODO EN BLANCO
				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "NO SE PUDO INICIARLIZAR LA IMAGEN! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 ) {
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void Juego::transparentar(Uint8 alpha) {
	this->transparenciaActual = alpha;

	this->bossActual->transparentar(alpha);
	this->fondo->transparentar(alpha);

	/*for (auto kv : this->vistasPersonajes)
		kv.second->transparentar(alpha);*/

	/*for (auto kv : this->vistasBalas)
		kv.second->transparentar(alpha);*/

	for (auto kv : this->visitasBonuses)
		kv.second->transparentar(alpha);

	for (auto kv : this->vistasEnemigos)
		kv.second->transparentar(alpha);
}

SDL_Renderer* Juego::getRenderer() {
	return renderizador;
}

SDL_Window* Juego::getVentana() {
	return ventana;
}

SDL_Rect* Juego::getCamera(){
	return camera;
}

Cliente* Juego::getCliente(){
	return cliente;
}

Background* Juego::getBackground(){
	return fondo;
}

int Juego::getPosX(){
	return posFondo;
}

VistaPersonaje* Juego::getPersonajeById(int id){
	return vistasPersonajes[id];
}

VistaBala* Juego::getBalaById(int id){
	return vistasBalas[id];
}

VistaEnemigo* Juego::getEnemigoById(int id){
	return vistasEnemigos[id];
}

VistaBonus* Juego::getBonusById(int id){
	return visitasBonuses[id];
}

VistaBoss* Juego::getBossById(int id){
	return vistasBoss[id];
}

string Juego::getIp(){
	return ip;
}

string Juego::getPuerto(){
	return puerto;
}

string Juego::getNombre(){
	return nombre;
}

void Juego::setPosX(int x){
	this->posFondo = x;
}

void Juego::setBackground(Background* fondo){
	this->fondo = fondo;
}

void Juego::setCliente(Cliente* client){
	cliente = client;
}

void Juego::setCamara(SDL_Rect* camara){
	camera = camara;
}

void Juego::addPersonaje(int id, VistaPersonaje* pj){
	vistasPersonajes[id] = pj;
}

void Juego::addBala(int id, VistaBala* bala){
	vistasBalas[id] = bala;
}

void Juego::addEnemigo(int id, VistaEnemigo* enemigo){
	vistasEnemigos[id] = enemigo;
}

void Juego::addBonus(int id, VistaBonus* bonus){
	visitasBonuses[id] = bonus;
}

void Juego::addBoss(int id, VistaBoss* boss){
	vistasBoss[id] = boss;
}

void Juego::addBalaViva(int id, VistaBala* bala){
	vistasBalasVivas[id] = bala;
}

void Juego::addEnemigoVivo(int id, VistaEnemigo* enemigo){
	vistasEnemigosVivos[id] = enemigo;
}

void Juego::removeBala(int id){
	vistasBalasVivas.erase(id);
}

void Juego::removeEnemigo(int id){
	vistasEnemigosVivos.erase(id);
}

bool Juego::existeBala(int id){
	auto it = vistasBalasVivas.find(id);
	return it != vistasBalasVivas.end();
}

bool Juego::existeEnemigo(int id){
	auto it = vistasEnemigosVivos.find(id);
	return it != vistasEnemigosVivos.end();
}

void Juego::conectar(){
	cliente->conectar(nombre);
}

void Juego::handleEvent( SDL_Event& e) {

	VistaPersonaje* miPj = getPersonajeById(miId);
	if (!miPj->getExiste())
		return;

	ProtocoloComando comando;
	string msj;

	// Si toco una tecla por la primera vez
	if ( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {

		switch( e.key.keysym.sym ) {

			case SDLK_LEFT:
				comando.setScancode(SDLK_LEFT);
				comando.setType(1);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_RIGHT:
				comando.setScancode(SDLK_RIGHT);
				comando.setType(1);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_UP:
				comando.setScancode(SDLK_UP);
				comando.setType(1);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_DOWN:
				comando.setScancode(SDLK_DOWN);
				comando.setType(1);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_x:
				comando.setScancode(SDLK_x);
				comando.setType(1);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_RETURN:
				cout << "TOCO ENTER" << endl;
				if (puedePasarDeNivel()) {
					comando.setScancode(SDLK_RETURN);
					comando.setType(1);
					msj = comando.toString();
					cliente->enviarAusuario("TODOS", msj, false);
				}
				break;

			case SDLK_z:
				comando.setScancode(SDLK_z);
				comando.setType(1);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_i:
				comando.setScancode(SDLK_i);
				comando.setType(1);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;
		}
	}

	// Si suelto la tecla
	else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {

		switch( e.key.keysym.sym ) {

			case SDLK_LEFT:
				comando.setScancode(SDLK_LEFT);
				comando.setType(0);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_RIGHT:
				comando.setScancode(SDLK_RIGHT);
				comando.setType(0);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_UP:
				comando.setScancode(SDLK_UP);
				comando.setType(0);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_DOWN:
				comando.setScancode(SDLK_DOWN);
				comando.setType(0);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_x:
				comando.setScancode(SDLK_x);
				comando.setType(0);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;

			case SDLK_RETURN:
				break;

			case SDLK_z:
				comando.setScancode(SDLK_z);
				comando.setType(0);
				msj = comando.toString();
				cliente->enviarAusuario("TODOS", msj, false);
				break;
		}
	}
}

void Juego::moverCamara(int id){

	for (auto kv : vistasPersonajes){
		if (kv.second->getId() != id){

			if (kv.second->getConectado() && kv.second->getPosCamara() != 0) {
				kv.second->setPosCamara(kv.second->getPosCamara() - 7);
			}
			else if (kv.second->getPosCamara() == 0 && !(kv.second->getConectado())){
				kv.second->setPosCamara(kv.second->getPosCamara()+7);
			}
			if (!(kv.second->getConectado()) && kv.second->getPosCamara() != 7 && kv.second->getPosCamara() != 0){
				kv.second->setPosCamara(kv.second->getPosCamara()-7);
			}
		}
	}

	for (auto kv : vistasEnemigosVivos)
		if (kv.second->existeEnemigo())
			kv.second->setPosX(kv.second->getPosx()-7);

	for (auto kv : visitasBonuses)
		if (kv.second->getExiste())
			kv.second->setPosx(kv.second->getPosx()-7);
}

void Juego::renderizar() {
	for (auto kv : visitasBonuses)
		kv.second->render();

	for (auto kv : vistasEnemigosVivos)
		kv.second->render();

	bossActual->render();

	for (auto kv : vistasPersonajes) {
		if (kv.second->getId() != miId) {

			if (!(kv.second->getConectado()) && !(kv.second->getGris())) kv.second->ponerTexturaGris();

			else if (kv.second->getConectado() && kv.second->getGris()) kv.second->sacarTexturaGris();

			kv.second->render();
		}
	}

	VistaPersonaje* miPj = vistasPersonajes[miId];
	miPj->render();

	for (auto kv : vistasBalasVivas)
		kv.second->render();

	puntajes->render();

	if (!bossActual->estaVivo())
		puntajes->mostrarResumen(this);
}

int Juego::getPersonajeMasMovido(){
	int aux = 0;
	for (auto kv : vistasPersonajes){
		int x = kv.second->getX();
		if (x > aux) aux = x;
	}
	return aux;
}

void Juego::jugadoresInicio(){
	for ( auto kv : vistasPersonajes){
		kv.second->setPosCamara(0);
		kv.second->setPosx(0);
		kv.second->setPosy(465);
		kv.second->setSeMovio(false);
		kv.second->setDerecha(true);
	}
	setPosX(0);

	for (auto kv : vistasEnemigos)
		kv.second->revivir();

	for (auto kv : visitasBonuses)
		kv.second->reiniciar();

	vistasBalasVivas.clear();
	vistasEnemigosVivos.clear();

	seleccionarBossSiguiente();

	transparentar(255);
	puntajes->resetTransparenciaActual();
}

void Juego::salaDeEspera(){
	cout << "ESPERANDO A TODOS LOS USUARIOS" << endl;
	string stream = cliente->recibir_nueva_vista();
}

void Juego::recibirEscenario(){
	string stream = cliente->recibir_nueva_vista();

	cout << stream;

	string ventanaAncho = "";
	string ventanaAlto = "";
	string cantUsers = "";
	string gameMode = "";

	string* variables[] = {&ventanaAncho, &ventanaAlto, &cantUsers, &gameMode};

	int j = 0;

	for (int i=0; i<stream.size() - 1; i++) {

		char actual = stream[i];

		if (actual == '$') {
			j++;
			continue;
		}

		*(variables[j]) += actual;
	}

	screenWidth = stoi(ventanaAncho);
	screenHeight = stoi(ventanaAlto);
	cantidadUsuarios = stoi(cantUsers);
	modoJuego = stoi(gameMode);
}

void Juego::recibirCapas(){
	cout <<"RECIBIENDO BACKGROUND"<< endl;

	while (true) {

		string stream = cliente->recibir_nueva_vista();

		if (stream == "$\n") break;

		string path = "";

		string* variables[] = {&path};

		int j = 0;

		for (int i=0; i<stream.size() - 1; i++) {

			char actual = stream[i];

			if (actual == '$') {
				j++;
				continue;
			}

			*(variables[j]) += actual;
		}

		char *path_c = new char[path.length() + 1];
		strcpy(path_c, path.c_str());

		if(!(fondo->agregar(path_c)) ){
			printf( "Failed to load media!\n" );
		}

	}
}

void Juego::recibirPersonajes(){

	while (true) {

		string nuevaVista = cliente->recibir_nueva_vista();

		if (nuevaVista == "$\n") break;

		cout << "NUEVO PERSONAJE: " << nuevaVista << endl;

		int id, sprite, posx, posy, cam, conectado, idUser;

		ProtocoloNuevaVista::parse(nuevaVista, &id, &sprite, &posx, &posy, &cam, &conectado, &idUser);

		VistaPersonaje *personaje = new VistaPersonaje(getRenderer(), id, modoJuego);

		setPosX(posx);
		personaje->setExiste(sprite);
		personaje->setId(id);
		personaje->setPosCamara(cam);
		personaje->setPosx(posx);
		personaje->setPosy(posy);
		personaje->setSeMovio(false);
		personaje->setConectado(conectado);
		addPersonaje(id, personaje);

		miId = idUser;

		if (!personaje->cargarImagen()) {
			printf("Failed to load media!\n");
			exit(1);
		}

	}

}

void Juego::recibirNuevoBackground(){

	setPuedePasarDeNivel(false);

	delete fondo;

	// Seteo el fondo
	Background* fondo = new Background(getRenderer());
	setBackground(fondo);
	recibirNuevasCapas();
	fondo->prepararEscenario();

	transparentar(0);

	SDL_RenderClear( getRenderer() );
	fondo->render(getPosX());
	renderizar();
	SDL_RenderPresent( getRenderer() );

	milliseconds velocidad(20);

	if (grayOutHandler != NULL) {
		delete grayOutHandler;
		grayOutHandler = NULL;
	}

	grayOutHandler = new GrayOutHandler(this, &transparenciaActual, &LIMITE_SUPERIOR_TRANSPARENCIA_NEGATIVA, velocidad, 1, &isRunning);
	grayOutHandler->doWork();
}

void Juego::recibirNuevasCapas(){
	cout <<"RECIBIENDO BACKGROUND"<< endl;

	while (true) {

		string stream = cliente->desencolar_vista();

		if (stream == "$\n") break;

		string path = "";
		string ancho = "";
		string zindex = "";

		string* variables[] = {&path, &ancho, &zindex};

		int j = 0;

		for (int i=0; i<stream.size() - 1; i++) {

			char actual = stream[i];

			if (actual == '$') {
				j++;
				continue;
			}

			*(variables[j]) += actual;
		}


		char *path_c = new char[path.length() + 1];
		strcpy(path_c, path.c_str());

		if(!(fondo->agregar(path_c)) ){
			printf( "Failed to load media!\n" );
		}

	}
}

void Juego::crearBalas(){
	int i = 1;
	for (i ; i < 51 ; i++) {
		VistaBala* bala = new VistaBala(TEXTURA_BALA);
		bala->cargarImagen();
		addBala(i, bala);
	}
	for (i ; i < 101 ; i++) {
		VistaBala* bala = new VistaBala(TEXTURA_BALA_ENEMIGA);
		bala->cargarImagen();
		addBala(i, bala);
	}
	for (i ; i < 151 ; i++) {
		VistaBala* bala = new VistaBala(TEXTURA_BALA_HMGUN);
		bala->cargarImagen();
		addBala(i, bala);
	}
	for (i ; i < 201 ; i++) {
		VistaBala* bala = new VistaBala(TEXTURA_BALA_SHOTGUN);
		bala->cargarImagen();
		bala->cargarImagenShotgun();
		bala->setShotgun(true);
		addBala(i, bala);
	}
	for (i ; i < 251 ; i++) {
		VistaBala* bala = new VistaBala(TEXTURA_BALA_RLAUNCHER);
		bala->cargarImagen();
		addBala(i, bala);
	}
	for (i ; i < 256 ; i++) {
		VistaBala* bala = new VistaBala(TEXTURA_BALA_RSHOBU);
		bala->cargarImagen();
		bala->setBomba(true);
		bala->cargarExplosion(renderizador);
		addBala(i, bala);
	}
}

void Juego::crearEnemigos(){
	string stream = cliente->recibir_nueva_vista();
	int cantidadEnemigos = stoi(stream);
	int i = 1;
	for (i ; i < cantidadEnemigos+1 ; i++){
		VistaEnemigo* enemigo = new VistaEnemigo(TEXTURAS_ENEMIGOS);
		enemigo->cargarImagen();
		addEnemigo(i, enemigo);
	}
}

void Juego::crearBonuses(){
	cout << "RECIBO BONUSES" << endl;
	idBonus = 1;

	while (true) {

		string stream = cliente->recibir_nueva_vista();

		if (stream == "$\n") break;

		string id_s = "";
		string tipo_s = "";

		string* variables[] = {&id_s, &tipo_s};

		int j = 0;

		for (int i=0; i<stream.size() - 1; i++) {

			char actual = stream[i];

			if (actual == '$') {
				j++;
				continue;
			}

			*(variables[j]) += actual;
		}

		int tipo = stoi(tipo_s);
		int idBn = stoi(id_s);
		VistaBonus* bonus;

		if (tipo == 1)
			bonus = new VistaBonus(TEXTURA_HMGUN);

		else if (tipo == 2)
			bonus = new VistaBonus(TEXTURA_SGUN);

		else if (tipo == 3)
			bonus = new VistaBonus(TEXTURA_RLAUNCHER);

		addBonus(idBn, bonus);
		idBonus++;
	}
}

void Juego::crearBoss(){
	int id = 1;
	VistaBoss* rshobu = new VistaRshobu(renderizador);
	rshobu->cargarImagen();
	addBoss(id, rshobu);
	id++;

	VistaBoss* daiManji = new VistaDaiManji(renderizador);
	daiManji->cargarImagen();
	addBoss(id, daiManji);
	id++;

	VistaBoss* rshobu2 = new VistaDaiManji(renderizador);
	rshobu2->cargarImagen();
	addBoss(id, rshobu2);
	id++;
}

void Juego::seleccionarBossSiguiente(){
	bossActual = getBossById(nivelActual++);
}

void Juego::cargarTexturaBala(){
	TEXTURA_BALA = new Textura(renderizador);
	if( !TEXTURA_BALA->cargarImagen( "imag/sprites/sfx/gunBullet.png") ) printf( "Fallo imagen bala\n" );

	TEXTURA_BALA_ENEMIGA = new Textura(renderizador);
	if( !TEXTURA_BALA_ENEMIGA->cargarImagen( "imag/sprites/sfx/soldierBullet.png") ) printf( "Fallo imagen bala enemiga\n" );

	TEXTURA_BALA_HMGUN = new Textura(renderizador);
	if( !TEXTURA_BALA_HMGUN->cargarImagen( "imag/sprites/sfx/disparoHMGun.png") ) printf( "Fallo imagen bala HMGUN\n" );

	TEXTURA_BALA_SHOTGUN = new Textura(renderizador);
	if( !TEXTURA_BALA_SHOTGUN->cargarImagen( "imag/sprites/sfx/disparoSgun.png") ) printf( "Fallo imagen bala SHOTGUN\n" );

	TEXTURA_BALA_RLAUNCHER = new Textura(renderizador);
	if( !TEXTURA_BALA_RLAUNCHER->cargarImagen( "imag/sprites/sfx/disparoRLauncher.png") ) printf( "Fallo imagen bala RLAUNCHER\n" );

	TEXTURA_BALA_RSHOBU = new Textura(renderizador);
	if( !TEXTURA_BALA_RSHOBU->cargarImagen( "imag/sprites/R-Shobu/disparo.png") ) printf( "Fallo imagen rshobu disparo\n" );

}

void Juego::cargarTexturaEnemigo(){
	Textura* TEXTURA_ENEMIGO_MUERTE1 = new Textura(renderizador);
	if( !TEXTURA_ENEMIGO_MUERTE1->cargarImagen( "imag/sprites/soldier/death1.png") ) printf( "Fallo imagen enemigo\n" );
	TEXTURAS_ENEMIGOS.push_back(TEXTURA_ENEMIGO_MUERTE1);

	Textura* TEXTURA_ENEMIGO_MUERTE2 = new Textura(renderizador);
	if( !TEXTURA_ENEMIGO_MUERTE2->cargarImagen( "imag/sprites/soldier/death2.png") ) printf( "Fallo imagen enemigo\n" );
	TEXTURAS_ENEMIGOS.push_back(TEXTURA_ENEMIGO_MUERTE2);

	Textura* TEXTURA_ENEMIGO_MIRANDO = new Textura(renderizador);
	if( !TEXTURA_ENEMIGO_MIRANDO->cargarImagen( "imag/sprites/soldier/looking.png") ) printf( "Fallo imagen enemigo\n" );
	TEXTURAS_ENEMIGOS.push_back(TEXTURA_ENEMIGO_MIRANDO);

	Textura* TEXTURA_ENEMIGO_CORRIENDO = new Textura(renderizador);
	if( !TEXTURA_ENEMIGO_CORRIENDO->cargarImagen( "imag/sprites/soldier/run.png") ) printf( "Fallo imagen enemigo\n" );
	TEXTURAS_ENEMIGOS.push_back(TEXTURA_ENEMIGO_CORRIENDO);

	Textura* TEXTURA_ENEMIGO_DISPARANDO = new Textura(renderizador);
	if( !TEXTURA_ENEMIGO_DISPARANDO->cargarImagen( "imag/sprites/soldier/shoot.png") ) printf( "Fallo imagen enemigo\n" );
	TEXTURAS_ENEMIGOS.push_back(TEXTURA_ENEMIGO_DISPARANDO);

	Textura* TEXTURA_ENEMIGO_QUIETO = new Textura(renderizador);
	if( !TEXTURA_ENEMIGO_QUIETO->cargarImagen( "imag/sprites/soldier/toying.png") ) printf( "Fallo imagen enemigo\n" );
	TEXTURAS_ENEMIGOS.push_back(TEXTURA_ENEMIGO_QUIETO);
}

void Juego::cargarTexturaBonus(){
	TEXTURA_HMGUN = new Textura(renderizador);
	if( !TEXTURA_HMGUN->cargarImagen( "imag/sprites/sfx/HMGun.png") ) printf( "Fallo imagen hmgun\n" );

	TEXTURA_SGUN = new Textura(renderizador);
	if( !TEXTURA_SGUN->cargarImagen( "imag/sprites/sfx/Sgun.png") ) printf( "Fallo imagen sgun\n" );

	TEXTURA_RLAUNCHER = new Textura(renderizador);
	if( !TEXTURA_RLAUNCHER->cargarImagen( "imag/sprites/sfx/RLauncher.png") ) printf( "Fallo imagen rlauncher\n" );

	TEXTURA_KILLALL = new Textura(renderizador);
	if( !TEXTURA_KILLALL->cargarImagen( "imag/sprites/sfx/KillAll.png") ) printf( "Fallo imagen kill all\n" );

	TEXTURA_RECOVER = new Textura(renderizador);
	if( !TEXTURA_RECOVER->cargarImagen( "imag/sprites/sfx/Recover.png") ) printf( "Fallo imagen recover\n" );
}

void Juego::parsearUpdateVista(string update){
	ProtocoloVistaUpdate::parse(update, &tipoObjeto, &id, &state, &posX, &posy, &posCam, &conectado, &spriteIdx, &aim, &saltando, &puntaje);
}

int Juego::getTipoObjeto(){
	return tipoObjeto;
}

void Juego::actualizarPersonaje(){
	puntajes->actualizarPuntaje(id, puntaje);

	VistaPersonaje* pj = getPersonajeById(id);

	if (pj->getPosCamara() < posCam){
		pj->setDerecha(true);
	}
	else if (pj->getPosCamara() > posCam){
		pj->setDerecha(false);
	}

	if (getPosX() < posX){
		moverCamara(id);
		setPosX(posX);
	}

	pj->setSaltando(saltando);
	pj->apuntar(aim);
	if (!(pj->getDisparar())) pj->setSpriteIndexTorso(spriteIdx);
	pj->setSpriteIndexPies(spriteIdx);
	pj->setConectado(conectado);
	pj->setPosCamara(posCam);
	pj->setPosy(posy);
	pj->setSeMovio(state);
}

void Juego::actualizarBala(){
	int derecha, arriba, abajo, izquierda, frame, rayo;
	derecha = spriteIdx;
	izquierda = aim;
	arriba = posCam;
	abajo = conectado;
	frame = saltando;

	VistaBala* bala = getBalaById(id);

	if (state && !existeBala(id)) addBalaViva(id, bala);
	else if (!state && existeBala(id) && !bala->isBomba()) removeBala(id);

	bala->setExiste(state);
	bala->setPosX(posX);
	bala->setPosY(posy);
	bala->setDerecha(derecha);
	bala->setArriba(arriba);
	bala->setAbajo(abajo);
	bala->setIzquierda(izquierda);
	if (bala->isShotgun()) bala->setFrame(frame);

}

void Juego::actualizarSpriteDisparo(){
	VistaPersonaje* pj = getPersonajeById(id);

	pj->setDisparar(state);
	pj->setSpriteIndexTorso(spriteIdx);
	pj->setSpriteIndexPies(spriteIdx);
	if (saltando) pj->ponerGun();
}

void Juego::actualizarEnemigo(){
	VistaEnemigo* enemigo = getEnemigoById(id);

	if (state && !existeEnemigo(id)) addEnemigoVivo(id, enemigo);
	else if (!state && existeEnemigo(id)) removeEnemigo(id);

	if (!conectado) enemigo->setExiste(state);
	enemigo->setPosX(posX);
	enemigo->setPosY(posy);
	enemigo->setFrame(spriteIdx);
	enemigo->setDisparando(aim);
	enemigo->setCantPasos(posCam);
	if (conectado) enemigo->morir();
}

void Juego::actualizarBonus(){
	if (spriteIdx == 1){
		nuevoBonus();
		return;
	}

	VistaBonus* bonus = getBonusById(id);

	bonus->setPosx(posX);
	bonus->setPosy(posy);
	bonus->setExiste(state);

	if (aim && saltando == 0){
		VistaPersonaje* pj = getPersonajeById(conectado);
		pj->ponerShotgun();
	}
}

void Juego::nuevoBonus(){
	VistaBonus* bonus;

	if (saltando == 1)
		bonus = new VistaBonus(TEXTURA_KILLALL);
	else if (saltando == 2)
		bonus = new VistaBonus(TEXTURA_RECOVER);

	addBonus(id, bonus);
}

void Juego::actualizarBoss(){
	bossActual->setDireccion(posX);
	bossActual->setExiste(state);
	bossActual->setPosx(posX);
	bossActual->setPosy(posy);
	bossActual->setFrame(spriteIdx);
	if (!conectado) {
		bossActual->morir();
		bossActual->setMuerto(!conectado);
	}
	if (id == 2){
		VistaDaiManji* dai = dynamic_cast<VistaDaiManji*>(bossActual);
		dai->setPuerta(aim);
		dai->setLaser(saltando);
		dai->setDisparando(puntaje);
	}
}

void Juego::crearVistaPuntajes(){
	puntajes = VistaPuntajes::NewVistaPuntaje(cantidadUsuarios, modoJuego, renderizador);
}

void Juego::actualizarImpacto(){
	VistaPersonaje* pj = getPersonajeById(id);

	if (state) pj->titilar();
	else pj->morir();

	if (puntaje != 0) {
		VistaBala* bala = getBalaById(puntaje);
		bala->setExplotando(true);
	}
}

void Juego::actualizarQuietos(){

	int i = 0;
	int j = 0;
	int sprite = id;
	int datos[8] = {state, posX, posy, posCam, conectado, spriteIdx, aim, saltando};

	for (i ; i < cantidadUsuarios ; i++){

		VistaPersonaje* pj = getPersonajeById(i+1);

		if (datos[j]){
			pj->setSeMovio(!datos[j]);
			pj->apuntar(datos[j+1]);
			if (!pj->getDisparar() && !pj->estaSaltando()) {
				pj->setSpriteIndexTorso(sprite);
				pj->setSpriteIndexPies(sprite);
			}
		}
		j += 2;
	}

}

void Juego::recibirUsuarios() {
	cout << "RECIBO USUARIOS" << endl;

	while (true) {

		string stream = cliente->recibir_nueva_vista();

		if (stream == "$\n") break;

		string nombre = "";
		string id = "";

		string *variables[] = {&nombre, &id};

		int j = 0;

		for (int i = 0; i < stream.size() - 1; i++) {

			char actual = stream[i];

			if (actual == '$') {
				j++;
				continue;
			}

			*(variables[j]) += actual;
		}

		int idPj = stoi(id);
		getPersonajeById(idPj)->setNombre(nombre);

	}
}

string Juego::getNombreUsuarioById(int id) {
	return vistasPersonajes[id]->getNombre();
}


bool Juego::puedePasarDeNivel() {
	return this->enterHabilitado;
}


void Juego::setPuedePasarDeNivel(bool aux) {
	this->enterHabilitado = aux;
}


void Juego::actualizarPuntaje() {
	puntajes->actualizarPuntaje(id, puntaje);
}

void Juego::setTransparenciaActual(Uint8 aux) {
	transparenciaActual = aux;
}

Uint8* Juego::getTransparenciaActual() {
	return &transparenciaActual;
}

typedef struct {
	Juego* juego;
	bool* quit;
	bool* pauseRecibir;
} controlador_t;

void* recibirVistas( void* arg){

	controlador_t* arg2 = (controlador_t*) arg;
	Juego* miJuego = (Juego*) arg2->juego;
	bool* quit = (bool*) arg2->quit;
	bool* pauseRecibir = (bool*) arg2->pauseRecibir;
	Cliente* cliente = miJuego->getCliente();

	while( !(*quit) ) {

		if (!(*pauseRecibir)) {
			cliente->encolar_vistas();
		}
	}
	return NULL;
}
int escucharEventos( void* arg ) {

	controlador_t* arg2 = (controlador_t*) arg;
	Juego* miJuego = (Juego*) arg2->juego;
	bool* quit = (bool*) arg2->quit;
	Cliente* cliente = miJuego->getCliente();

	//Event handler
	SDL_Event e;

	while( !(*quit) ) {

		while (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				*quit = true;
			}
			miJuego->handleEvent(e);
		}

	}
	return 0;
}

int main( int argc, char** argv) {

	//Start up SDL and create window
	Juego juego;

	if( !juego.iniciar() ) {
		printf("Failed to initialize!\n");
		return 1;
	}

	juego.presentacion();

	Cliente cliente(juego.getIp(), juego.getPuerto());
	juego.setCliente(&cliente);
	juego.conectar();

	juego.liberarPresentacion();

	juego.recibirEscenario();

	// Seteo el fondo
	Background* fondo = new Background(juego.getRenderer());
	juego.setBackground(fondo);
	juego.recibirCapas();
	fondo->prepararEscenario();

	// Creo bonuses
	juego.cargarTexturaBonus();
	juego.crearBonuses();

	// Creo enemigos
	juego.cargarTexturaEnemigo();
	juego.crearEnemigos();

	// Creo los puntajes
	juego.crearVistaPuntajes();

	// Recibir personajes
	juego.recibirPersonajes();

	// Creo la pool de balas
	juego.cargarTexturaBala();
	juego.crearBalas();

	// Creo Boss
	juego.crearBoss();
	juego.seleccionarBossSiguiente();

	juego.cargarEntrada();
	juego.entrada();
	juego.recibirUsuarios();

	//Main loop flag
	bool quit = false;
	bool pauseRecibir = false;

	controlador_t* arg = new controlador_t;
	arg->juego = &juego;
	arg->quit = &quit;
	arg->pauseRecibir = &pauseRecibir;

	// Thread que escucha eventos
	pthread_attr_t attr;
	bzero(&attr, sizeof(attr));
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_t* recibirThread = new pthread_t;
	if (pthread_create(recibirThread, NULL, recibirVistas, arg) != 0 ){
		cerr << "ERROR --> No se pudo crear thread de control" << endl;
		pthread_attr_destroy(&attr);
		exit(1);
	}

	// Dibujo por primera vez (es necesario)
	SDL_RenderClear( juego.getRenderer() );
	fondo->render(juego.getPosX());
	juego.renderizar();
	SDL_RenderPresent( juego.getRenderer() );

	juego.liberarEntrada();

	SDL_Event e;
	//WHILE APLICACION CORRIENDO
	while( !quit ) {
		pauseRecibir = false;

		if (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				quit = true;
			}
			juego.handleEvent(e);
		}

		string update = cliente.desencolar_vista();

		if (update != "$\n") {

			time_point<high_resolution_clock> start;
			start = high_resolution_clock::now();
			time_point<high_resolution_clock> actual;

			juego.parsearUpdateVista(update);
			int tipoObjeto = juego.getTipoObjeto();

			// Tipo de objeto 10 = REINICIAR ESCENARIO
			if (tipoObjeto == 10 ){
				juego.jugadoresInicio();
				usleep(100000);
				pauseRecibir = true;
				juego.recibirNuevoBackground();
				cout << "RECIBI TODO" << endl;
				continue;
			}

			// Tipo de objeto 1 = PERSONAJES
			if (tipoObjeto == 1)
				juego.actualizarPersonaje();

			// Tipo de objeto 2 = BALAS
			else if (tipoObjeto == 2)
				juego.actualizarBala();

			// Tipo de objeto 3 = SPRITE DISPARO
			else if (tipoObjeto == 3)
				juego.actualizarSpriteDisparo();

			// Tipo de objeto 4 = ENEMIGOS
			else if (tipoObjeto == 4)
				juego.actualizarEnemigo();

			// Tipo de objeto 5 = BONUSES
			else if (tipoObjeto == 5)
				juego.actualizarBonus();

			// Tipo de objeto 6 = BOSS
			else if (tipoObjeto == 6)
				juego.actualizarBoss();

			// Tipo de objeto 7 = IMPACTO BALA
			else if (tipoObjeto == 7)
				juego.actualizarImpacto();

			// Tipo de objeto 8 = PERSONAJE QUIETO
			else if (tipoObjeto == 8)
				juego.actualizarQuietos();

			// Tipo de objeto 9 = PUNTAJE
			else if (tipoObjeto == 9)
				juego.actualizarPuntaje();

			SDL_RenderClear( juego.getRenderer() );

			fondo->render(juego.getPosX());
			juego.renderizar();

			SDL_RenderPresent( juego.getRenderer() );

			actual = high_resolution_clock::now();
			auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
			auto elapsed_ms = duration_cast<nanoseconds>(deltaTiempo);
			auto time = elapsed_ms.count()/1000000.0;

			//cout << "Elapsed ms: " << time << endl;

		}
	}

	//Free resources and close SDL
	delete recibirThread;
	pthread_attr_destroy(&attr);
	cliente.desconectar();
	juego.close();

	return 0;

}

Uint8 Juego::LIMITE_SUPERIOR_TRANSPARENCIA = 255;
int Juego::LIMITE_SUPERIOR_TRANSPARENCIA_NEGATIVA = -255;
Uint8 Juego::transparenciaActual = 255;
Uint8 Juego::LIMITE_INFERIOR_TRANSPARENCIA = 0;
