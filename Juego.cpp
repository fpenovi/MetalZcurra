#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <chrono>
#include <SDL2/SDL_thread.h>
#include "Cliente.h"
#include "VistaPersonaje.h"
#include "ProtocoloComando.h"
#include "ProtocoloVistaUpdate.h"
#include "ProtocoloNuevaVista.h"
#include "Background.h"
#include "Texto.h"
#include "Textura.h"
#include "VistaBala.h"
#include "VistaEnemigo.h"

using namespace std;
using namespace chrono;

class Juego {

private:
	SDL_Renderer* renderizador;
	SDL_Window* ventana;
	SDL_Rect* camera;
	Cliente* cliente;
	unordered_map<int, VistaPersonaje*> vistasPersonajes;
	unordered_map<int, VistaBala*> vistasBalas;
	unordered_map<int, VistaEnemigo*> vistasEnemigos;
	int lastKeyPressed;
	Background* fondo;
	int posx;
	int screenWidth = 800;
	int screenHeight = 600;
	int levelWidth;
	int levelHeight;
	Textura* TEXTURA_BALA;
	int tipoObjeto, id, state, posX, posy, posCam, conectado, spriteIdx;

	// Atributos para sala de espera
	Texto* textoip;
	Texto* textopuerto;
	Texto* textonombre;
	Texto* esperandoTexto;
	Texto* puntitos;
	Textura* neoGeo;
	Textura* fondoInicial;
	Textura* TEXTURA_EXPLOSION1;
	Textura* TEXTURA_EXPLOSION2;
	Textura* TEXTURA_EXPLOSION3;
	Textura* TEXTURA_METAL;
	SDL_Rect spriteEntrada1[ 10 ];
	SDL_Rect spriteEntrada2[ 10 ];
	SDL_Rect spriteEntrada3[ 10 ];
	SDL_Rect spriteMetal[ 7 ];
	string ip;
	string puerto;
	string nombre;


public:

	Juego() {
		renderizador = NULL;
		ventana = NULL;
		lastKeyPressed = 0;
	}

	void close() {
		//Free loaded images
		for (auto kv : vistasPersonajes)
			delete kv.second;

		for (auto kv : vistasBalas)
			delete kv.second;

		for (auto kv : vistasEnemigos)
			delete kv.second;

		//Destroy window
		SDL_DestroyRenderer( renderizador );
		SDL_DestroyWindow( ventana );
		ventana = NULL;
		renderizador = NULL;
		delete TEXTURA_BALA;
		delete fondo;

		//Quit SDL subsystems
		IMG_Quit();
		SDL_Quit();
	}

