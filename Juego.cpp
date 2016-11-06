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

using namespace std;
using namespace chrono;

class Juego {

private:
	SDL_Renderer* renderizador;
	SDL_Window* ventana;
	SDL_Rect* camera;
	Cliente* cliente;
	unordered_map<int, VistaPersonaje*> vistas;
	int lastKeyPressed;
	Background* fondo;
	int posx;
	int screenWidth;
	int screenHeight;
	int levelWidth;
	int levelHeight;

	Textura* neoGeo = new Textura(renderizador);
	Textura* esperando = new Textura(renderizador);
	Textura* fondoInicial = new Textura(renderizador);
	Textura* TEXTURA_EXPLOSION1 = new Textura(renderizador);
	Textura* TEXTURA_EXPLOSION2 = new Textura(renderizador);
	Textura* TEXTURA_EXPLOSION3 = new Textura(renderizador);
	Textura* TEXTURA_METAL = new Textura(renderizador);
	SDL_Rect spriteEntrada1[ 10 ];
	SDL_Rect spriteEntrada2[ 10 ];
	SDL_Rect spriteEntrada3[ 10 ];
	SDL_Rect spriteMetal[ 7 ];


public:

	Juego() {
		renderizador = NULL;
		ventana = NULL;
		lastKeyPressed = 0;
	}

	void close() {
		//ToDo Liberar personajes

		//Free loaded images
		for (auto kv : vistas)
			kv.second->liberarTextura();

		//Destroy window
		SDL_DestroyRenderer( renderizador );
		SDL_DestroyWindow( ventana );
		ventana = NULL;
		renderizador = NULL;
		delete fondo;

		//Quit SDL subsystems
		IMG_Quit();
		SDL_Quit();
	}

	void presentacion(){
		//ESTE METODO ES EL QUE PIDE IP PUERTO Y NOMBRE
		Uint32 start = 0;
		Texto* textoip = new Texto("IP: ", renderizador);
		textoip->cargarTitulo();
		Texto* textopuerto = new Texto("Puerto: ", renderizador);
		textopuerto->cargarTitulo();
		Texto* textonombre = new Texto("Nombre: ", renderizador);
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
			quit=false;
			while(!quit)
			{
				SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0 );
				SDL_RenderClear( renderizador );
				neoGeo->render( 100, 50,NULL, 0.0, NULL, SDL_FLIP_NONE);
				quit=textopuerto->pedir();
				SDL_RenderPresent( renderizador );
			}
			quit=false;
			while(!quit)
			{
				SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0 );
				SDL_RenderClear( renderizador );
				neoGeo->render( 100, 50,NULL, 0.0, NULL, SDL_FLIP_NONE);
				quit=textonombre->pedir();
				SDL_RenderPresent( renderizador );
			}
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
				esperando->render(180,400,NULL, 0.0, NULL, SDL_FLIP_NONE);

				SDL_RenderPresent( renderizador );
			}
		}
	}

	void cargarEntrada(){
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
		if (!esperando->cargarImagen("imag/entrada/esperando.png")){
			cout << "error cargando esperando"<<endl;
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
		Texto* esperandoTexto = new Texto("Esperando jugadores ", renderizador);
		esperandoTexto->cargarTitulo();
		Texto* puntitos = new Texto(".", renderizador);
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
		return vistas[id];
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
		vistas[id] = pj;
	}

	void conectar(){
		cliente->conectar();
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

				case SDLK_r:
					comando.setScancode(SDLK_r);
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

				case SDLK_r:
					break;


			}
		}
	}

	void moverCamara(int id){

		for (auto kv : vistas){
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
	}

	void renderizar() {
		for (auto kv : vistas) {
			if (!(kv.second->getConectado()) && !(kv.second->getGris())) {
				kv.second->ponerTexturaGris();
			}
			else if (kv.second->getConectado() && kv.second->getGris()) {
				kv.second->sacarTexturaGris();
			}
			kv.second->render(kv.second->getSeMovio());
		}
	}

	int getPersonajeMasMovido(){
		int aux = 0;
		for (auto kv : vistas){
			int x = kv.second->getX();
			if (x > aux) aux = x;
		}
		return aux;
	}

	void jugadoresInicio(){
		for ( auto kv : vistas){
			kv.second->setPosCamara(0);
			kv.second->setPosx(0);
			kv.second->setPosy(360);
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
			personaje->setearSprites(sprite);
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
	Cliente cliente(argv);

	juego.setCliente(&cliente);
	juego.conectar();

	juego.recibirEscenario();

	if( !juego.iniciar() ) {
		printf("Failed to initialize!\n");
		return 1;
	}

	// Seteo el fondo
	Background* fondo = new Background(juego.getRenderer());
	juego.setBackground(fondo);
	juego.recibirCapas();
	fondo->prepararEscenario();

	// Recibir personajes
	juego.recibirPersonajes();

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

	juego.salaDeEspera();

	// Dibujo por primera vez (es necesario)
	SDL_RenderClear( juego.getRenderer() );
	fondo->render(juego.getPosX());
	juego.renderizar();
	SDL_RenderPresent( juego.getRenderer() );

	SDL_Event e;
	//WHILE APLICACION CORRIENDO
	while( !quit ) {
		pauseRecibir = false;

		time_point<high_resolution_clock> start;
		start = high_resolution_clock::now();
		time_point<high_resolution_clock> actual;

		if (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				quit = true;
			}
			juego.handleEvent(e);
		}

		string update = cliente.desencolar_vista();

		if (update != "$\n") {

			int id, state, posx, posy, posCam, conectado, spriteIdx;

			ProtocoloVistaUpdate::parse(update, &id, &state, &posx, &posy, &posCam, &conectado, &spriteIdx);

			if (id == 100 ){
				juego.jugadoresInicio();
				usleep(100000);
				pauseRecibir = true;
				juego.recibirNuevoBackground();
				cout << "RECIBI TODO" << endl;
				continue;
			}

			VistaPersonaje* pj = juego.getPersonajeById(id);

			if (pj->getPosCamara() < posCam){
				pj->setDerecha(true);
			}
			else if (pj->getPosCamara() > posCam){
				pj->setDerecha(false);
			}

			if (juego.getPosX() < posx){
				juego.moverCamara(id);
				juego.setPosX(posx);
			}

			pj->setSpriteIndex(spriteIdx);
			pj->setConectado(conectado);
			pj->setPosCamara(posCam);
			pj->setPosy(posy);
			pj->setSeMovio(state);

			SDL_RenderClear( juego.getRenderer() );

			fondo->render(juego.getPosX());
			juego.renderizar();

			SDL_RenderPresent( juego.getRenderer() );

		}


		// MIDO EL TIEMPO QUE TARDO EN RENDERIZAR
		actual = high_resolution_clock::now();
		auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
		auto elapsed_ms = duration_cast<nanoseconds>(deltaTiempo);
		auto time = elapsed_ms.count()/1000000.0;

		//cout << "Elapsed ms: " << time << endl;
	}

	//Free resources and close SDL
	delete recibirThread;
	pthread_attr_destroy(&attr);
	cliente.desconectar();
	juego.close();

	return 0;
}

