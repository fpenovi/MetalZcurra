

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
		bool saltando ;
		int subiendo;
		int frameSaltando;
		bool salto=false;
		int posCamara;

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
			posy = 360;
			ancho=60;
			alto=80;
			frameCorriendo=0;
			frameParado=0;
			frameSaltando=0;
			velx = 0;
			vely = 0;
			derecha = true;
			saltando = false;
			subiendo = 0;
			frameSaltando=0;
			posCamara=0;
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
		            	if (!saltando) saltando=true; subiendo=1; /*vely -= Personaje_VEL_Y;*/ break;
		            	if (subiendo==1) vely-=Personaje_VEL_Y;
		            	else if(subiendo == 2) vely+=Personaje_VEL_Y;
		            	else if(subiendo == 0) saltando = false;
		        }
		    }

		    //If a key was released
		    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
		    {
		        //Adjust the velocity
		        switch( e.key.keysym.sym )
		        {
		            case SDLK_LEFT: /*velx += Personaje_VEL*/velx=0; break;
		            case SDLK_RIGHT: /*velx -= Personaje_VEL*/velx=0; break;
		            case SDLK_UP: /*vely -= Personaje_VEL_Y; */break;
		        }
		    }
		}

		//Moves the Personaje
		//UTILIZO LA POSCAMARA PARA RENDERIZAR EN LA POSICION CORRECTA
		bool mover(int anchoMax)
		{
			if (posx >= anchoMax) posx=0;
			int pos1 = posx;
			int pos2 = posy;
		    //moverlo a derecha o izquierda
		    posx += velx;
		    posCamara+=velx;
		    if( ( posCamara < 0 )  || ( posCamara + ancho > SCREEN_WIDTH/2 ) )
		    {
		        posCamara -= velx;
		    }

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
			if (velx < 0 || posCamara+ancho != SCREEN_WIDTH/2){
		    	posx -= velx;
		    }

			return true;

		}

		void render(bool seMovio){
			if (seMovio==true){
					animacionCorrer();
				} 
				else {
					animacionParado();
				}
				if (estaSaltando()) animacionSaltando();
		}

		void animacionParado(){
			
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) flip = SDL_FLIP_HORIZONTAL;
			SDL_Rect* currentClip = &spriteParado[ frameParado / 9];
			TEXTURA_PERSONAJE_PARADO.render(posCamara,posy, currentClip,0,NULL,flip );

			++frameParado;

			if( frameParado / 9 >= ANIMACION_PARADO )
			{
				frameParado = 0;
			}
			
		}
		void animacionCorrer(){
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
			}
			SDL_Rect* currentClip = &spriteCorriendo[ frameCorriendo /3 ];
			TEXTURA_PERSONAJE_CORRIENDO.render( posCamara, posy, currentClip,0,NULL,flip);
			++frameCorriendo;
			if( frameCorriendo /3 >= ANIMACION_CORRIENDO ){
				frameCorriendo = 0;
			}
		}

		int animacionSaltando(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
			}
			SDL_Rect* currentClip = &spriteSaltando[ frameSaltando/4 ];
			TEXTURA_PERSONAJE_SALTANDO.render( posCamara, posy, currentClip,0,NULL,flip);
			
			if (frameSaltando/4  >= ANIMACION_SALTANDO/2){
				subiendo=2;
				//bajando= true;
				posy+=Personaje_VEL_Y;
			}
			else {
				subiendo=1;
				//bajando=false;
				posy-=Personaje_VEL_Y;
			}
			++frameSaltando;
			if( frameSaltando/4 == ANIMACION_SALTANDO ){
				frameSaltando = 0;
				saltando=false;
				subiendo=0;
				//bajando= false;
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
};

class Layer
{
	private:
		Textura fondo;
		double scroll;
		int ancho;
		int alto;
		int vuelta;
		int ultimaPos;
		double velocidad;
	public:
		Layer(){
			vuelta=0;
			ancho = 0;
			alto = 0;
			scroll= 0;
			ultimaPos=0;
			velocidad=0;
		}

