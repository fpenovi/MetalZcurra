

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <SDL2/SDL_ttf.h>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
SDL_Renderer* renderizador;
SDL_Window* ventana;
TTF_Font* gFont;


//Texture wrapper class
class Textura
{
	private:
		//The actual hardware texture
		SDL_Texture* textura;
		int ancho;
		int alto;
		int scroll;
		SDL_Rect camara;
	public:
		Textura(){
			textura = NULL;
			ancho = 0;
			alto = 0;
			scroll=0;
			camara={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
		}

		//Deallocates memory
		~Textura(){
			free();
		}

		//Cargar una imagen en la textura
		bool cargarImagen(char* path){

			//elimino cualquier textura
			free();

			//Aca voy a guardar la ultima
			SDL_Texture* texturaFinal = NULL;

			//Cargo la imagen
			SDL_Surface* imagenCargada = IMG_Load( path);
			if( imagenCargada == NULL ){
				printf( "IMPOSIBLE CARGAR IMAGEN %s! SDL_image Error: %s\n", path, IMG_GetError() );
			}
			else
			{
				//Si la imagen tiene un fondito de color se la saca (ahora esta en blanco) pone transpar
				SDL_SetColorKey( imagenCargada, SDL_TRUE, SDL_MapRGB( imagenCargada->format,0xFF, 0xFF, 0xFF ) );

				//Create texture from surface pixels
		        texturaFinal = SDL_CreateTextureFromSurface( renderizador, imagenCargada );
				if( texturaFinal == NULL ){
					printf( "IMPOSIBLE CREAR IMAGEN DESDE %s! SDL Error: %s\n", path, SDL_GetError() );
				}
				else{
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

		void setBlendMode( SDL_BlendMode blending )
		{
		    //Set blending function
		    SDL_SetTextureBlendMode( textura, blending );
		}
        
		void setAlpha( Uint8 alpha )
		{
		    //Modulate texture alpha
		    SDL_SetTextureAlphaMod( textura, alpha );
		}

		bool loadFromText( string texto, SDL_Color colorTexto)
		{
			//Get rid of preexisting texture
			free();

			//Render text surface
			SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, texto.c_str(), colorTexto );
			if( textSurface != NULL )
			{
				//Create texture from surface pixels
		        textura = SDL_CreateTextureFromSurface( renderizador, textSurface );
				if( textura == NULL )
				{
					printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
				}
				else
				{
					//Get image dimensions
					ancho = textSurface->w;
					alto = textSurface->h;
				}

				//Get rid of old surface
				SDL_FreeSurface( textSurface );
			}
			else
			{
				printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
			}

			
			//Return success
			return textura != NULL;
		}

		void setColor( Uint8 red, Uint8 green, Uint8 blue )
		{
			//Modulate texture
			SDL_SetTextureColorMod( textura, red, green, blue );
		}
};

class Arma 
{
	protected:
		const static int ANIMACION_PARADO=4;
		const static int ANIMACION_CORRIENDO=18;
		const static int ANIMACION_SALTANDO=12;
		const static int ANIMACION_DISPARANDO_GUN=10;
		const static int ANIMACION_DISPARANDO_SHOTGUN=4;
		int ANIMACION_ACTUAL=10; //empieza siendo 10 por la gun

		int frameDisparando=0;
		bool disparando=false;


		Textura TEXTURA_ARMA_PARADO;
		SDL_Rect spriteParado[ ANIMACION_PARADO ];

		Textura TEXTURA_ARMA_CORRIENDO;
		SDL_Rect spriteCorriendo[ ANIMACION_CORRIENDO ];

		Textura TEXTURA_ARMA_SALTANDO;
		SDL_Rect spriteSaltando[ ANIMACION_SALTANDO ];

		Textura TEXTURA_ARMA_DISPARANDO;
		SDL_Rect spriteDisparando[ ANIMACION_DISPARANDO_GUN];


	public:
		Arma(){
			cargarImagen("imag/sprites/gunStill.png",
				"imag/sprites/gunRun.png",
				"imag/sprites/gunJump.png",
				"imag/sprites/gunShoot.png");
		}

		void renderParado(int x, int y, int frame, SDL_RendererFlip flip){
			SDL_Rect* currentClip = &spriteParado[frame];
			TEXTURA_ARMA_PARADO.render(x,y,currentClip,0,NULL,flip );
		}
		void renderCorriendo(int x, int y, int frame, SDL_RendererFlip flip){
			SDL_Rect* currentClip = &spriteCorriendo[frame];
			TEXTURA_ARMA_CORRIENDO.render(x,y,currentClip,0,NULL,flip );
		}
		void renderSaltando(int x, int y, int frame, SDL_RendererFlip flip){
			SDL_Rect* currentClip = &spriteSaltando[frame];
			TEXTURA_ARMA_SALTANDO.render(x,y,currentClip,0,NULL,flip );
		}
		bool renderDisparando(int x, int y, SDL_RendererFlip flip){
			SDL_Rect* currentClip = &spriteDisparando[frameDisparando/4];
			TEXTURA_ARMA_DISPARANDO.render(x,y,currentClip,0,NULL,flip );
			++frameDisparando;
			if( frameDisparando /4 >= ANIMACION_ACTUAL ){
				frameDisparando = 0;
				disparando=false;
			}
			cout << frameDisparando << endl;
			return disparando;
		}

		bool cargarImagen(char* pathParado,char* pathCorriendo,char* pathSaltando,char* pathDisparando){
			bool success = true;
			int i;

			if( !TEXTURA_ARMA_PARADO.cargarImagen( pathParado) )
			{
				printf( "Fallo sprite arma parado\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_PARADO;i++){
					spriteParado[ i ].x = i*149;
					spriteParado[ i ].y = 0;
					spriteParado[ i ].w = 149;
					spriteParado[ i ].h = 62;
				}
			}

			if( !TEXTURA_ARMA_CORRIENDO.cargarImagen( pathCorriendo) )
			{
				printf( "Fallo sprite arma corriendo\n" );
				success = false;
			}
			else{
				for (i = 0;i<ANIMACION_CORRIENDO;i++){
					spriteCorriendo[ i ].x = i*149;
					spriteCorriendo[ i ].y = 0;
					spriteCorriendo[ i ].w = 149;
					spriteCorriendo[ i ].h = 64;
				}
			}

			if( !TEXTURA_ARMA_SALTANDO.cargarImagen( pathSaltando) )
			{
				printf( "Fallo sprite arma saltando\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_SALTANDO;i++){
					spriteSaltando[ i ].x = i*149;
					spriteSaltando[ i ].y = 0;
					spriteSaltando[ i ].w = 149;
					spriteSaltando[ i ].h = 79;
				}
			}

			if( !TEXTURA_ARMA_DISPARANDO.cargarImagen( pathDisparando) )
			{
				printf( "Fallo sprite disparando\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_ACTUAL;i++){
					spriteDisparando[ i ].x = i*149;
					spriteDisparando[ i ].y = 0;
					spriteDisparando[ i ].w = 149;
					spriteDisparando[ i ].h = 62;
				}
			}

			return success;
		}

		void liberar(){
			TEXTURA_ARMA_DISPARANDO.free();
			TEXTURA_ARMA_CORRIENDO.free();
			TEXTURA_ARMA_SALTANDO.free();
			TEXTURA_ARMA_PARADO.free();
		}
		void ponerShotgun(){
			cout << "aca" << endl;
			liberar();
			ANIMACION_ACTUAL=ANIMACION_DISPARANDO_SHOTGUN;
			cargarImagen("imag/sprites/shotgunStill.png",
				"imag/sprites/shotgunRun.png",
				"imag/sprites/shotgunJump.png",
				"imag/sprites/shotgunShoot.png");
		}

		void ponerGun(){
			cout << "acagun" << endl;
			liberar();
			ANIMACION_ACTUAL=ANIMACION_DISPARANDO_GUN;
			cargarImagen("imag/sprites/gunStill.png",
				"imag/sprites/gunRun.png",
				"imag/sprites/gunJump.png",
				"imag/sprites/gunShoot.png");
		}
		void disparar(){
			disparando=true;
		}
};

class Personaje
{

    private:

		int posx, posy;
		int ancho, alto;
		const static int ANIMACION_PARADO=4;
		const static int ANIMACION_CORRIENDO=18;
		const static int ANIMACION_SALTANDO=12;
		const static int ANIMACION_DISPARANDO_GUN=4;


		Textura TEXTURA_PERSONAJE_PARADO_PIES;
		//Textura TEXTURA_PERSONAJE_PARADO_TORSO;
		//SDL_Rect spriteParadoTorso[ ANIMACION_PARADO ];
		SDL_Rect spriteParadoPies[ ANIMACION_PARADO ];

		Textura TEXTURA_PERSONAJE_CORRIENDO_PIES;
		//Textura TEXTURA_PERSONAJE_CORRIENDO_TORSO;
		//SDL_Rect spriteCorriendoTorso[ ANIMACION_CORRIENDO ];
		SDL_Rect spriteCorriendoPies[ ANIMACION_CORRIENDO ];

		Textura TEXTURA_PERSONAJE_SALTANDO_PIES;
		//Textura TEXTURA_PERSONAJE_SALTANDO_TORSO;
		//SDL_Rect spriteSaltandoTorso[ ANIMACION_SALTANDO ];
		SDL_Rect spriteSaltandoPies[ ANIMACION_SALTANDO ];



		int frameCorriendo;
		int frameParado;
		int frameSaltando;
		int frameDisparando;

		int subiendo;

		bool derecha;
		bool saltando;
		
		
		bool salto=false;
		int posCamara;

		//velocidad del personaje
		int velx;
		int vely;


		const static int Personaje_VEL = 7;
		const static int Personaje_VEL_Y = 4;
		bool muerto;
		bool disparar;

		Arma arma=Arma();

    public:
		
		//Initializes the variables
		Personaje(){
			posx = 0;
			posy = 360;
			ancho=60;
			alto=80;
			frameCorriendo=0;
			frameParado=0;
			frameSaltando=0;
			frameDisparando=0;
			velx = 0;
			vely = 0;
			derecha = true;
			saltando = false;
			subiendo = 0;
			frameSaltando=0;
			posCamara=0;
			muerto = false;
			disparar=false;
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
		            	if (!saltando) saltando=true; subiendo=1; break;
		            	if (subiendo==1) vely-=Personaje_VEL_Y;
		            	else if(subiendo == 2) vely+=Personaje_VEL_Y;
		            	else if(subiendo == 0) saltando = false;
		            case SDLK_f:
		            	disparar=true;
		            	arma.disparar();
		            	break;
		            case SDLK_g:
		            	arma.ponerGun();
		            	break;
		            case SDLK_s:
		            	arma.ponerShotgun();
		            	break;
		        }
		    }

		    //If a key was released
		    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
		    {
		        //Adjust the velocity
		        switch( e.key.keysym.sym )
		        {
		            case SDLK_LEFT: velx+=Personaje_VEL; break;
		            case SDLK_RIGHT: velx-=Personaje_VEL; break;
		            case SDLK_UP: break;
		            case SDLK_f: break;
		            case SDLK_s: break;
		            case SDLK_g: break;
		        }

		    }
		}

		bool mover(int anchoMax)
		{	
			int pos1 = posx;
			int pos2 = posy;
		    //moverlo a derecha o izquierda
		    posx += velx;
		    posCamara+=velx;

		    if( ( posCamara < 0 )  || ( posCamara + ancho > SCREEN_WIDTH*3/4 ) )
		    {
		        posCamara -= velx;

		    };
		    if( ( posx < 0 ) ) //|| ( posx + ancho > SCREEN_WIDTH ) )
		    {
		        posx -= velx;
		    }

			posy += vely;

		    if( ( posy < 0 ) )
		    {
		        //Move back
		        posy -= vely;
		    }

		    if (pos1 == posx && pos2 ==  posy) {
		    	return false;
		    }

		    //no cambio posx, porque la camara depende de posx, si el personaje
		    //se mueve para atras, la camara no se tiene que mover
		    //la camara tampoco se tiene que mover si el personaje
		    // no la esta empujando, la tiene que empujar cuando llega a la mitad de la pantalla
			if (velx < 0 || posCamara+ancho +1!= SCREEN_WIDTH*3/4){
		    	posx -= velx;

		    }

			return true;
		}

		void render(bool seMovio){
			if (estaSaltando()) {
				animacionSaltandoPiernas();
				if (disparar) animacionDisparar();
				else animacionSaltandoTorso();
				return;
			}

			if (seMovio==true) {
				animacionCorrerPiernas();
				if (disparar) animacionDisparar();
				else animacionCorrerTorso();
			}
			else {
				animacionParadoPiernas();
				if (disparar) animacionDisparar();
				else animacionParadoTorso();
			}
		}

		void animacionParadoPiernas(){
			
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x=posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=80;
			}

			SDL_Rect* currentClipPies = &spriteParadoPies[ frameParado / 8];
			TEXTURA_PERSONAJE_PARADO_PIES.render(x,posy+48, currentClipPies,0,NULL,flip );

			++frameParado;
			if( frameParado / 8 >= ANIMACION_PARADO )
			{
				frameParado = 0;
			}	
		}

