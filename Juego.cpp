#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <SDL2/SDL_thread.h>
#include "Cliente.h"
#include "VistaMarco.h"
#include "ProtocoloComando.h"
#include "ProtocoloVistaUpdate.h"
#include "HandleKeyHold.h"

using namespace std;

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
	VistaMarco* personaje;
	int lastKeyPressed;
	HandleKeyHold* keyHoldHandler;

public:

	Juego() {
		renderizador = NULL;
		ventana = NULL;
		keyHoldHandler = NULL;
		lastKeyPressed = 0;
	}

	void close() {
		//Free loaded images
		personaje->liberarTextura();

		//Destroy window
		SDL_DestroyRenderer( renderizador );
		SDL_DestroyWindow( ventana );
		ventana = NULL;
		renderizador = NULL;
		delete keyHoldHandler;

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
				renderizador = SDL_CreateRenderer( ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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

	VistaMarco* getPersonaje(){
		return personaje;
	}


	void setCliente(Cliente* client){
		cliente = client;
	}

	void setCamara(SDL_Rect* camara){
		camera = camara;
	}

	void setPersonaje(VistaMarco* pj){
		personaje = pj;
	}

	void conectar(){
		cliente->conectar();
	}

	void crearKeyHoldHandler() {
		keyHoldHandler = new HandleKeyHold(this->cliente);
		keyHoldHandler->On();
		keyHoldHandler->Pause();
	}

	void handleEvent( SDL_Event& e) {

		ProtocoloComando comando;
		string msj;

		// Si toco una tecla por la primera vez
		if ( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {

			switch( e.key.keysym.sym ) {

				case SDLK_LEFT:
					//comando.setScancode(SDLK_LEFT);
					//comando.setType(1);
					//msj = comando.toString();
					keyHoldHandler->setKeyPressed(SDLK_LEFT);
					keyHoldHandler->Resume();
					//velx -= Personaje_VEL;
					//derecha = false;
					//return msj;
					break;

				case SDLK_RIGHT:
					//comando.setScancode(SDLK_RIGHT);
					//comando.setType(1);
					//msj = comando.toString();
					keyHoldHandler->setKeyPressed(SDLK_RIGHT);
					keyHoldHandler->Resume();
					//velx += Personaje_VEL;
					//derecha = true;
					//return msj;
					break;

				case SDLK_UP:
					//comando.setScancode(SDLK_UP);
					//comando.setType(1);
					//msj = comando.toString();
					//if (!saltando) saltando=true;
					//subiendo=true;
					//return msj;
					break;
			}
		}

		// Si suelto la tecla
		else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {

			keyHoldHandler->Pause();
			keyHoldHandler->setKeyPressed(0);

			switch( e.key.keysym.sym ) {

				case SDLK_LEFT:
					comando.setScancode(SDLK_LEFT);
					comando.setType(0);
					msj = comando.toString();
					cliente->enviarAusuario("TODOS", msj, false);
					//velx += Personaje_VEL;
					//return msj;
					break;

				case SDLK_RIGHT:
					comando.setScancode(SDLK_RIGHT);
					comando.setType(0);
					msj = comando.toString();
					cliente->enviarAusuario("TODOS", msj, false);
					//velx -= Personaje_VEL;
					//return msj;
					break;

				case SDLK_UP:
					//return msj;
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
};

typedef struct {
	Juego* juego;
	bool* quit;
} controlador_t;

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
	//Textura fondo;

	if( !juego.iniciar() ) {
		printf("Failed to initialize!\n");
		return 1;
	}

	VistaMarco personaje(juego.getRenderer());
	juego.setPersonaje(&personaje);

	//Load media
	if ( !personaje.cargarImagen() ) {
		printf("Failed to load media!\n");
		return 1;
	}

	//Main loop flag
	bool quit = false;
	bool seMovio;

	// Creo la camara
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	juego.setCamara(&camera);

	// Thread que escucha eventos
	//controlador_t* arg = new controlador_t;
	//arg->juego = &juego;
	//arg->quit = &quit;
	//SDL_Thread* threadID = SDL_CreateThread( escucharEventos, "EscucharEventos", arg );

	SDL_Event e;

	//WHILE APLICACION CORRIENDO
	while( !quit ) {

		//cambio a nueva posicion
		//seMovio = personaje.mover();
		if  (SDL_PollEvent(&e) != 0) {

			SDL_PumpEvents();
			SDL_FlushEvent(SDL_KEYDOWN);

			if (e.type == SDL_QUIT) {
				quit = true;
			}
			juego.handleEvent(e);
		}

		cout << "ESCUCHO MENSAJE" << endl;

		string update = cliente.recibir_vista();

		if (update != "$\n") {

			int id, state, posx, posy;

			ProtocoloVistaUpdate::parse(update, &id, &state, &posx, &posy);

			personaje.setPosx(posx);
			personaje.setPosy(posy);
			seMovio = state;
		}

		//Muevo la camara
		juego.moverCamara();

		//Borro la pantalla
		//DRAWCOLOR ASI PONE TODO EN BLANCO
		SDL_SetRenderDrawColor( juego.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( juego.getRenderer() );

		//Render background
		//fondo.render( 0, 0, &camera );
		personaje.render(seMovio,camera.x,camera.y);
		SDL_RenderPresent( juego.getRenderer() );

		cout << "DIBUJO TODO" << endl;
	}

	//Free resources and close SDL
	juego.close();
	cliente.desconectar();

	return 0;
}

