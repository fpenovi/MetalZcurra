#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "Cliente.h"
#include "VistaMarco.h"
#include "ProtocoloComando.h"
using namespace std;

//Screen dimension constants

#define SCREEN_WIDTH 800
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define LEVEL_WIDTH 4500
#define LEVEL_HEIGHT 480

class Juego {

private:
	SDL_Renderer* renderizador;
	SDL_Window* ventana;

public:

	Juego() {
		renderizador = NULL;
		ventana = NULL;
	}

	void close(VistaMarco personaje) {
		//Free loaded images
		personaje.liberarTextura();

		//Destroy window
		SDL_DestroyRenderer( renderizador );
		SDL_DestroyWindow( ventana );
		ventana = NULL;
		renderizador = NULL;

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
};

void handleEvent( SDL_Event& e, Cliente &cliente) {

	ProtocoloComando msj;

	//If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
		//Adjust the velocity
		switch( e.key.keysym.sym ) {

			case SDLK_LEFT:
				msj.setScancode(SDLK_LEFT);
				msj.setType(0);
				cliente.enviarAusuario("TODOS", msj.toString(), false);
				//velx -= Personaje_VEL;
				//derecha = false;
				break;

			case SDLK_RIGHT:
				msj = to_string(SDLK_RIGHT);
				msj = msj + "\n";
				cliente.enviarAusuario("TODOS", msj, false);
				//velx += Personaje_VEL;
				//derecha = true;
				break;

			case SDLK_UP:
				msj = to_string(SDLK_UP);
				msj = msj + "\n";
				cliente.enviarAusuario("TODOS", msj, false);
				//if (!saltando) saltando=true;
				//subiendo=true;
				break;
		}
	}

		//If a key was released
	else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {

		//Adjust the velocity
		switch( e.key.keysym.sym ) {

			case SDLK_LEFT:
				//velx += Personaje_VEL;
				break;

			case SDLK_RIGHT:
				//velx -= Personaje_VEL;
				break;

			case SDLK_UP:
				break;
		}
	}
}

int main( int argc, char** argv) {

	if (argc < 3) {
		fprintf(stderr, "Modo de Uso: %s IP-hostname puerto\n", argv[0]);
		exit(0);
	}

	//Start up SDL and create window
	Juego juego;
	Cliente cliente(argv);
	cliente.conectar();
	//Textura fondo;

	if( !juego.iniciar() ) {
		printf("Failed to initialize!\n");
		return 1;
	}

	VistaMarco personaje(juego.getRenderer());

	//Load media
	if ( !personaje.cargarImagen() ) {
		printf("Failed to load media!\n");
		return 1;
	}

	//Main loop flag
	bool quit = false;
	bool seMovio;
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//Event handler
	SDL_Event e;

	//WHILE APLICACION CORRIENDO
	while( !quit ) {

		//MANEJA LA COLA DE EVENTOS
		while( SDL_PollEvent( &e ) != 0 ) {
			if( e.type == SDL_QUIT ){
				quit = true;
			}
			handleEvent( e , cliente );

		}

		//cambio a nueva posicion
		seMovio = personaje.mover();

		//Center the camera over the personaje
		camera.x = ( personaje.getX() + personaje.getAncho() / 2 ) - SCREEN_WIDTH / 2;
		camera.y = ( personaje.getY() + personaje.getAlto() / 2 ) - SCREEN_HEIGHT / 2;

		//Keep the camera in bounds
		if ( camera.x < 0 )
			camera.x = 0;

		if ( camera.y < 0 )
			camera.y = 0;

		if ( camera.x > LEVEL_WIDTH - camera.w )
			camera.x = LEVEL_WIDTH - camera.w;

		if ( camera.y > LEVEL_HEIGHT - camera.h )
			camera.y = LEVEL_HEIGHT - camera.h;

		//Borro la pantalla
		//DRAWCOLOR ASI PONE TODO EN BLANCO
		SDL_SetRenderDrawColor( juego.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( juego.getRenderer() );

		//Render background
		//fondo.render( 0, 0, &camera );
		personaje.render(seMovio,camera.x,camera.y);
		SDL_RenderPresent( juego.getRenderer() );
	}

	//Free resources and close SDL
	juego.close(personaje);
	cliente.desconectar();

	return 0;
}