		int getAncho(){
			return ancho;
		}
		int getAlto(){
			return alto;
		}

		void render(int x, int y ){
			//cout << camara.x << endl;
			fondo.render(x,y);
		}

		void scrollear(int posJugadorx){
			//calculo lo que tiene que scrollear el fondo segun su velocidad
			posJugadorx = posJugadorx/velocidad;

			//para qeu no se vaya al chori
			if (scroll > posJugadorx) scroll = -posJugadorx;

			//cuanto tiene que moverse
			int movimiento = posJugadorx - ultimaPos;

			//guardo ultima pos nueva
			ultimaPos = posJugadorx;

			//scrolleo
			scroll -= movimiento;

			//si me termine el layer empiezo denuevo
			if (-scroll > ancho) scroll = 0 ;
			//renderizo la pos del layer
			render(scroll,0);

			//renderizo tambien lo de adelante para que sea infinito
			render(scroll+ancho,0);


				
		}

		bool cargarImagen(char* path){
			if (!fondo.cargarImagen(path)){
				cout << "error cargando imagen, en agregar Background"<<endl;
				return false;
			}

			ancho = fondo.getAncho();
			alto = fondo.getAlto();
			return true;
		}

		void asignarVelocidad(double anchomax){

			velocidad = anchomax/ancho;
			vuelta = int(anchomax) / ancho;
			velocidad = velocidad / vuelta;
			cout << velocidad << endl;
			cout << vuelta << endl;
		}

};


class Background
{
	private:
		int cantCapas;
		int contadorCapas=0;
		Layer capas[5];//HARDODEADO
		int anchoMax = 0;

	public:
		Background(int cantidadCapas=1){
			//LA IDEA ES QUE ME MANDE UNA CANTIDAD Y AHI INICIALICE EL ARRAY DE TEXTURAS CON ESE TAMANIO
			//cantCapas=cantidadCapas;
			//capas[cantCapas];
		}

		void render( int personajex){
			//RECORRO LA LISTA DE LAYERS EN ORDEN DE MAS LEJANO A MAS CERCANO Y RENDERIZO
			for (int i=0; i<contadorCapas;i++){
				Layer *layer = &capas[i];
				layer->scrollear(personajex);
			}
		}

		bool agregar(char* path){
			//AGREGO UNA CAPA SOLA Y GUARDO SI EL ANCHO ES EL MAYOR DE TODOS
			if (!capas[contadorCapas].cargarImagen(path)) {
				cout << "error cargando imagen, en agregar Background"<<endl;
				return false;
			}
			int ancho = capas[contadorCapas].getAncho();
			if (ancho>anchoMax) anchoMax=ancho;
			contadorCapas++;
			return true;
		}

		void prepararEscenario(){
			//ASIGNO LAS VELOCIDADES DE CADA LAYER
			for (int i=0; i<contadorCapas ;i++){
				Layer *layer = &capas[i];
				//layer->asignarVelocidad(double(anchoMax)/layer->getAncho());
				layer->asignarVelocidad(double(anchoMax));
			}
		}

		int anchoMaximo(){
			return anchoMax;
		}
};

class Programa
{
	private:
		Textura neoGeo;
		Textura esperando;


	public:

		Programa(){
			renderizador = NULL;
			ventana = NULL;
			gFont = NULL;

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

		void presentacion(){
			Uint32 start = 0;

			SDL_Event e;
			bool quit = false;

			if( !neoGeo.cargarImagen("imag/entrada/neogeo.png") || !esperando.cargarImagen("imag/entrada/esperando.xcf"))
			{
				printf( "Failed to load presentacion!\n" );
			}
			else{
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
					neoGeo.render( 100, 50);
					esperando.render(180,400);

					SDL_RenderPresent( renderizador );
				}
			}
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
						if( TTF_Init() == -1 )
						{
							printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
							success = false;
						}
					}
				}
			}

			return success;
		}
};

