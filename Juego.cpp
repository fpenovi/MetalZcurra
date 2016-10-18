#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <chrono>
#include <SDL2/SDL_thread.h>
#include "Cliente.h"
#include "VistaMarco.h"
#include "ProtocoloComando.h"
#include "ProtocoloVistaUpdate.h"
#include "ProtocoloNuevaVista.h"
#include "HandleKeyHold.h"
#include "HandleJump.h"
#include "LTimer.h"
#include "Background.h"

using namespace std;
using namespace chrono;

//Screen dimension constants

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define LEVEL_WIDTH 4500
#define LEVEL_HEIGHT 480


class Juego {

private:
	SDL_Renderer* renderizador;
	SDL_Window* ventana;
	SDL_Rect* camera;
	Cliente* cliente;
	unordered_map<int, VistaMarco*> vistas;
	VistaMarco* personaje;
	int lastKeyPressed;
	HandleKeyHold* keyHoldHandler;
	HandleJump* jumpHandler;
	Background* fondo;

public:

	Juego() {
		renderizador = NULL;
		ventana = NULL;
		keyHoldHandler = NULL;
		jumpHandler = NULL;
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
		delete keyHoldHandler;
		delete jumpHandler;
		delete fondo;

		//Quit SDL subsystems
		IMG_Quit();
		SDL_Quit();
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
			ventana = SDL_CreateWindow( "PERSONAJE1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

	VistaMarco* getPersonajeById(int id){
		return vistas[id];
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

	void addPersonaje(int id, VistaMarco* pj){
		vistas[id] = pj;
	}

	void setPersonaje(VistaMarco* pj){
		this->personaje = pj;
	}

	void conectar(){
		cliente->conectar();
	}

	void crearKeyHoldHandler() {
		keyHoldHandler = new HandleKeyHold(this->cliente);
		keyHoldHandler->On();
		keyHoldHandler->Pause();
	}

	void crearJumpHandler(){
		jumpHandler = new HandleJump(this->cliente);
		jumpHandler->On();
		jumpHandler->Pause();
	}

	void handleEvent( SDL_Event& e) {

		ProtocoloComando comando;
		string msj;

		// Si toco una tecla por la primera vez
		if ( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {

			switch( e.key.keysym.sym ) {

				case SDLK_LEFT:
					keyHoldHandler->setKeyPressed(SDLK_LEFT);
					keyHoldHandler->Resume();
					break;

				case SDLK_RIGHT:
					keyHoldHandler->setKeyPressed(SDLK_RIGHT);
					keyHoldHandler->Resume();
					break;

				case SDLK_UP:
					jumpHandler->setKeyPressed(SDLK_UP);
					jumpHandler->Resume();
					//if (!saltando) saltando=true;
					//subiendo=true;
					//return msj;
					break;
			}
		}

		// Si suelto la tecla
		else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {

			switch( e.key.keysym.sym ) {

				case SDLK_LEFT:
					keyHoldHandler->Pause();
					keyHoldHandler->setKeyPressed(0);
					comando.setScancode(SDLK_LEFT);
					comando.setType(0);
					msj = comando.toString();
					cliente->enviarAusuario("TODOS", msj, false);
					break;

				case SDLK_RIGHT:
					keyHoldHandler->Pause();
					keyHoldHandler->setKeyPressed(0);
					comando.setScancode(SDLK_RIGHT);
					comando.setType(0);
					msj = comando.toString();
					cliente->enviarAusuario("TODOS", msj, false);
					break;

				case SDLK_UP:
					jumpHandler->Pause();
					break;
			}
		}
	}

	void moverCamara(){

		//Center the camera over the personaje
		camera->x = ( personaje->getX() + personaje->getAncho() / 2 ) - SCREEN_WIDTH / 2;
		camera->y = ( personaje->getY() + personaje->getAlto() / 2 ) - SCREEN_HEIGHT / 2;

		//Keep the camera in bounds
		if ( camera->x < 0 )
			camera->x = 0;

		if ( camera->y < 0 )
			camera->y = 0;

		if ( camera->x > LEVEL_WIDTH - camera->w )
			camera->x = LEVEL_WIDTH - camera->w;

		if ( camera->y > LEVEL_HEIGHT - camera->h )
			camera->y = LEVEL_HEIGHT - camera->h;

	}

	void renderizar(){
		for (auto kv : vistas)
			kv.second->render(kv.second->getSeMovio(), camera->x, camera->y);
	}
};

typedef struct {
	Juego* juego;
	bool* quit;
} controlador_t;

int recibirVistas( void* arg){

	controlador_t* arg2 = (controlador_t*) arg;
	Juego* miJuego = (Juego*) arg2->juego;
	bool* quit = (bool*) arg2->quit;
	Cliente* cliente = miJuego->getCliente();

	while( !(*quit) ) {

		cliente->encolar_vistas();

	}
	return 0;
}

int escucharEventos( void* arg ) {

	controlador_t* arg2 = (controlador_t*) arg;
	Juego* miJuego = (Juego*) arg2->juego;
	bool* quit = (bool*) arg2->quit;
	Cliente* cliente = miJuego->getCliente();

	//Event handler
	SDL_Event e;

	while( !(*quit) ) {

		if  (SDL_PollEvent(&e) != 0) {

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
	juego.crearKeyHoldHandler();
	juego.crearJumpHandler();
	//Textura fondo;

	if( !juego.iniciar() ) {
		printf("Failed to initialize!\n");
		return 1;
	}

	while (true){

		string nuevaVista = cliente.recibir_nueva_vista();

		if (nuevaVista == "$\n") break;

		int id, sprite, posx, posy;

		ProtocoloNuevaVista::parse(nuevaVista, &id, &sprite, &posx, &posy);

		VistaMarco* personaje = new VistaMarco(juego.getRenderer());
		personaje->setId(id);
		personaje->setPosx(posx);
		personaje->setPosy(posy);
		personaje->setSeMovio(false);
		juego.addPersonaje(id, personaje);

		if ( !personaje->cargarImagen() ) {
			printf("Failed to load media!\n");
			return 1;
		}
	}

	Background fondo(juego.getRenderer());
	juego.setBackground(&fondo);

	if(!fondo.agregar("imag/background/gris.png") || !fondo.agregar("imag/background/rojo.png")){
		printf( "Failed to load media!\n" );
	}

	fondo.prepararEscenario();

	juego.setPersonaje(juego.getPersonajeById(1));

	//Main loop flag
	bool quit = false;

	// Creo la camara
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	juego.setCamara(&camera);

	// Thread que escucha eventos
	controlador_t* arg = new controlador_t;
	arg->juego = &juego;
	arg->quit = &quit;
	SDL_Thread* threadID = SDL_CreateThread( escucharEventos, "EscucharEventos", arg );
	SDL_Thread* threadID2 = SDL_CreateThread( recibirVistas, "RecibirVistas", arg );

	int countedFrames = 0;
	LTimer fpsTimer;
	LTimer capTimer;
	fpsTimer.start();
	const int SCREEN_FPS = 60;
	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

	//WHILE APLICACION CORRIENDO
	while( !quit ) {

		capTimer.start();

		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if ( avgFPS > 2000000 )		// Si la division falla, corrijo el fps
			avgFPS = 0;

		time_point<high_resolution_clock> start;
		start = high_resolution_clock::now();
		time_point<high_resolution_clock> actual;

		string update = cliente.desencolar_vista();

		if (update != "$\n") {

			int id, state, posx, posy;

			ProtocoloVistaUpdate::parse(update, &id, &state, &posx, &posy);

			VistaMarco* pj = juego.getPersonajeById(id);

			if (pj->getX() < posx){
				pj->setDerecha(true);
			}
			else if (pj->getX() > posx){
				pj->setDerecha(false);
			}

			pj->setPosx(posx);
			pj->setPosy(posy);
			pj->setSeMovio(state);
		}

		//Muevo la camara
		juego.moverCamara();

		//Borro la pantalla
		//DRAWCOLOR ASI PONE TODO EN BLANCO
		SDL_SetRenderDrawColor( juego.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( juego.getRenderer() );

		//Render background
		//fondo.render( 0, 0, &camera );
		fondo.render(juego.getPersonajeById(1)->getX());
		juego.renderizar();
		SDL_RenderPresent( juego.getRenderer() );

		++countedFrames;	// luego de renderizar incremento los frames renderizados

		// Si termine de renderizar antes de tiempo, espero el tiempo necesario
		int frameTicks = capTimer.getTicks();
		if ( frameTicks < SCREEN_TICKS_PER_FRAME )
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );

		actual = high_resolution_clock::now();
		auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
		auto elapsed_ms = duration_cast<nanoseconds>(deltaTiempo);

		cout << "Elapsed ms: " << deltaTiempo.count() / 1000000.0 << endl;
	}

	//Free resources and close SDL
	juego.close();
	cliente.desconectar();

	return 0;
}