		void animacionParadoTorso(){
			if (saltando) return;
			int x = posCamara;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=80;
			}
			arma.renderParado(x,posy,frameParado/8,flip);
		}

		void animacionCorrerPiernas(){
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=80;
			}

			SDL_Rect* currentClipPies = &spriteCorriendoPies[ frameCorriendo/3 ];
			TEXTURA_PERSONAJE_CORRIENDO_PIES.render( x, posy+45, currentClipPies,0,NULL,flip);

			++frameCorriendo;
			if( frameCorriendo /3 >= ANIMACION_CORRIENDO ){
				frameCorriendo = 0;
			}
		}

		void animacionCorrerTorso(){
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=80;
			}
			arma.renderCorriendo(x,posy,frameCorriendo/3,flip);
		}

		int animacionSaltandoPiernas(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=80;
			}
			SDL_Rect* currentClipPies = &spriteSaltandoPies[ frameSaltando/4 ];
			TEXTURA_PERSONAJE_SALTANDO_PIES.render( x, posy+45, currentClipPies,0,NULL,flip);

			if (frameSaltando/4  >= ANIMACION_SALTANDO/2){
				subiendo=2;
				posy+=Personaje_VEL_Y;
			}
			else {
				subiendo=1;
				posy-=Personaje_VEL_Y;
			}
			++frameSaltando;
			if( frameSaltando/4 == ANIMACION_SALTANDO ){
				frameSaltando = 0;
				saltando=false;
				subiendo=0;
			}
		}

		int animacionSaltandoTorso(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=80;
			}
			arma.renderSaltando(x,posy,frameSaltando/4,flip);
		}

		void animacionDisparar(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=80;
			}

			if (!arma.renderDisparando(x,posy,flip)) disparar=false;
		}

		bool cargarImagen(){
			//Loading success flag
			bool success = true;
			int i;
			//Load sprite sheet texture
			if( !TEXTURA_PERSONAJE_PARADO_PIES.cargarImagen( "imag/sprites/stillLegs.png") )
			{
				printf( "Fallo sprite parado pies\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_PARADO;i++){
					spriteParadoPies[ i ].x = i*149;
					spriteParadoPies[ i ].y = 0;
					spriteParadoPies[ i ].w = 149;
					spriteParadoPies[ i ].h = 34;
				}
			}
			/*if( !TEXTURA_PERSONAJE_PARADO_TORSO.cargarImagen( "imag/sprites/stillGun.png") )
			{
				printf( "Fallo sprite parado torso\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_PARADO;i++){
					spriteParadoTorso[ i ].x = i*149;
					spriteParadoTorso[ i ].y = 0;
					spriteParadoTorso[ i ].w = 149;
					spriteParadoTorso[ i ].h = 62;
				}
			}*/

			if( !TEXTURA_PERSONAJE_CORRIENDO_PIES.cargarImagen( "imag/sprites/runLegs.png") )
			{
				printf( "Fallo sprite corriendo pies\n" );
				success = false;
			}
			else{
				for (i = 0;i<ANIMACION_CORRIENDO;i++){
					spriteCorriendoPies[ i ].x = i*149;
					spriteCorriendoPies[ i ].y = 0;
					spriteCorriendoPies[ i ].w = 149;
					spriteCorriendoPies[ i ].h = 43;
				}
			}
			/*if( !TEXTURA_PERSONAJE_CORRIENDO_TORSO.cargarImagen( "imag/sprites/runGun.png") )
			{
				printf( "Fallo sprite corriendo torso\n" );
				success = false;
			}
			else{
				for (i = 0;i<ANIMACION_CORRIENDO;i++){
					spriteCorriendoTorso[ i ].x = i*149;
					spriteCorriendoTorso[ i ].y = 0;
					spriteCorriendoTorso[ i ].w = 149;
					spriteCorriendoTorso[ i ].h = 64;
				}
			}*/

			if( !TEXTURA_PERSONAJE_SALTANDO_PIES.cargarImagen( "imag/sprites/jumpLegs.png") )
			{
				printf( "Fallo sprite saltando pies\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_SALTANDO;i++){
					spriteSaltandoPies[ i ].x = i*149;
					spriteSaltandoPies[ i ].y = 0;
					spriteSaltandoPies[ i ].w = 149;
					spriteSaltandoPies[ i ].h = 56;
				}
			}
			/*if( !TEXTURA_PERSONAJE_SALTANDO_TORSO.cargarImagen( "imag/sprites/jumpGun.png") )
			{
				printf( "Fallo sprite saltando torso\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_SALTANDO;i++){
					spriteSaltandoTorso[ i ].x = i*149;
					spriteSaltandoTorso[ i ].y = 0;
					spriteSaltandoTorso[ i ].w = 149;
					spriteSaltandoTorso[ i ].h = 79;
				}
			}

			if( !TEXTURA_PERSONAJE_DISPRANDO_GUN.cargarImagen( "imag/sprites/gunShoot.png") )
			{
				printf( "Fallo sprite disparando gun\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_DISPARANDO_GUN;i++){
					spriteDisparandoGun[ i ].x = i*149;
					spriteDisparandoGun[ i ].y = 0;
					spriteDisparandoGun[ i ].w = 149;
					spriteDisparandoGun[ i ].h = 62;
				}
			}*/

			return success;
		}
		
		void liberarTextura(){
			TEXTURA_PERSONAJE_SALTANDO_PIES.free();
			TEXTURA_PERSONAJE_CORRIENDO_PIES.free();
			TEXTURA_PERSONAJE_PARADO_PIES.free();
		}

		void ponerShotgun(){
			arma.ponerShotgun();
		}

		void ponerGun(){
			arma.ponerGun();
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
		int getvelx(){
			return velx;
		}
		int getvely(){
			return vely;
		}
		int getAncho(){
			return ancho;
		}
		int getAlto(){
			return alto;
		}
		void morir(){
			muerto =true;
			//TEXTURA_PERSONAJE_PARADO_TORSO.setColor(128,128,128);
		}
		void revivir(){
			muerto = false;
			//TEXTURA_PERSONAJE_PARADO_TORSO.setColor(255,255,255);
		}
		int getPosCamara(){
			return posCamara;
		}
};


class Programa
{
	public:

		Programa(){
			renderizador = NULL;
			ventana = NULL;
			gFont = NULL;
		}

		void close(Personaje personaje){
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

		bool iniciar(){
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
						/*if( TTF_Init() == -1 )
						{
							printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
							success = false;
						}*/
					}
				}
			}
			SDL_RendererInfo info;
			SDL_GetRendererInfo(renderizador, &info);
			cout << "Max Height: " << info.max_texture_height << " Max Width: " << info.max_texture_width << endl;
			return success;
		}
};


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	Programa programa;
	bool quit = false;

	if( !programa.iniciar() ){
		printf( "Failed to initialize!\n" );
	}
	else
	{

		//Load mediaojo
		Personaje personaje;
		if( !personaje.cargarImagen() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;
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
				seMovio = personaje.mover(4500); //POR AHORA NO ES NECESARIO MANDARLE EL ANCHO MAXIMO

				//Borro la pantalla
				//DRAWCOLOR ASI PONE TODO ErrorN BLANCO
				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );
				personaje.render(seMovio);
				SDL_RenderPresent( renderizador );



			}
		}
	}
}