class Texto
{
	private:
		Textura texturaTitulo;
		Textura texturaInput;
		//TTF_Font* gFont;
		int ancho;
		int alto;
		string titulo;
	public:

		Texto(string tit){
			titulo = tit;
			texturaTitulo;// = NULL;
			texturaInput;// = NULL;
		}

		bool cargarTitulo(){
			bool success = true;
			gFont = TTF_OpenFont( "imag/neogray.ttf", 28 );
			if( gFont == NULL )
			{
				printf( "FALLO CARGANDO EL TIPO DE LETRA! SDL_ttf Error: %s\n", TTF_GetError() );
				success = false;
			}
			else
			{	
				//lo pongo en negro
				SDL_Color textColor = {0, 0, 0, 0xFF };
				if( !texturaTitulo.loadFromText( titulo, textColor ) )
				{
					printf( "Fallo cargando texto!\n" );
					success = false;
				}
			}
		}

		void pedir(){

			//Main loop flag
			bool quit = false;

			SDL_Event e;

			//color del texto = negro
			SDL_Color textColor = { 0, 0, 0, 0xFF };

			//The current input text.
			string inputText = "";

			texturaInput.loadFromText( inputText.c_str(), textColor );

			//ESTO HACE QUE SE PUEDA ESCRIBIR
			SDL_StartTextInput();

			//While application is running
			while( !quit )
			{
				//The rerender text flag
				bool renderText = false;

				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					else if( e.type == SDL_KEYDOWN )
					{
						//EL BORRAR
						if (e.key.keysym.sym == SDLK_RETURN){
							quit = true;
							continue;
						}
						if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
						{
							//lop off character
							inputText.pop_back();
							renderText = true;
						}
						//manejo copy
						else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
						{
							SDL_SetClipboardText( inputText.c_str() );
						}
						//manejo paste
						else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
						{
							inputText = SDL_GetClipboardText();
							renderText = true;
						}
					}
					//Special text input event
					else if( e.type == SDL_TEXTINPUT )
					{
						//Not copy or pasting
						if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
						{
							//Append character
							inputText += e.text.text;
							renderText = true;
						}
					}
				}

				//SI HUBO ALGUNA LETRA ESCRITA, RENDEREO
				if( renderText )
				{
					if( inputText != "" )
					{
						//Render new text
						texturaInput.loadFromText( inputText.c_str(), textColor );
					}
					//Text is empty
					else
					{
						//Render space texture
						texturaInput.loadFromText( " ", textColor );
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );

				//Render text textures
				texturaTitulo.render(5,0 );
				texturaInput.render(150,0);

				//Update screen
				SDL_RenderPresent( renderizador );
			}
			SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( renderizador );
			SDL_RenderPresent( renderizador );
			//Disable text input
			SDL_StopTextInput();
		}
};

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	Programa programa;
	Personaje personaje;
	Background fondo;
	Texto textoip("IP: ");
	Texto textopuerto("Puerto: ");

	if( !programa.iniciar() ){
		printf( "Failed to initialize!\n" );
	}
	else
	{
		
		/*textoip.cargarTitulo();
		textoip.pedir();
		
		textopuerto.cargarTitulo();
		textopuerto.pedir();

		//programa.pedirIp();
		programa.presentacion();*/

		//Load media
		if( !personaje.cargarImagen() || !fondo.agregar("imag/background/gris.png") || !fondo.agregar("imag/background/rojo.png"))
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			fondo.prepararEscenario();
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
				seMovio = personaje.mover(fondo.anchoMaximo()); //POR AHORA NO ES NECESARIO MANDARLE EL ANCHO MAXIMO

				//Borro la pantalla
				//DRAWCOLOR ASI PONE TODO ErrorN BLANCO
				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );

				fondo.render(personaje.getX());
				personaje.render(seMovio);
				SDL_RenderPresent( renderizador );
			}
		}
	}
}
