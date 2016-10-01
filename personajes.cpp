#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
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
			renderizador = NULL;
		}

		//Deallocates memory
		~Textura(){
			free();
		}

		//Loads image at specified path
		bool cargarImagen( std::string path){
			//elimino cualquier textura
			free();

			//Aca guardo la ultima
			SDL_Texture* texturaFinal = NULL;

			//Cargo la imagen
			SDL_Surface* imagenCargada = IMG_Load( path.c_str() );
			if( imagenCargada == NULL ){
				printf( "IMPOSIBLE CARGAR IMAGEN %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			}

			else
			{
				//Si la imagen tiene un fondito de color se la saca (creo)
				SDL_SetColorKey( imagenCargada, SDL_TRUE, SDL_MapRGB( imagenCargada->format, 0, 0xFF, 0xFF ) );

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
				renderQuad.w = clip->w+30;
				renderQuad.h = clip->h+40;
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
		const static int ANIMACION_SALTANDO=12;
		SDL_Rect spriteParado[ ANIMACION_PARADO ];
		SDL_Rect spriteCorriendo[ ANIMACION_CORRIENDO ];
		SDL_Rect spriteSaltando[ ANIMACION_SALTANDO ];

		int frameCorriendo = 0;
		int frameParado = 0;
		bool derecha = true;
		bool quieto = true;
		bool saltando = false;
		bool subiendo = false;
		bool bajando = false;
		int frameSaltando=0;

		//velocidad del personaje
		int velx;
		int vely;
		Textura TEXTURA_PERSONAJE;
		bool imagCargada = false;

    public:
		static const int Personaje_VEL = 5;
		static const int Personaje_VEL_Y = 7;
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
		    if( ( posx < 0 ) || ( posx + ancho > SCREEN_WIDTH ) )
		    {
		        posx -= velx;
		    }

			posy += vely;

		    if( ( posy < 0 ) || ( posy + alto > SCREEN_HEIGHT ) )
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

		void animacionParado(){
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) flip = SDL_FLIP_HORIZONTAL;
			SDL_Rect* currentClip = &spriteParado[ frameParado / 9];
			TEXTURA_PERSONAJE.render(posx,posy, currentClip,0,NULL,flip );;

			++frameParado;

			if( frameParado / 9 >= ANIMACION_PARADO )
			{
				frameParado = 0;
			}
			
		}
		void animacionCorrer(){
			//if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
			}
			SDL_Rect* currentClip = &spriteCorriendo[ frameCorriendo /3 ];
			TEXTURA_PERSONAJE.render( posx, posy, currentClip,0,NULL,flip);
			++frameCorriendo;
			if( frameCorriendo /3 >= ANIMACION_CORRIENDO ){
				frameCorriendo = 0;
			}
		}

		int animacionSaltando(){

			SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( renderizador );
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
			}
			SDL_Rect* currentClip = &spriteSaltando[ frameSaltando/2 ];
			TEXTURA_PERSONAJE.render( posx, posy, currentClip,0,NULL,flip);
			
			if (frameSaltando/2  >= ANIMACION_SALTANDO/2){
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
			SDL_RenderPresent( renderizador );
			if( frameSaltando/2 == ANIMACION_SALTANDO ){
				frameSaltando = 0;
				saltando=false;
				bajando= false;
			}
		}

		Textura getTextura(){
			return TEXTURA_PERSONAJE;
		}

		bool cargarImagenSprites(){
			//Loading success flag
			bool success = true;

			//Load sprite sheet texture
			if( !TEXTURA_PERSONAJE.cargarImagen( "imag/marco/marco.png") )
			{
				printf( "Fallo\n" );
				success = false;
			}
			else
			{
				//SPRITE PARADO
				spriteParado[ 0 ].x = 4;
				spriteParado[ 0 ].y = 0;
				spriteParado[ 0 ].w = 30;
				spriteParado[ 0 ].h = 40;

				spriteParado[ 1 ].x =  35;
				spriteParado[ 1 ].y =   0;
				spriteParado[ 1 ].w =  29;
				spriteParado[ 1 ].h = 40;
				
				spriteParado[ 2 ].x = 64;
				spriteParado[ 2 ].y =   0;
				spriteParado[ 2 ].w =  29;
				spriteParado[ 2 ].h = 40;


				//SPRITE CORRIENDO
				spriteCorriendo[ 0 ].x = 107;
				spriteCorriendo[ 0 ].y = 0;
				spriteCorriendo[ 0 ].w = 30;
				spriteCorriendo[ 0 ].h = 40;

				spriteCorriendo[ 1 ].x =  139;
				spriteCorriendo[ 1 ].y =   0;
				spriteCorriendo[ 1 ].w =  28;
				spriteCorriendo[ 1 ].h = 40;
				
				spriteCorriendo[ 2 ].x = 168;
				spriteCorriendo[ 2 ].y =   0;
				spriteCorriendo[ 2 ].w =  32;
				spriteCorriendo[ 2 ].h = 40;

				spriteCorriendo[ 3 ].x = 201;
				spriteCorriendo[ 3 ].y = 0;
				spriteCorriendo[ 3 ].w = 32;
				spriteCorriendo[ 3 ].h = 40;

				spriteCorriendo[ 4 ].x =  235;
				spriteCorriendo[ 4 ].y =   0;
				spriteCorriendo[ 4 ].w =  27;
				spriteCorriendo[ 4 ].h = 40;
				
				spriteCorriendo[ 5 ].x = 266;
				spriteCorriendo[ 5 ].y =   0;
				spriteCorriendo[ 5 ].w =  29;
				spriteCorriendo[ 5 ].h = 40;

				spriteCorriendo[ 6 ].x = 298;
				spriteCorriendo[ 6 ].y = 0;
				spriteCorriendo[ 6 ].w = 32;
				spriteCorriendo[ 6 ].h = 40;

				spriteCorriendo[ 7 ].x =  330;
				spriteCorriendo[ 7 ].y =   0;
				spriteCorriendo[ 7 ].w =  36;
				spriteCorriendo[ 7 ].h = 40;
				
				spriteCorriendo[ 8 ].x = 366;
				spriteCorriendo[ 8 ].y =   0;
				spriteCorriendo[ 8 ].w =  31;
				spriteCorriendo[ 8 ].h = 40;

				//sprites saltando
				spriteSaltando[ 0 ].x=3;
				spriteSaltando[ 0 ].y=189;
				spriteSaltando[ 0 ].w=29;
				spriteSaltando[ 0 ].h=48;

				spriteSaltando[ 1 ].x=32;
				spriteSaltando[ 1 ].y=190;
				spriteSaltando[ 1 ].w=29;
				spriteSaltando[ 1 ].h=46;

				spriteSaltando[ 2 ].x=63;
				spriteSaltando[ 2 ].y=191;
				spriteSaltando[ 2 ].w=29;
				spriteSaltando[ 2 ].h=44;

				spriteSaltando[ 3 ].x=94;
				spriteSaltando[ 3 ].y=192;
				spriteSaltando[ 3 ].w=29;
				spriteSaltando[ 3 ].h=39;

				spriteSaltando[ 4 ].x=124;
				spriteSaltando[ 4 ].y=193;
				spriteSaltando[ 4 ].w=29;
				spriteSaltando[ 4 ].h=37;

				spriteSaltando[ 5 ].x=156;
				spriteSaltando[ 5 ].y=193;
				spriteSaltando[ 5 ].w=29;
				spriteSaltando[ 5 ].h=36;

				spriteSaltando[ 6 ].x=187;
				spriteSaltando[ 6 ].y=193;
				spriteSaltando[ 6 ].w=29;
				spriteSaltando[ 6 ].h=37;

				spriteSaltando[ 7 ].x=218;
				spriteSaltando[ 7 ].y=193;
				spriteSaltando[ 7 ].w=29;
				spriteSaltando[ 7 ].h=41;

				spriteSaltando[ 8 ].x=250;
				spriteSaltando[ 8 ].y=192;
				spriteSaltando[ 8 ].w=29;
				spriteSaltando[ 8 ].h=44;

				spriteSaltando[ 9 ].x=282;
				spriteSaltando[ 9 ].y=194;
				spriteSaltando[ 9 ].w=26;
				spriteSaltando[ 9 ].h=37;

				spriteSaltando[ 10 ].x=338;
				spriteSaltando[ 10 ].y=194;
				spriteSaltando[ 10 ].w=28;
				spriteSaltando[ 10 ].h=37;

				spriteSaltando[ 11 ].x=368;
				spriteSaltando[ 11 ].y=194;
				spriteSaltando[ 11 ].w=31;
				spriteSaltando[ 11 ].h=37;
				imagCargada=true;

			}

			
			return success;
		}

		bool cargarImagenGoku(){
			//Loading success flag
			bool success = true;

			//Load sprite sheet texture
			if( !TEXTURA_PERSONAJE.cargarImagen( "imag/goku/gokuss3cheto.png") )
			{
				printf( "Fallo\n" );
				success = false;
			}
			else
			{
				//SPRITE PARADO
				spriteParado[ 0 ].x = 473;
				spriteParado[ 0 ].y = 186;
				spriteParado[ 0 ].w = 50;
				spriteParado[ 0 ].h = 75;

				spriteParado[ 1 ].x = 528;
				spriteParado[ 1 ].y = 188;
				spriteParado[ 1 ].w = 56;
				spriteParado[ 1 ].h = 72;
				
				spriteParado[ 2 ].x = 590;
				spriteParado[ 2 ].y = 190;
				spriteParado[ 2 ].w = 61;
				spriteParado[ 2 ].h = 70;


				//SPRITE CORRIENDO
				spriteCorriendo[ 0 ].x = 331;
				spriteCorriendo[ 0 ].y = 106;
				spriteCorriendo[ 0 ].w = 40;
				spriteCorriendo[ 0 ].h = 68;

				spriteCorriendo[ 1 ].x =  377;
				spriteCorriendo[ 1 ].y =   106;
				spriteCorriendo[ 1 ].w =  67;
				spriteCorriendo[ 1 ].h = 74;
				
				spriteCorriendo[ 2 ].x =  377;
				spriteCorriendo[ 2 ].y =   106;
				spriteCorriendo[ 2 ].w =  67;
				spriteCorriendo[ 2 ].h = 74;

				spriteCorriendo[ 3 ].x =  377;
				spriteCorriendo[ 3 ].y =   106;
				spriteCorriendo[ 3 ].w =  67;
				spriteCorriendo[ 3 ].h = 74;

				spriteCorriendo[ 4 ].x =  377;
				spriteCorriendo[ 4 ].y =   106;
				spriteCorriendo[ 4 ].w =  67;
				spriteCorriendo[ 4 ].h = 74;
				
				spriteCorriendo[ 5 ].x =  377;
				spriteCorriendo[ 5 ].y =   106;
				spriteCorriendo[ 5 ].w =  67;
				spriteCorriendo[ 5 ].h = 74;

				spriteCorriendo[ 6 ].x =  377;
				spriteCorriendo[ 6 ].y =   106;
				spriteCorriendo[ 6 ].w =  67;
				spriteCorriendo[ 6 ].h = 74;

				spriteCorriendo[ 7 ].x =  377;
				spriteCorriendo[ 7 ].y =   106;
				spriteCorriendo[ 7 ].w =  67;
				spriteCorriendo[ 7 ].h = 74;
				
				spriteCorriendo[ 8 ].x = 331;
				spriteCorriendo[ 8 ].y = 106;
				spriteCorriendo[ 8 ].w = 40;
				spriteCorriendo[ 8 ].h = 68;

				//sprites saltando
				spriteSaltando[ 0 ].x=12;
				spriteSaltando[ 0 ].y=13;
				spriteSaltando[ 0 ].w=45;
				spriteSaltando[ 0 ].h=80;

				spriteSaltando[ 1 ].x=75;
				spriteSaltando[ 1 ].y=14;
				spriteSaltando[ 1 ].w=46;
				spriteSaltando[ 1 ].h=50;

				spriteSaltando[ 2 ].x=127;
				spriteSaltando[ 2 ].y=13;
				spriteSaltando[ 2 ].w=50;
				spriteSaltando[ 2 ].h=47;

				spriteSaltando[ 3 ].x=186;
				spriteSaltando[ 3 ].y=10;
				spriteSaltando[ 3 ].w=46;
				spriteSaltando[ 3 ].h=50;

				spriteSaltando[ 4 ].x=238;
				spriteSaltando[ 4 ].y=12;
				spriteSaltando[ 4 ].w=50;
				spriteSaltando[ 4 ].h=48;

				spriteSaltando[ 5 ].x=75;
				spriteSaltando[ 5 ].y=14;
				spriteSaltando[ 5 ].w=46;
				spriteSaltando[ 5 ].h=50;

				spriteSaltando[ 6 ].x=127;
				spriteSaltando[ 6 ].y=13;
				spriteSaltando[ 6 ].w=50;
				spriteSaltando[ 6 ].h=47;

				spriteSaltando[ 7 ].x=186;
				spriteSaltando[ 7 ].y=10;
				spriteSaltando[ 7 ].w=46;
				spriteSaltando[ 7 ].h=50;

				spriteSaltando[ 8 ].x=238;
				spriteSaltando[ 8 ].y=12;
				spriteSaltando[ 8 ].w=50;
				spriteSaltando[ 8 ].h=48;

				spriteSaltando[ 9 ].x=75;
				spriteSaltando[ 9 ].y=14;
				spriteSaltando[ 9 ].w=46;
				spriteSaltando[ 9 ].h=50;7;

				spriteSaltando[ 10 ].x=186;
				spriteSaltando[ 10 ].y=10;
				spriteSaltando[ 10 ].w=46;
				spriteSaltando[ 10 ].h=50;

				spriteSaltando[ 11 ].x=292;
				spriteSaltando[ 11 ].y=12;
				spriteSaltando[ 11 ].w=60;
				spriteSaltando[ 11 ].h=86;
				imagCargada=true;

			}

			
			return success;
		}

		bool cargarImagenMegaman(){
			//Loading success flag
			bool success = true;

			//Load sprite sheet texture
			if( !TEXTURA_PERSONAJE.cargarImagen( "imag/megaman/megamanx.jpg") )
			{
				printf( "Fallo\n" );
				success = false;
			}
			else
			{
				//SPRITE PARADO
				spriteParado[ 0 ].x = 460;
				spriteParado[ 0 ].y = 23;
				spriteParado[ 0 ].w = 37;
				spriteParado[ 0 ].h = 48;

				spriteParado[ 1 ].x = 505;
				spriteParado[ 1 ].y = 22;
				spriteParado[ 1 ].w = 35;
				spriteParado[ 1 ].h = 49;
				
				spriteParado[ 2 ].x = 550;
				spriteParado[ 2 ].y = 22;
				spriteParado[ 2 ].w = 35;
				spriteParado[ 2 ].h = 50;


				//SPRITE CORRIENDO
				spriteCorriendo[ 0 ].x = 5;
				spriteCorriendo[ 0 ].y = 100;
				spriteCorriendo[ 0 ].w = 34;
				spriteCorriendo[ 0 ].h = 44;

				spriteCorriendo[ 1 ].x =  50;
				spriteCorriendo[ 1 ].y =   100;
				spriteCorriendo[ 1 ].w =  31;
				spriteCorriendo[ 1 ].h = 47;
				
				spriteCorriendo[ 2 ].x =  90;
				spriteCorriendo[ 2 ].y =   100;
				spriteCorriendo[ 2 ].w =  34;
				spriteCorriendo[ 2 ].h = 43;

				spriteCorriendo[ 3 ].x =  130;
				spriteCorriendo[ 3 ].y =   100;
				spriteCorriendo[ 3 ].w =  38;
				spriteCorriendo[ 3 ].h = 46;

				spriteCorriendo[ 4 ].x =  170;
				spriteCorriendo[ 4 ].y =   100;
				spriteCorriendo[ 4 ].w =  45;
				spriteCorriendo[ 4 ].h = 40;
				
				spriteCorriendo[ 5 ].x =  225;
				spriteCorriendo[ 5 ].y =   100;
				spriteCorriendo[ 5 ].w =  53;
				spriteCorriendo[ 5 ].h = 34;

				spriteCorriendo[ 6 ].x =  225;
				spriteCorriendo[ 6 ].y =   100;
				spriteCorriendo[ 6 ].w =  53;
				spriteCorriendo[ 6 ].h = 34;

				spriteCorriendo[ 7 ].x =  170;
				spriteCorriendo[ 7 ].y =   100;
				spriteCorriendo[ 7 ].w =  45;
				spriteCorriendo[ 7 ].h = 40;
				
				spriteCorriendo[ 8 ].x =  130;
				spriteCorriendo[ 8 ].y =   100;
				spriteCorriendo[ 8 ].w =  38;
				spriteCorriendo[ 8 ].h = 46;

				//sprites saltando
				spriteSaltando[ 0 ].x=5;
				spriteSaltando[ 0 ].y=285;
				spriteSaltando[ 0 ].w=26;
				spriteSaltando[ 0 ].h=51;

				spriteSaltando[ 1 ].x=45;
				spriteSaltando[ 1 ].y=285;
				spriteSaltando[ 1 ].w=23;
				spriteSaltando[ 1 ].h=55;

				spriteSaltando[ 2 ].x=85;
				spriteSaltando[ 2 ].y=285;
				spriteSaltando[ 2 ].w=23;
				spriteSaltando[ 2 ].h=58;

				spriteSaltando[ 3 ].x=120;
				spriteSaltando[ 3 ].y=285;
				spriteSaltando[ 3 ].w=27;
				spriteSaltando[ 3 ].h=56;

				spriteSaltando[ 4 ].x=120;
				spriteSaltando[ 4 ].y=285;
				spriteSaltando[ 4 ].w=27;
				spriteSaltando[ 4 ].h=56;3;

				spriteSaltando[ 5 ].x=120;
				spriteSaltando[ 5 ].y=285;
				spriteSaltando[ 5 ].w=27;
				spriteSaltando[ 5 ].h=56;

				spriteSaltando[ 6 ].x=120;
				spriteSaltando[ 6 ].y=285;
				spriteSaltando[ 6 ].w=27;
				spriteSaltando[ 6 ].h=56;

				spriteSaltando[ 7 ].x=120;
				spriteSaltando[ 7 ].y=285;
				spriteSaltando[ 7 ].w=27;
				spriteSaltando[ 7 ].h=56;

				spriteSaltando[ 8 ].x=155;
				spriteSaltando[ 8 ].y=285;
				spriteSaltando[ 8 ].w=34;
				spriteSaltando[ 8 ].h=53;

				spriteSaltando[ 9 ].x=195;
				spriteSaltando[ 9 ].y=285;
				spriteSaltando[ 9 ].w=32;
				spriteSaltando[ 9 ].h=54;

				spriteSaltando[ 10 ].x=235;
				spriteSaltando[ 10 ].y=285;
				spriteSaltando[ 10 ].w=30;
				spriteSaltando[ 10 ].h=57;

				spriteSaltando[ 11 ].x=272;
				spriteSaltando[ 11 ].y=285;
				spriteSaltando[ 11 ].w=31;
				spriteSaltando[ 11 ].h=54;
				imagCargada=true;

			}

			
			return success;

		}

		

		void liberarTextura(){
			TEXTURA_PERSONAJE.free();
		}
		bool estaSaltando(){
			return saltando;
		}
};

//class Posicion{}

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
						//Initialize renderer color
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

	if( !programa.iniciar() ){
		printf( "Failed to initialize!\n" );

	}
	else
	{
		//Load media
		if( !personaje.cargarImagenMegaman())
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

				//Borro la pantalla
				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );

				//cambio a nueva posicion
				seMovio = personaje.mover();

				if (seMovio==true){
					personaje.animacionCorrer();
				} 
				else {
					personaje.animacionParado();
				}

				if (personaje.estaSaltando()) personaje.animacionSaltando();

				//Update screen
				SDL_RenderPresent( renderizador );
			}
		}
	}

	//Free resources and close SDL
	programa.close(personaje);

	return 0;
}