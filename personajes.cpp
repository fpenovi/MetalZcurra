#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 4500;
const int LEVEL_HEIGHT = 480;
SDL_Renderer* renderizador;
SDL_Window* ventana;


//Texture wrapper class
class Textura
{
	private:
		//The actual hardware texture
		SDL_Texture* textura;
		int ancho;
		int alto;
	public:
		Textura(){
			textura = NULL;
			ancho = 0;
			alto = 0;
		}

		//Deallocates memory
		~Textura(){
			free();
		}

		//Cargar una imagen en la textura
		bool cargarImagen( std::string path){

			//elimino cualquier textura
			free();

			//Aca voy a guardar la ultima
			SDL_Texture* texturaFinal = NULL;

			//Cargo la imagen
			SDL_Surface* imagenCargada = IMG_Load( path.c_str() );
			if( imagenCargada == NULL ){
				printf( "IMPOSIBLE CARGAR IMAGEN %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			}
			else
			{
				//Si la imagen tiene un fondito de color se la saca
				SDL_SetColorKey( imagenCargada, SDL_TRUE, SDL_MapRGB( imagenCargada->format,0xFF, 0xFF, 0xFF ) );

				//Create texture from surface pixels
		        texturaFinal = SDL_CreateTextureFromSurface( renderizador, imagenCargada );
				if( texturaFinal == NULL )
				{
					printf( "IMPOSIBLE CREAR IMAGEN DESDE %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
				}
				else
				{
					//Get image dimensions
					ancho = imagenCargada->w;
					alto = imagenCargada->h;
				}

				//Get rid of old loaded surface
				SDL_FreeSurface( imagenCargada );
		

			//Return success
			textura = texturaFinal;
			return textura != NULL;
			}
		}

		//Deallocates texture
		void free(){
			//Libera textura si existe
			if( textura != NULL ){
				SDL_DestroyTexture( textura );
				textura = NULL;
				ancho = 0;
				alto = 0;
			}
		}
		
		//Renderizar textura en un punto
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE ){
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { x, y, ancho, alto };

			//Set clip rendering dimensions
			if( clip != NULL ){
				renderQuad.w = clip->w;
				renderQuad.h = clip->h;
			}

			//ESTO LO PONE EN PANTALLA
			SDL_RenderCopyEx( renderizador, textura, clip, &renderQuad, angle, center, flip );
		}

		int getAncho(){
			return ancho;
		}

		int getAlto(){
			return alto;
		}
};

class Personaje
{

    private:

		int posx, posy;
		int ancho, alto;
		const static int ANIMACION_PARADO=3;
		const static int ANIMACION_CORRIENDO=9;
		const static int ANIMACION_SALTANDO=10;
		SDL_Rect spriteParado[ ANIMACION_PARADO ];
		SDL_Rect spriteCorriendo[ ANIMACION_CORRIENDO ];
		SDL_Rect spriteSaltando[ ANIMACION_SALTANDO ];

		int frameCorriendo;
		int frameParado;
		bool derecha;
		bool quieto;
		bool saltando ;
		bool subiendo;
		bool bajando;
		int frameSaltando;

		//velocidad del personaje
		int velx;
		int vely;
		Textura TEXTURA_PERSONAJE_PARADO;
		Textura TEXTURA_PERSONAJE_SALTANDO;
		Textura TEXTURA_PERSONAJE_CORRIENDO;
		const static int Personaje_VEL = 5;
		const static int Personaje_VEL_Y = 4;

    public:
		
		//Initializes the variables
		Personaje(){
			posx = 0;
			posy = 240;
			ancho=60;
			alto=80;
			frameCorriendo=0;
			frameParado=0;
			frameSaltando=0;
			velx = 0;
			vely = 0;
			derecha = true;
			quieto = true;
			saltando = false;
			subiendo = false;
			bajando = false;
			frameSaltando=0;
		}

		//maneja los eventos
		void handleEvent( SDL_Event& e )
		{
		    //If a key was pressed
			if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
		    {
		        //Adjust the velocity
		        switch( e.key.keysym.sym )
		        {
		            case SDLK_LEFT: velx -= Personaje_VEL;derecha = false;break;
		            case SDLK_RIGHT: velx += Personaje_VEL;derecha = true;break;
		            case SDLK_UP: 
		            	if (!saltando) saltando=true; subiendo=true; break;
		        }
		    }

		    //If a key was released
		    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
		    {
		        //Adjust the velocity
		        switch( e.key.keysym.sym )
		        {
		            case SDLK_LEFT: velx += Personaje_VEL; break;
		            case SDLK_RIGHT: velx -= Personaje_VEL; break;
		            case SDLK_UP:break;
		        }
		    }
		}

		bool estaQuieto(){
			return quieto;
		}

		//Moves the Personaje
		bool mover()
		{
			int pos1 = posx;
			int pos2 = posy;
		    //moverlo a derecha o izquierda
		    posx += velx;

		    //Que no salga de la pantalla
		    if( ( posx < 0 ) || ( posx + ancho > LEVEL_WIDTH ) )
		    {
		        posx -= velx;
		    }

			posy += vely;

		    if( ( posy < 0 ) || ( posy + alto > LEVEL_HEIGHT ) )
		    {
		        //Move back
		        posy -= vely;
		    }
		    if (pos1 == posx && pos2 ==  posy) {
		    	quieto = true;
		    	return false;
		    }
			return true;
		}

		void render(bool seMovio, int camx, int camy){
			if (seMovio==true){
					animacionCorrer(camx,camy);
				} 
				else {
					animacionParado(camx,camy);
				}
				if (estaSaltando()) animacionSaltando(camx,camy);
		}

		void animacionParado(int camx,int camy){
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) flip = SDL_FLIP_HORIZONTAL;
			SDL_Rect* currentClip = &spriteParado[ frameParado / 9];
			TEXTURA_PERSONAJE_PARADO.render(posx-camx,posy-camy, currentClip,0,NULL,flip );;

			++frameParado;

			if( frameParado / 9 >= ANIMACION_PARADO )
			{
				frameParado = 0;
			}
			
		}
		void animacionCorrer(int camx,int camy){
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
			}
			SDL_Rect* currentClip = &spriteCorriendo[ frameCorriendo /3 ];
			TEXTURA_PERSONAJE_CORRIENDO.render( posx-camx, posy-camy, currentClip,0,NULL,flip);
			++frameCorriendo;
			if( frameCorriendo /3 >= ANIMACION_CORRIENDO ){
				frameCorriendo = 0;
			}
		}