	void presentacion(){
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

	void liberarPresentacion(){
		delete neoGeo;
		delete textoip;
		delete textopuerto;
		delete textonombre;
	}

	void cargarEntrada(){
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

	void entrada(){
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

	void liberarEntrada(){
		delete esperandoTexto;
		delete puntitos;
		delete fondoInicial;
		delete TEXTURA_EXPLOSION1;
		delete TEXTURA_EXPLOSION2;
		delete TEXTURA_EXPLOSION3;
		delete TEXTURA_METAL;
	}

	bool iniciar() {
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
			ventana = SDL_CreateWindow( "PERSONAJE1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN );
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

	SDL_Renderer* getRenderer() {
		return renderizador;
	}

	SDL_Window* getVentana() {
		return ventana;
	}

	SDL_Rect* getCamera(){
		return camera;
	}

	Cliente* getCliente(){
		return cliente;
	}

	Background* getBackground(){
		return fondo;
	}

	int getPosX(){
		return posx;
	}

	VistaPersonaje* getPersonajeById(int id){
		return vistasPersonajes[id];
	}

	VistaBala* getBalaById(int id){
		return vistasBalas[id];
	}

	VistaEnemigo* getEnemigoById(int id){
		return vistasEnemigos[id];
	}

	string getIp(){
		return ip;
	}

	string getPuerto(){
		return puerto;
	}

	string getNombre(){
		return nombre;
	}

	void setPosX(int x){
		this->posx = x;
	}

	void setBackground(Background* fondo){
		this->fondo = fondo;
	}

	void setCliente(Cliente* client){
		cliente = client;
	}

	void setCamara(SDL_Rect* camara){
		camera = camara;
	}

	void addPersonaje(int id, VistaPersonaje* pj){
		vistasPersonajes[id] = pj;
	}

	void addBala(int id, VistaBala* bala){
		vistasBalas[id] = bala;
	}

	void addEnemigo(int id, VistaEnemigo* enemigo){
		vistasEnemigos[id] = enemigo;
	}

	void conectar(){
		cliente->conectar(nombre);
	}

	void handleEvent( SDL_Event& e) {

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

				case SDLK_r:
					comando.setScancode(SDLK_r);
					comando.setType(1);
					msj = comando.toString();
					cliente->enviarAusuario("TODOS", msj, false);
					break;

				case SDLK_z:
					comando.setScancode(SDLK_z);
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

				case SDLK_r:
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

	void moverCamara(int id){

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
		for (auto kv : vistasEnemigos){
			if (kv.second->existeEnemigo()) kv.second->setPosX(kv.second->getPosx()-7);
		}
	}

	void renderizar() {
		for (auto kv : vistasPersonajes) {
			if (!(kv.second->getConectado()) && !(kv.second->getGris())) {
				kv.second->ponerTexturaGris();
			}
			else if (kv.second->getConectado() && kv.second->getGris()) {
				kv.second->sacarTexturaGris();
			}
			kv.second->render(kv.second->getSeMovio());
		}

		for (auto kv : vistasBalas){
			kv.second->render();
		}

		for (auto kv : vistasEnemigos){
			kv.second->render();
		}
	}

	int getPersonajeMasMovido(){
		int aux = 0;
		for (auto kv : vistasPersonajes){
			int x = kv.second->getX();
			if (x > aux) aux = x;
		}
		return aux;
	}

	void jugadoresInicio(){
		for ( auto kv : vistasPersonajes){
			kv.second->setPosCamara(0);
			kv.second->setPosx(0);
			kv.second->setPosy(440);
			kv.second->setSeMovio(false);
			kv.second->setDerecha(true);
		}
		setPosX(0);
	}

	void salaDeEspera(){
		cout << "ESPERANDO A TODOS LOS USUARIOS" << endl;
		string stream = cliente->recibir_nueva_vista();
	}

	void recibirEscenario(){
		string stream = cliente->recibir_nueva_vista();

		cout << stream;

		string ventanaAncho = "";
		string ventanaAlto = "";
		string nivelAncho = "";
		string nivelAlto = "";

		string* variables[] = {&ventanaAncho, &ventanaAlto, &nivelAncho, &nivelAlto};

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
		levelWidth = stoi(nivelAncho);
		levelHeight = stoi(nivelAlto);
	}

	void recibirCapas(){
		cout <<"RECIBIENDO BACKGROUND"<< endl;

		while (true) {

			string stream = cliente->recibir_nueva_vista();

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

	void recibirPersonajes(){

		while (true) {

			string nuevaVista = cliente->recibir_nueva_vista();

			if (nuevaVista == "$\n") break;

			int id, sprite, posx, posy, cam, conectado;

			ProtocoloNuevaVista::parse(nuevaVista, &id, &sprite, &posx, &posy, &cam, &conectado);

			VistaPersonaje *personaje = new VistaPersonaje(getRenderer());

			setPosX(posx);
			//personaje->setearSprites(sprite);
			personaje->setId(id);
			personaje->setPosCamara(cam);
			personaje->setPosx(posx);
			personaje->setPosy(posy);
			personaje->setSeMovio(false);
			personaje->setConectado(conectado);
			addPersonaje(id, personaje);

			cout << "CONECTADO: " << conectado << endl;

			if (!personaje->cargarImagen()) {
				printf("Failed to load media!\n");
				exit(1);
			}

		}

	}

	void recibirNuevoBackground(){

		delete fondo;

		// Seteo el fondo
		Background* fondo = new Background(getRenderer());
		setBackground(fondo);
		recibirNuevasCapas();
		fondo->prepararEscenario();

		SDL_RenderClear( getRenderer() );
		fondo->render(getPosX());
		renderizar();
		SDL_RenderPresent( getRenderer() );

	}

	void recibirNuevasCapas(){
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

	void crearBalas(){
		int i = 1;
		for (i ; i < 51 ; i++) {
			VistaBala* bala = new VistaBala(TEXTURA_BALA);
			bala->cargarImagen();
			addBala(i, bala);
		}
		for (i ; i < 101 ; i++) {
			VistaBala* bala = new VistaBala(TEXTURA_BALA);
			bala->cargarImagen();
			addBala(i, bala);
		}
	}

	void crearEnemigos(){
		int i = 1;
		for (i ; i < 6 ; i++){
			VistaEnemigo* enemigo = new VistaEnemigo(getRenderer());
			enemigo->cargarImagen();
			addEnemigo(i, enemigo);
		}
	}

	void cargarTexturaBala(){
		TEXTURA_BALA = new Textura(renderizador);
		if( !TEXTURA_BALA->cargarImagen( "imag/bala/bala.png") ) printf( "Fallo imagen bala\n" );
	}

	void parsearUpdateVista(string update){
		ProtocoloVistaUpdate::parse(update, &tipoObjeto, &id, &state, &posX, &posy, &posCam, &conectado, &spriteIdx);
	}

	int getTipoObjeto(){
		return tipoObjeto;
	}

	void actualizarPersonaje(){

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

		if (!(pj->getDisparar())) pj->setSpriteIndexTorso(spriteIdx);
		pj->setSpriteIndexPies(spriteIdx);
		pj->setConectado(conectado);
		pj->setPosCamara(posCam);
		pj->setPosy(posy);
		pj->setSeMovio(state);
	}

	void actualizarBala(){
		int derecha, arriba, abajo;
		derecha = spriteIdx;
		arriba = posCam;
		abajo = conectado;

		VistaBala* bala = getBalaById(id);

		bala->setExiste(state);
		bala->setPosX(posX);
		bala->setPosY(posy);
		bala->setDerecha(derecha);
		bala->setArriba(arriba);
		bala->setAbajo(abajo);

	}

	void actualizarSpriteDisparo(){
		VistaPersonaje* pj = getPersonajeById(id);

		pj->setDisparar(state);
		pj->setSpriteIndexTorso(spriteIdx);
		pj->setSpriteIndexPies(spriteIdx);
	}

	void actualizarEnemigo(){
		VistaEnemigo* enemigo = getEnemigoById(id);

		enemigo->setExiste(state);
		enemigo->setPosX(posX);
		enemigo->setPosY(posy);
		if (conectado) enemigo->morir();
	}
};

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

	if (argc < 3) {
		fprintf(stderr, "Modo de Uso: %s IP-hostname puerto\n", argv[0]);
		exit(0);
	}

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

	// Recibir personajes
	juego.recibirPersonajes();

	// Creo la pool de balas
	juego.cargarTexturaBala();
	juego.crearBalas();

	// Creo enemigos
	juego.crearEnemigos();

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

	juego.cargarEntrada();
	juego.entrada();
	juego.salaDeEspera();

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