		int animacionSaltando(int camx,int camy){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
			}
			SDL_Rect* currentClip = &spriteSaltando[ frameSaltando/4 ];
			TEXTURA_PERSONAJE_SALTANDO.render( posx-camx, posy-camy, currentClip,0,NULL,flip);
			
			if (frameSaltando/4  >= ANIMACION_SALTANDO/2){
				subiendo=false;
				bajando= true;
				posy+=Personaje_VEL_Y;
			}
			else {
				subiendo=true;
				bajando=false;
				posy-=Personaje_VEL_Y;
			}
			++frameSaltando;
			if( frameSaltando/4 == ANIMACION_SALTANDO ){
				frameSaltando = 0;
				saltando=false;
				bajando= false;
			}
		}

		bool cargarImagen(){
			//Loading success flag
			bool success = true;
			int i;

			//Load sprite sheet texture
			if( !TEXTURA_PERSONAJE_PARADO.cargarImagen( "imag/marco/quieto3.png") )
			{
				printf( "Fallo sprite parado\n" );
				success = false;
			}
			else
			{
				//SPRITE PARADO
				for (i = 0;i<ANIMACION_PARADO;i++){
					spriteParado[ i ].x = i*60;
					spriteParado[ i ].y = 0;
					spriteParado[ i ].w = 60;
					spriteParado[ i ].h = 80;
				}
			}

			if( !TEXTURA_PERSONAJE_CORRIENDO.cargarImagen( "imag/marco/corriendo2.png") )
			{
				printf( "Fallo sprite corriendo\n" );
				success = false;
			}
			else{
				for (i = 0;i<ANIMACION_CORRIENDO;i++){
					spriteCorriendo[ i ].x = i*60;
					spriteCorriendo[ i ].y = 0;
					spriteCorriendo[ i ].w = 60;
					spriteCorriendo[ i ].h = 80;
				}
			}

			if( !TEXTURA_PERSONAJE_SALTANDO.cargarImagen( "imag/marco/saltando3.png") )
			{
				printf( "Fallo sprite saltando\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_SALTANDO;i++){
					spriteSaltando[ i ].x = i*60;
					spriteSaltando[ i ].y = 0;
					spriteSaltando[ i ].w = 60;
					spriteSaltando[ i ].h = 80;
				}
			}
			return success;
		}
		

		void liberarTextura(){
			TEXTURA_PERSONAJE_SALTANDO.free();
			TEXTURA_PERSONAJE_CORRIENDO.free();
			TEXTURA_PERSONAJE_PARADO.free();

		}
		bool estaSaltando(){
			return saltando;
		}
		int getX(){
			return posx;
		}
		int getY(){
			return posy;
		}
		int getAncho(){
			return ancho;
		}
		int getAlto(){
			return alto;
		}
};

class Posicion
{
	private:
		int x;
		int y;
		int alto;
		int ancho;
	public:
		void asignarPos(int x2,int y2){
			x=x2;
			y=y2;
		}
		int getX(){
			return x;
		}
		int getY(){
			return y;
		}
};

class Programa
{
	public:

		Programa(){
			renderizador = NULL;
			ventana = NULL;
		}

		void close(Personaje personaje)
		{
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

		bool iniciar()
		{
			//flag
			bool success = true;

			//SDL
			if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
				printf( "NO SE INICIALIZO SDL! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else{
				//Set texture filtering to linear
				if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
					printf( "Warning: Linear texture filtering not enabled!" );
				}

				//VENTANA
				ventana = SDL_CreateWindow( "PERSONAJE1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
				if( ventana == NULL )
				{
					printf( "NO SE PUDO CREAR LA VENTANA! SDL Error: %s\n", SDL_GetError() );
					success = false;
				}
				else
				{
					//Create vsynced renderer for window
					renderizador = SDL_CreateRenderer( ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
					if( renderizador == NULL )
					{
						printf( "NO SE PUDO HACER EL RENDER! SDL Error: %s\n", SDL_GetError() );
						success = false;
					}
					else
					{
						//DRAWCOLOR ASI PONE TODO EN BLANCO
						SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );

						//Initialize PNG loading
						int imgFlags = IMG_INIT_PNG;
						if( !( IMG_Init( imgFlags ) & imgFlags ) )
						{
							printf( "NO SE PUDO INICIARLIZAR LA IMAGEN! SDL_image Error: %s\n", IMG_GetError() );
							success = false;
						}
					}
				}
			}

			return success;
		}
};

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	Programa programa;
	Personaje personaje;
	Textura fondo;

	if( !programa.iniciar() ){
		printf( "Failed to initialize!\n" );

	}
	else
	{
		//Load media
		if( !personaje.cargarImagen() || !fondo.cargarImagen("imag/background/bg1.png"))
		{
			printf( "Failed to load media!\n" );
		}
		else
		{

			//Main loop flag
			bool quit = false;
			int flagCorriendo;
			int flagQuieto;
			bool seMovio;
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//Event handler
			SDL_Event e;

			//WHILE APLICACION CORRIENDO
			while( !quit )
			{

				//MANEJA LA COLA DE EVENTOS
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT ){
						quit = true;
					}
					personaje.handleEvent( e );

				}

				//cambio a nueva posicion
				seMovio = personaje.mover();

				//Center the camera over the personaje
				camera.x = ( personaje.getX() + personaje.getAncho() / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( personaje.getY() + personaje.getAlto() / 2 ) - SCREEN_HEIGHT / 2;

				//Keep the camera in bounds
				if( camera.x < 0 )
				{ 
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > LEVEL_WIDTH - camera.w )
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if( camera.y > LEVEL_HEIGHT - camera.h )
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

				//Borro la pantalla
				//DRAWCOLOR ASI PONE TODO EN BLANCO
				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );

				//Render background
				fondo.render( 0, 0, &camera );
				personaje.render(seMovio,camera.x,camera.y);
				SDL_RenderPresent( renderizador );
			}
		}
	}

	//Free resources and close SDL
	programa.close(personaje);

	return 0;
}