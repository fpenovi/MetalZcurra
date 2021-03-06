

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <deque>
#include <unistd.h>

 
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
				//printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
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

class Bala
{
	private:
		int id;
		int id_personaje;
		int posx;
		int posy;
		bool existe=false;
		int velocidad;
		Textura TEXTURA_BALA;
		int ancho;
		int alto;
		int frame;
		const static int ANIMACION_BALA=12;
		SDL_Rect spriteBala[ ANIMACION_BALA ];
	public:
		Bala(int x, int y, int personaje){
			existe=true;
			posx=x;
			posy=y;
			id_personaje=personaje;
			velocidad=10;
			frame=0;

		}

		bool mover(){
			if (!existe) return false;
			posx+=velocidad;
			if (posx>800 || posx<0) {
				existe=false;
				return false;
			}
			return true;
		}
		int quienDisparo(){
			return id_personaje;
		}
		int getId(){
			return id;
		}
		bool cargarImagen(){
			if( !TEXTURA_BALA.cargarImagen( "imag/sprites/disparoSgun.png") )
			{
				printf( "Fallo imagen bala\n" );
				return false;
			}
			else
			{	
				for (int i = 0;i<ANIMACION_BALA;i++){
					spriteBala[ i ].x = i*299;
					spriteBala[ i ].y = 0;
					spriteBala[ i ].w = 299;
					spriteBala[ i ].h = 137;
				}
			}

			ancho=299;//TEXTURA_BALA.getAncho();
			alto=137;//TEXTURA_BALA.getAlto();
			return true;
		}
		bool render(){
			if (!existe) return false;
			SDL_Rect* currentClip = &spriteBala[frame/3];
			TEXTURA_BALA.render(posx+40,posy+15,currentClip);
			++frame;
			if( frame /3 >= ANIMACION_BALA ){
				frame = 0;
			}
			return true;
			//TEXTURA_BALA.render(posx+40,posy+15);
		}
		void desaparecer(){
			existe=false;
		}
		int getPosx(){
			return posx;
		}
		int getPosy(){
			return posy;
		}
		int getAncho(){
			return ancho;
		}
		int getAlto(){
			return alto;
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
		const static int ANIMACION_APUNTANDO=4;
		const static int ANIMACION_DISPARANDO_GUN_ABAJO=7;
		const static int ANIMACION_DISPARANDO_GUN_ARRIBA=10;
		const static int ANIMACION_DISPARANDO_SHOTGUN_ABAJO=4;
		const static int ANIMACION_DISPARANDO_SHOTGUN_ARRIBA=4;


		int ANIMACION_ACTUAL=10; //empieza siendo 10 por la gun
		int ANIMACION_ACTUAL_DISPARANDO_ARRIBA=10; // empieza siendo 10 por la gun
		int ANIMACION_ACTUAL_DISPARANDO_ABAJO=7; 

		int frameDisparando=0;
		int frameDisparandoArriba=0;
		int frameDisparandoAbajo=0;
		bool disparando=false;

		Textura TEXTURA_ARMA_PARADO;
		SDL_Rect spriteParado[ ANIMACION_PARADO ];

		Textura TEXTURA_ARMA_CORRIENDO;
		SDL_Rect spriteCorriendo[ ANIMACION_CORRIENDO ];

		Textura TEXTURA_ARMA_SALTANDO;
		SDL_Rect spriteSaltando[ ANIMACION_SALTANDO ];

		Textura TEXTURA_ARMA_DISPARANDO;
		SDL_Rect spriteDisparando[ ANIMACION_DISPARANDO_GUN];

		Textura TEXTURA_ARMA_APUNTA_ABAJO;
		SDL_Rect spriteApuntaAbajo[ ANIMACION_APUNTANDO];

		Textura TEXTURA_ARMA_APUNTA_ARRIBA;
		SDL_Rect spriteApuntaArriba[ ANIMACION_APUNTANDO];

		Textura TEXTURA_ARMA_DISPARANDO_ABAJO;
		SDL_Rect spriteDisparandoAbajo[ ANIMACION_DISPARANDO_GUN_ABAJO];

		Textura TEXTURA_ARMA_DISPARANDO_ARRIBA;
		SDL_Rect spriteDisparandoArriba[ ANIMACION_DISPARANDO_GUN_ARRIBA];


		int id;

		deque<Bala*> balas;
		int cantBalas=0;
		bool arriba=false;
		bool abajo=false;


	public:
		Arma(){
			cargarImagen("imag/sprites/gunStill.png",
				"imag/sprites/gunRun.png",
				"imag/sprites/gunJump.png",
				"imag/sprites/gunShoot.png",
				"imag/sprites/gunPointDown.png",
				"imag/sprites/gunPointUp.png",
				"imag/sprites/gunShootDown.png",
				"imag/sprites/gunShootUp.png");
		}
		void asignarID(int id2){

			id=id2;
		}
		void renderParado(int x, int y, int frame, SDL_RendererFlip flip){
			SDL_Rect* currentClip;
			if (arriba) {
				currentClip = &spriteApuntaArriba[frame];
				TEXTURA_ARMA_APUNTA_ARRIBA.render(x,y-5,currentClip,0,NULL,flip );
			}
			else if (abajo) {
				currentClip = &spriteApuntaAbajo[frame];
				TEXTURA_ARMA_APUNTA_ABAJO.render(x,y+10,currentClip,0,NULL,flip );
			}
			else{
				currentClip = &spriteParado[frame];
				TEXTURA_ARMA_PARADO.render(x,y,currentClip,0,NULL,flip );
			}
		}
		void renderCorriendo(int x, int y, int frame, SDL_RendererFlip flip){
			SDL_Rect* currentClip= &spriteCorriendo[frame];
			TEXTURA_ARMA_CORRIENDO.render(x,y,currentClip,0,NULL,flip );
		}
		void renderSaltando(int x, int y, int frame, SDL_RendererFlip flip){
			SDL_Rect* currentClip = &spriteSaltando[frame];
			TEXTURA_ARMA_SALTANDO.render(x,y,currentClip,0,NULL,flip );
		}
		bool renderDisparando(int x, int y, SDL_RendererFlip flip){
			if (arriba) {
				renderDispararArriba(x,y,flip);
			}
			else if (abajo) {
				renderDispararAbajo(x,y,flip);
			}
			else{
				SDL_Rect* currentClip = &spriteDisparando[frameDisparando/4];
				TEXTURA_ARMA_DISPARANDO.render(x,y,currentClip,0,NULL,flip );
				++frameDisparando;
				if( frameDisparando /4 >= ANIMACION_ACTUAL ){
					frameDisparando = 0;
					disparando=false;
				}
			}
			return disparando;
		}
		bool renderDispararArriba(int x, int y,SDL_RendererFlip flip){
			//LE TENGO QUE RESTAR A LA Y, NOSE PORQUE, SUPONGO QUE ALGO DE LA IMAGEN

			SDL_Rect* currentClip = &spriteDisparandoArriba[frameDisparandoArriba/4];
			TEXTURA_ARMA_DISPARANDO_ARRIBA.render(x,y-100,currentClip,0,NULL,flip );
			frameDisparandoArriba++;
			if( frameDisparandoArriba /4 >= ANIMACION_ACTUAL_DISPARANDO_ARRIBA ){
				frameDisparandoArriba = 0;
				disparando=false;
			}
		}
		bool renderDispararAbajo(int x, int y,SDL_RendererFlip flip){
			SDL_Rect* currentClip = &spriteDisparandoAbajo[frameDisparandoAbajo/4];
			TEXTURA_ARMA_DISPARANDO_ABAJO.render(x,y,currentClip,0,NULL,flip );
			frameDisparandoAbajo++;
			if( frameDisparandoAbajo /4 >= ANIMACION_ACTUAL_DISPARANDO_ABAJO ){
				frameDisparandoAbajo = 0;
				disparando=false;
			}
		}
		bool cargarImagen(char* pathParado,char* pathCorriendo,char* pathSaltando,char* pathDisparando,char* pathApuntarAbajo,
			char* pathApuntarArriba, char* pathDisparoAbajo,char* pathDisparoArriba){
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
			if( !TEXTURA_ARMA_APUNTA_ABAJO.cargarImagen( pathApuntarAbajo) )
			{
				printf( "Fallo sprite apunta abajo\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_APUNTANDO;i++){
					spriteApuntaAbajo[ i ].x = i*149;
					spriteApuntaAbajo[ i ].y = 0;
					spriteApuntaAbajo[ i ].w = 149;
					spriteApuntaAbajo[ i ].h = 75;
				}
			}
			if( !TEXTURA_ARMA_APUNTA_ARRIBA.cargarImagen( pathApuntarArriba) )
			{
				printf( "Fallo sprite apunta arriba\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_APUNTANDO;i++){
					spriteApuntaArriba[ i ].x = i*149;
					spriteApuntaArriba[ i ].y = 0;
					spriteApuntaArriba[ i ].w = 149;
					spriteApuntaArriba[ i ].h = 100;
				}
			}
			if( !TEXTURA_ARMA_DISPARANDO_ARRIBA.cargarImagen( pathDisparoArriba) )
			{
				printf( "Fallo sprite disparo arriba\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_ACTUAL_DISPARANDO_ARRIBA;i++){
					spriteDisparandoArriba[ i ].x = i*149;
					spriteDisparandoArriba[ i ].y = 0;
					spriteDisparandoArriba[ i ].w = 149;
					spriteDisparandoArriba[ i ].h = 154;
				}
			}
			if( !TEXTURA_ARMA_DISPARANDO_ABAJO.cargarImagen( pathDisparoAbajo) )
			{
				printf( "Fallo sprite disparo abajo\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_ACTUAL_DISPARANDO_ABAJO;i++){
					spriteDisparandoAbajo[ i ].x = i*149;
					spriteDisparandoAbajo[ i ].y = 0;
					spriteDisparandoAbajo[ i ].w = 149;
					spriteDisparandoAbajo[ i ].h = 135;
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
			liberar();
			ANIMACION_ACTUAL=ANIMACION_DISPARANDO_SHOTGUN;
			ANIMACION_ACTUAL_DISPARANDO_ABAJO=ANIMACION_DISPARANDO_SHOTGUN_ABAJO;
			ANIMACION_ACTUAL_DISPARANDO_ARRIBA=ANIMACION_DISPARANDO_SHOTGUN_ARRIBA;
			cargarImagen("imag/sprites/shotgunStill.png",
				"imag/sprites/shotgunRun.png",
				"imag/sprites/shotgunJump.png",
				"imag/sprites/shotgunShoot.png",
				"imag/sprites/shotgunPointDown.png",
				"imag/sprites/shotgunPointUp.png",
				"imag/sprites/shotgunShootDown.png",
				"imag/sprites/shotgunShootUp.png");
		}
		void ponerGun(){
			liberar();
			ANIMACION_ACTUAL=ANIMACION_DISPARANDO_GUN;
			ANIMACION_ACTUAL_DISPARANDO_ABAJO=ANIMACION_DISPARANDO_GUN_ABAJO;
			ANIMACION_ACTUAL_DISPARANDO_ARRIBA=ANIMACION_DISPARANDO_GUN_ARRIBA;	
			cargarImagen("imag/sprites/gunStill.png",
				"imag/sprites/gunRun.png",
				"imag/sprites/gunJump.png",
				"imag/sprites/gunShoot.png",
				"imag/sprites/gunPointDown.png",
				"imag/sprites/gunPointUp.png",
				"imag/sprites/gunShootDown.png",
				"imag/sprites/gunShootUp.png");
		}
		void disparar(int x, int y){
			if (disparando) return;
			disparando=true;
			Bala *bala = new Bala(x,y,id);
			bala->cargarImagen();
			balas.push_front(bala);
			cantBalas++;
		}
		void renderBalas(){

			bool vive;
			if (cantBalas==0) return;
			for(int i = 0; i < cantBalas; i++){
        		Bala *bala=balas[i];
        		vive=bala->mover();
        		if (vive) {
        			bala->render();
        		}
			}
			if (!vive) {
				balas.pop_back();
				cantBalas--;
			}
		}
		void apuntarAbajo(bool a){
			abajo=a;
		}
		void apuntarArriba(bool a){
			arriba=a;
		}

		deque<Bala*> getBalas(){
			return balas;
		}
		void transparentar(){
			TEXTURA_ARMA_PARADO.setAlpha(120);
		}
};

class Personaje
{

    private:

    	int id;
		int posx, posy;
		int ancho, alto;

		const static int ANIMACION_PARADO=4;
		const static int ANIMACION_CORRIENDO=18;
		const static int ANIMACION_SALTANDO=12;


		Textura TEXTURA_PERSONAJE_PARADO_PIES;
		SDL_Rect spriteParadoPies[ ANIMACION_PARADO ];

		Textura TEXTURA_PERSONAJE_CORRIENDO_PIES;
		SDL_Rect spriteCorriendoPies[ ANIMACION_CORRIENDO ];

		Textura TEXTURA_PERSONAJE_SALTANDO_PIES;
		SDL_Rect spriteSaltandoPies[ ANIMACION_SALTANDO ];


		int frameCorriendo;
		int frameParado;
		int frameSaltando;

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
		int puntaje;
		bool abajo;
		bool arriba;

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
			muerto = false;
			disparar=false;
			puntaje=0;
			abajo=false;
			arriba=false;
		}

		void titilar(){
			TEXTURA_PERSONAJE_PARADO_PIES.setAlpha(90);
		}
		void destitilar(){
			TEXTURA_PERSONAJE_PARADO_PIES.setAlpha(255);
		}

		void asignarID(int id2){
			id=id2;
			arma.asignarID(id);
		}

		int getID(){
			return id;
		}
		//maneja los eventos
		void handleEvent( SDL_Event& e)
		{
		    //If a key was pressed
			if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
		    {
		        //Adjust the velocity
		        switch( e.key.keysym.sym )
		        {
		            case SDLK_LEFT: velx -= Personaje_VEL;derecha = false;break;
		            case SDLK_RIGHT: velx += Personaje_VEL;derecha = true;break;
		            case SDLK_UP: apuntarArriba();break;
		            case SDLK_DOWN: apuntarAbajo();break;
		            case SDLK_x: 
		            	if (!saltando) saltando=true; subiendo=1; break;
		            	if (subiendo==1) vely-=Personaje_VEL_Y;
		            	else if(subiendo == 2) vely+=Personaje_VEL_Y;
		            	else if(subiendo == 0) saltando = false;
		            case SDLK_z:
		            	disparar=true;
		            	arma.disparar(posCamara,posy);
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
		            case SDLK_UP:arriba=false;arma.apuntarArriba(false);break;
		            case SDLK_DOWN:abajo=false;arma.apuntarAbajo(false);break;
		            case SDLK_x: break;
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
			arma.renderBalas();
		}

		void animacionParadoPiernas(){
			
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x=posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=75;
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
				x-=75;
			}
			arma.renderParado(x,posy,frameParado/8,flip);
		}

		void animacionCorrerPiernas(){
			if (saltando) return;
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=75;
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
				x-=75;
			}
			arma.renderCorriendo(x,posy,frameCorriendo/3,flip);
		}

		int animacionSaltandoPiernas(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=75;
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
				x-=75;
			}
			arma.renderSaltando(x,posy,frameSaltando/4,flip);
		}

		void animacionDisparar(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x = posCamara;
			if (!derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=75;
			}

			if (!arma.renderDisparando(x,posy,flip)) disparar=false;
		}

		void apuntarAbajo(){
			arma.apuntarAbajo(true);
			abajo=true;

		}
		void apuntarArriba(){
			arma.apuntarArriba(true);
			arriba=true;
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
		int getAncho(){
			return ancho;
		}
		int getAlto(){
			return alto;
		}
		void morir(){
			muerto =true;
		}
		int getPosx(){
			return posCamara;
		}
		int getPosy(){
			return posy;
		}
		deque<Bala*> getBalas(){
			return arma.getBalas();
		}
		void sumarPuntos(int p){
			puntaje+=p;
			cout << puntaje << endl;
		}
		void transparentar(){
			TEXTURA_PERSONAJE_PARADO_PIES.setAlpha(120);
			arma.transparentar();
		}
};

class Enemigo
{
	private:
		Textura TEXTURA_ENEMIGO_MUERTE1;
		Textura TEXTURA_ENEMIGO_MUERTE2;
		Textura TEXTURA_ENEMIGO_MIRANDO;
		Textura TEXTURA_ENEMIGO_CORRIENDO;
		Textura TEXTURA_ENEMIGO_DISPARANDO;
		Textura TEXTURA_ENEMIGO_QUIETO;

		const static int ANIMACION_MUERTE1=15;
		const static int ANIMACION_MUERTE2=11;
		const static int ANIMACION_MIRANDO=5;
		const static int ANIMACION_CORRIENDO=12;
		const static int ANIMACION_DISPARANDO=3;
		const static int ANIMACION_QUIETO=20;

		SDL_Rect spriteMuerte1[ ANIMACION_MUERTE1 ];
		SDL_Rect spriteMuerte2[ ANIMACION_MUERTE2 ];
		SDL_Rect spriteMirando[ ANIMACION_MIRANDO ];
		SDL_Rect spriteCorriendo[ ANIMACION_CORRIENDO ];
		SDL_Rect spriteDisparando[ ANIMACION_DISPARANDO ];
		SDL_Rect spriteQuieto[ ANIMACION_QUIETO ];

		int frameMuerte1;
		int frameMuerte2;
		int frameMirando;
		int frameCorriendo;
		int frameDisparando;
		int frameQuieto;


		int posInicialx;
		int posy;
		int ancho;
		int	alto;
		int	derecha;
		int	posCamara;
		int	muerto;
		int velx=4;
		int id;

	public:
		Enemigo(int pos,int id2){
			posy = 360;
			posCamara=pos;
			ancho=60;
			alto=80;
			derecha = true;
			muerto = false;
			id=id2;
			frameMuerte1=0;
			frameMuerte2=0;
			frameMirando=0;
			frameCorriendo=0;
			frameDisparando=0;
			frameQuieto=0;
		}
		bool cargarImagen(){
			bool success=true;
			int i;
			//Load sprite sheet texture
			if( !TEXTURA_ENEMIGO_MUERTE1.cargarImagen( "imag/sprites/soldier/death1.png") )
			{
				printf( "Fallo sprite MUERTE1\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_MUERTE1;i++){
					spriteMuerte1[ i ].x = i*149;
					spriteMuerte1[ i ].y = 0;
					spriteMuerte1[ i ].w = 149;
					spriteMuerte1[ i ].h = 96;
				}
			}


			if( !TEXTURA_ENEMIGO_MUERTE2.cargarImagen( "imag/sprites/soldier/death2.png") )
			{
				printf( "Fallo sprite MUERTE2\n" );
				success = false;
			}
			else{
				for (i = 0;i<ANIMACION_MUERTE2;i++){
					spriteMuerte2[ i ].x = i*149;
					spriteMuerte2[ i ].y = 0;
					spriteMuerte2[ i ].w = 149;
					spriteMuerte2[ i ].h = 79;
				}
			}

			if( !TEXTURA_ENEMIGO_MIRANDO.cargarImagen( "imag/sprites/soldier/looking.png") )
			{
				printf( "Fallo sprite LOOKING\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_MIRANDO;i++){
					spriteMirando[ i ].x = i*149;
					spriteMirando[ i ].y = 0;
					spriteMirando[ i ].w = 149;
					spriteMirando[ i ].h = 79;
				}
			}
			if( !TEXTURA_ENEMIGO_CORRIENDO.cargarImagen( "imag/sprites/soldier/run.png") )
			{
				printf( "Fallo sprite SOLDIER RUN\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_CORRIENDO;i++){
					spriteCorriendo[ i ].x = i*149;
					spriteCorriendo[ i ].y = 0;
					spriteCorriendo[ i ].w = 149;
					spriteCorriendo[ i ].h = 86;
				}
			}
			if( !TEXTURA_ENEMIGO_DISPARANDO.cargarImagen( "imag/sprites/soldier/shoot.png") )
			{
				printf( "Fallo sprite SHOOT ENEMIGO\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_DISPARANDO;i++){
					spriteDisparando[ i ].x = i*149;
					spriteDisparando[ i ].y = 0;
					spriteDisparando[ i ].w = 149;
					spriteDisparando[ i ].h = 81;
				}
			}
			if( !TEXTURA_ENEMIGO_QUIETO.cargarImagen( "imag/sprites/soldier/toying.png") )
			{
				printf( "Fallo sprite QUIETO ENEMIGO\n" );
				success = false;
			}
			else{

				for (i = 0;i<ANIMACION_QUIETO;i++){
					spriteQuieto[ i ].x = i*149;
					spriteQuieto[ i ].y = 0;
					spriteQuieto[ i ].w = 149;
					spriteQuieto[ i ].h = 98;
				}
			}
			return success;
		}

		void morir(){
			muerto =true;
			for (int i=0;i<15;i++) animacionMuerte1();
		}
		int getAncho(){
			return ancho;
		}
		int getAlto(){
			return alto;
		}

		void animacionMuerte1(){
			SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
			int x = posCamara;
			SDL_Rect* currentClip = &spriteMuerte1[ frameMuerte1/4 ];
			TEXTURA_ENEMIGO_MUERTE1.render( x, posy+45, currentClip,0,NULL,flip);
			++frameMuerte1;
			if( frameMuerte1 /4 >= ANIMACION_MUERTE1 ){
				frameMuerte1 = 0;
			}
		}
		void animacionMuerte2(){
			SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
			int x = posCamara;
			SDL_Rect* currentClip = &spriteMuerte2[ frameMuerte2/3 ];
			TEXTURA_ENEMIGO_MUERTE2.render( x, posy+45, currentClip,0,NULL,flip);
			++frameMuerte2;
			if( frameMuerte2 /3 >= ANIMACION_MUERTE2 ){
				frameMuerte2= 0;
			}
		}
		void animacionMirando(){
			SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
			int x = posCamara;
			SDL_Rect* currentClip = &spriteMirando[ frameMirando/9 ];
			TEXTURA_ENEMIGO_MIRANDO.render( x, posy+45, currentClip,0,NULL,flip);
			++frameMirando;
			if( frameMirando /9 >= ANIMACION_MIRANDO ){
				frameMirando = 0;
			}
		}
		void animacionCorriendo(){
			SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
			int x = posCamara;
			SDL_Rect* currentClip = &spriteCorriendo[ frameCorriendo/3 ];
			TEXTURA_ENEMIGO_CORRIENDO.render( x, posy+45, currentClip,0,NULL,flip);
			++frameCorriendo;
			if( frameCorriendo /3 >= ANIMACION_CORRIENDO ){
				frameCorriendo = 0;
			}
		}
		void animacionDisparando(){
			SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
			int x = posCamara;
			SDL_Rect* currentClip = &spriteDisparando[ frameDisparando/8 ];
			TEXTURA_ENEMIGO_DISPARANDO.render( x, posy+45, currentClip,0,NULL,flip);
			++frameDisparando;
			if( frameDisparando /8 >= ANIMACION_DISPARANDO ){
				frameDisparando = 0;
			}
		}
		void animacionQuieto(){
			SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
			int x = posCamara;
			SDL_Rect* currentClip = &spriteQuieto[ frameQuieto/5 ];
			TEXTURA_ENEMIGO_QUIETO.render( x, posy+45, currentClip,0,NULL,flip);
			++frameQuieto;
			if( frameQuieto /5 >= ANIMACION_QUIETO ){
				frameQuieto = 0;
			}
		}
		void render(){
			if (muerto) return;
			animacionCorriendo();
		}
		int getPosx(){
			return posCamara;
		}
		int getPosy(){
			return posy;
		}
		void mover(){
			if (!muerto) posCamara-=velx;
		}
		void transparentar(){
			TEXTURA_ENEMIGO_QUIETO.setAlpha(120);
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
		int contadorTotal;
		int contadorRender;
		bool stop;
	public:
		Layer(){
			vuelta=0;
			ancho = 0;
			alto = 0;
			scroll= 0;
			ultimaPos=0;
			velocidad=0;
			contadorRender=0;
			contadorTotal=0;
			stop=false;
		}

		int getAncho(){
			return ancho;
		}
		int getAlto(){
			return alto;
		}

		void render(int x, int y, SDL_Rect* clip){
			//cout << camara.x << endl;
			fondo.render(x,y,clip);
		}

		int scrollear(int posJugadorx){
			if (stop) {
				SDL_Rect clip = {-scroll,0,800,600};
				render(0,0,&clip);
				return scroll;
			}
			contadorTotal++;
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
			//render(scroll,0);
			/*SDL_Rect clip = {-scroll,0,800,600};
			render(0,0,&clip);*/

			//renderizo tambien lo de adelante para que sea infinito
			if (-scroll + SCREEN_WIDTH > ancho) {
				scroll+=movimiento;
				stop=true;
			}
			SDL_Rect clip = {-scroll,0,800,600};
			render(0,0,&clip);
			return scroll;
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
			velocidad = (anchomax-800)/(ancho-800);
			vuelta = int(anchomax) / ancho;
		}

		void transparentar(){
			fondo.setAlpha(120);
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

		int render( int personajex){
			//RECORRO LA LISTA DE LAYERS EN ORDEN DE MAS LEJANO A MAS CERCANO Y RENDERIZO
			int scroll=0;
			for (int i=0; i<contadorCapas;i++){
				Layer *layer = &capas[i];
				scroll=layer->scrollear(personajex);
			}
			return scroll;
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
				layer->asignarVelocidad(double(anchoMax));
			}
		}

		int anchoMaximo(){
			return anchoMax;
		}
		void transparentar(){
			for (int i=0; i<contadorCapas;i++){
				Layer *layer = &capas[i];
				layer->transparentar();
				//scroll=layer->scrollear(personajex);
			}
		}
};

class Texto
{
	private:
		Textura texturaTitulo;
		Textura texturaInput;
		int ancho;
		int alto;
		string titulo;
		string inputText;
	public:

		Texto(string tit){
			titulo = tit;
			texturaTitulo;// = NULL;
			texturaInput;// = NULL;
			inputText="";
		}

		bool cargarTitulo(){
			bool success = true;
			gFont = TTF_OpenFont( "imag/sprites/letras/MetalSlug.ttf", 28 );
			if( gFont == NULL )
			{
				printf( "FALLO CARGANDO EL TIPO DE LETRA! SDL_ttf Error: %s\n", TTF_GetError() );
				success = false;
			}
			else
			{	
				//lo pongo en negro
				//SDL_Color textColor = {255, 0, 0, 0xFF };
				SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF };
				if( !texturaTitulo.loadFromText( titulo, textColor ) )
				{
					printf( "Fallo cargando texto!\n" );
					success = false;
				}
			}
		}

		bool pedir(){

			//Main loop flag
			bool quit = false;

			SDL_Event e;

			//color del texto = rojo
			SDL_Color textColor = { 255, 0, 0, 0xFF };

			texturaInput.loadFromText( inputText.c_str(), textColor );

			//ESTO HACE QUE SE PUEDA ESCRIBIR
			SDL_StartTextInput();

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
			texturaTitulo.render(140,350 );
			texturaInput.render(310,350);

			SDL_StopTextInput();
			return quit;
		}

		string getTexto(){
			return inputText;
		}

		void renderTitulo(int x,int y){

			texturaTitulo.render(x,y);
		}
};

class Puntaje
{
	private:	
		Textura textura_puntaje;
		int puntaje;
		string puntajeString;
		int posx;
		int posy;
	public:
		Puntaje(int x,int y){
			posx=x;
			posy=y;
			puntaje=0;
			puntajeString="0";		
		}

		void sumarPuntos(int x){
			puntaje+=x;

			puntajeString=to_string(puntaje);

			cout << puntaje << endl;
			cout << puntajeString << endl;

		}

		void nuevaPos(int x, int y){
			posx=x;
			posy=y;
		}

		int total(){
			return puntaje;
		}

		void render(){
			SDL_Color textColor = {0, 0, 0, 0xFF };

			if( !textura_puntaje.loadFromText( puntajeString, textColor ) )
			{
				printf( "Fallo cargando puntaje!\n" );
			}
			textura_puntaje.render(posx,posy);
		}
};

class Entrada
{
	private:

		Textura neoGeo;
		Textura esperando;
		Textura fondo;
		Textura TEXTURA_EXPLOSION1;
		Textura TEXTURA_EXPLOSION2;
		Textura TEXTURA_EXPLOSION3;
		Textura TEXTURA_METAL;
		SDL_Rect spriteEntrada1[ 10 ];
		SDL_Rect spriteEntrada2[ 10 ];
		SDL_Rect spriteEntrada3[ 10 ];
		SDL_Rect spriteMetal[ 7 ];

	public:
		Entrada(){
			int i;
			if( !TEXTURA_EXPLOSION1.cargarImagen( "imag/entrada/orden/entrada1.xcf") )
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
			if( !TEXTURA_EXPLOSION2.cargarImagen( "imag/entrada/orden/entrada2.xcf") )
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
			if( !TEXTURA_EXPLOSION3.cargarImagen( "imag/entrada/orden/entrada3.xcf") )
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

			if (!fondo.cargarImagen("imag/entrada/orden/fondito.png")){
				cout << "error cargando fondito"<<endl;
			}
			if (!esperando.cargarImagen("imag/entrada/esperando.xcf")){
				cout << "error cargando esperando"<<endl;
			}
			if (!TEXTURA_METAL.cargarImagen("imag/entrada/orden/MetalSlug.png")){
				cout << "error cargando metal slug"<<endl;
			}
			if( !neoGeo.cargarImagen("imag/entrada/neogeo.png"))
			{
				printf( "Failed to load presentacion!\n" );
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

		~Entrada(){
			neoGeo.free();
			esperando.free();
			fondo.free();
			TEXTURA_EXPLOSION1.free();
			TEXTURA_EXPLOSION2.free();
			TEXTURA_EXPLOSION3.free();
			TEXTURA_METAL.free();
			TTF_CloseFont( gFont );
			//ESTOY CERRANDO Y ABRIENDO AL PEDO AHORA
			//PORQUE USO EL MISMO TIPO DE LETRA
			//TENGO QUE CERRARLO Y ABRIRLO SOLO SI CAMBIO LA LETRA
			//EN EL TP PROBABLEMENTE TENGAN QUE HACERLO EN OTRO LADO
			gFont = TTF_OpenFont( "imag/sprites/letras/MetalSlug.ttf", 28 );
			if ( gFont == NULL )
			{
				printf( "FALLO CARGANDO EL TIPO DE LETRA! SDL_ttf Error: %s\n", TTF_GetError() );
			}
		}

		void pedirTodo(){
			Texto textoip("IP: ");
			textoip.cargarTitulo();
			Texto textopuerto("Puerto: ");
			textopuerto.cargarTitulo();
			Texto textonombre("Nombre: ");
			textonombre.cargarTitulo();

			pedirTexto(&textoip);
			pedirTexto(&textopuerto);
			pedirTexto(&textonombre);
		}

		void pedirTexto(Texto* texto){
			bool quit=false;
			while(!quit)
			{
				SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0 );
				SDL_RenderClear( renderizador );
				neoGeo.render( 100, 50);
				quit=texto->pedir();
				SDL_RenderPresent( renderizador );
			}
		}

		void animaciones(){
			SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( renderizador );
			int frame=0;
			int contador=0;
			while (frame < 10){
				//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );
				fondo.render(0,0);
				SDL_Rect* currentClip = &spriteEntrada1[ frame];
				TEXTURA_EXPLOSION1.render( 0, 0, currentClip);
				SDL_RenderPresent( renderizador );
				if (contador % 5 == 0) frame++;
				contador++;
			}
			frame=0;
			while (frame < 10){
				//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );
				fondo.render(0,0);
				SDL_Rect* currentClip = &spriteEntrada2[ frame ];
				TEXTURA_EXPLOSION2.render( 0, 0, currentClip);
				SDL_RenderPresent( renderizador );
				if (contador % 5 == 0) frame++;
				contador++;
			}
			frame=0;
			while (frame < 10){
				//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );
				fondo.render(0,0);
				SDL_Rect* currentClip = &spriteEntrada3[ frame ];
				TEXTURA_EXPLOSION3.render( 0, 0, currentClip);
				SDL_RenderPresent( renderizador );
				if (contador%5 == 0) frame++;
				contador++;
			}

			frame=0;
			while (frame < 7){
				//SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderizador );
				fondo.render(0,0);
				SDL_Rect* currentClip = &spriteMetal[ frame ];
				TEXTURA_METAL.render( 150, 0, currentClip);
				SDL_RenderPresent( renderizador );
				if (contador % 4 == 0) frame++;
				contador++;
			}
		}

		void animacionEsperando(){
			bool quit = false;
			SDL_Event e;
			Texto esperandoTexto("Esperando jugadores ");
			esperandoTexto.cargarTitulo();
			Texto puntitos(".");
			puntitos.cargarTitulo();

			int vueltas=0;

			//while (!quit){
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
				fondo.render(0,0);
				SDL_Rect* currentClip = &spriteMetal[ 6 ];
				TEXTURA_METAL.render( 115, 0, currentClip);
				esperandoTexto.renderTitulo(120,400);
				if (vueltas % 100 <= 33) puntitos.renderTitulo(360,440);
				else if (vueltas % 100<= 66){
					puntitos.renderTitulo(360,440);
					puntitos.renderTitulo(370,440);
				}
				else {
					puntitos.renderTitulo(360,440);
					puntitos.renderTitulo(370,440);
					puntitos.renderTitulo(380,440);
					}
				SDL_RenderPresent( renderizador );
				vueltas++;
			//}	
		}
};

class PantallaFinal
{
	private:
		Textura textura_perdieron;
		int cantidadJugadores;


	public:
		PantallaFinal(int cant){
			cantidadJugadores=cant;
		}
		void lose(){
			cout << "LOSE" << endl;
			SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF };
			string text = "YOU LOSE MOTHER FUCKERS";
			for (int i=text.length()-1; i>=0;i--){
				if( !textura_perdieron.loadFromText( text.substr(i), textColor ) )
				{
					printf( "Fallo cargando puntaje!\n" );
				}
				SDL_SetRenderDrawColor( renderizador, 0, 0, 0, 0xFF );
				SDL_RenderClear( renderizador );
				textura_perdieron.render(100,280);
				SDL_RenderPresent( renderizador );
				usleep(70000);
			}
			SDL_Event e;
			bool quit=false;
			while (!quit){
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit=true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						if (e.key.keysym.sym == SDLK_RETURN){
							quit=true;
						}
					}
				}
			}
		}

		void pantallaPuntajes(int p1, int p2, int p3, int p4){

			if (p1 > 0){
				Puntaje puntaje1=Puntaje(210,210);
				Textura j1=Textura();
				SDL_Color textColor = {0, 0, 0, 0xFF };
				if( !j1.loadFromText( "Jugador 1", textColor ) )
				{
					printf( "Fallo cargando puntaje1!\n" );
				}
				j1.render(120,170);
				puntaje1.sumarPuntos(p1);
				puntaje1.render();
			} 
			if (p2 > 0){
				Puntaje puntaje2=Puntaje(510,210);
				Textura j2=Textura();
				SDL_Color textColor = {0, 0, 0, 0xFF };
				if( !j2.loadFromText( "Jugador 2", textColor ) )
				{
					printf( "Fallo cargando puntaje2!\n" );
				}
				j2.render(450,170);
				puntaje2.sumarPuntos(p2);
				puntaje2.render();
			} 
			if (p3 > 0){
				Puntaje puntaje3=Puntaje(210,350);
				Textura j3=Textura();
				SDL_Color textColor = {0, 0, 0, 0xFF };
				if( !j3.loadFromText( "Jugador 3", textColor ) )
				{
					printf( "Fallo cargando puntaje3!\n" );
				}
				j3.render(120,300);
				puntaje3.sumarPuntos(p3);
				puntaje3.render();
			} 
			if (p4 > 0){
				Puntaje puntaje4=Puntaje(510,350);
				Textura j4=Textura();
				SDL_Color textColor = {0, 0, 0, 0xFF };
				if( !j4.loadFromText( "Jugador 4", textColor ) )
				{
					printf( "Fallo cargando puntaje4!\n" );
				}
				j4.render(450,300);
				puntaje4.sumarPuntos(p4);
				puntaje4.render();
			} 

		}
};

class Bomba{
	private:
		int posx;
		int posy;
		Textura TEXTURA_BOMBA;
		Textura TEXTURA_EXPLOSION;
		const static int ANIMACION_EXPLOSION=10;
		SDL_Rect spriteExplosion[ ANIMACION_EXPLOSION ];
		int frameExplosion;
		bool explotar;
		bool existe;
		int ancho;
	public:
		Bomba(int x, int y){
			posx=x;
			posy=y;
			explotar=false;
			ancho=54; //TAMANIO DE LAF OTO DE LA BOMBA
			frameExplosion=0;
			existe=true;
			if( !TEXTURA_BOMBA.cargarImagen( "imag/sprites/R-Shobu/disparo.png") )
			{
				printf( "Fallo imagen bomba\n" );
			}

			if( !TEXTURA_EXPLOSION.cargarImagen( "imag/sprites/R-Shobu/explosion.png") )
			{
				printf( "Fallo sprite explosionq\n" );
			}
			else
			{	
				for (int i = 0;i<ANIMACION_EXPLOSION;i++){
					spriteExplosion[ i ].x = i*148;
					spriteExplosion[ i ].y = 0;
					spriteExplosion[ i ].w = 148;
					spriteExplosion[ i ].h = 66;
				}
			}
		}
		bool caer(){
			if (!existe) return false;
			if (explotar) return true;
			posy+=7;
			if (posy>=360) explotar=true;
			return true;
		}
		void render(){
			if (!explotar) TEXTURA_BOMBA.render(posx,posy);
			else{
				SDL_Rect* currentClip = &spriteExplosion[ frameExplosion / 4];
				TEXTURA_EXPLOSION.render(posx,posy,currentClip);
				++frameExplosion;

				if( frameExplosion / 4 >= ANIMACION_EXPLOSION )
				{
					
					existe=false;
				}	
			}
		}
};

class Helicoptero{
	private:
		Textura TEXTURA_HELICOPTERO_DERECHA;
		Textura TEXTURA_HELICOPTERO_IZQUIERDA;
		Textura TEXTURA_HELICE_DERECHA;
		Textura TEXTURA_HELICE_IZQUIERDA;
		const static int ANIMACION_HELICOPTERO_DERECHA=12;
		const static int ANIMACION_HELICOPTERO_IZQUIERDA=7;
		const static int ANIMACION_HELICE=5;
		SDL_Rect spriteHelicopteroDerecha[ ANIMACION_HELICOPTERO_DERECHA ];
		SDL_Rect spriteHelicopteroIzquierda[ ANIMACION_HELICOPTERO_IZQUIERDA ];
		SDL_Rect spriteHeliceDerecha[ ANIMACION_HELICE ];
		SDL_Rect spriteHeliceIzquierda[ ANIMACION_HELICE ];
		//Bomba bomba;
		int posx;
		int posy;
		bool derecha;
		int frameHeliceDerecha;
		int frameHeliceIzquierda;
		int frameHelicopteroDerecha;
		int frameHelicopteroIzquierda;
		int ancho;
		int alto;
		bool disparando=false;
		deque<Bomba*> bombas;
		int cantBombas=0;
	public:

		Helicoptero(int x, int y){
			posx=x;
			posy=y;
			derecha=false;
			alto=140;
			ancho=190;
			bool success = true;
			int i;

			if( !TEXTURA_HELICOPTERO_DERECHA.cargarImagen( "imag/sprites/R-Shobu/moveRight.png") )
			{
				printf( "Fallo sprite gelicoptero derecha\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_HELICOPTERO_DERECHA;i++){
					spriteHelicopteroDerecha[ i ].x = i*299;
					spriteHelicopteroDerecha[ i ].y = 0;
					spriteHelicopteroDerecha[ i ].w = 299;
					spriteHelicopteroDerecha[ i ].h = 128;
				}
			}

			if( !TEXTURA_HELICOPTERO_IZQUIERDA.cargarImagen( "imag/sprites/R-Shobu/moveLeft.png") )
			{
				printf( "Fallo sprite gelicoptero izq\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_HELICOPTERO_IZQUIERDA;i++){
					spriteHelicopteroIzquierda[ i ].x = i*299;
					spriteHelicopteroIzquierda[ i ].y = 0;
					spriteHelicopteroIzquierda[ i ].w = 299;
					spriteHelicopteroIzquierda[ i ].h = 146;
				}
			}

			if( !TEXTURA_HELICE_DERECHA.cargarImagen( "imag/sprites/R-Shobu/aspasRight.png") )
			{
				printf( "Fallo sprite helice derecha\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_HELICE;i++){
					spriteHeliceDerecha[ i ].x = i*299;
					spriteHeliceDerecha[ i ].y = 0;
					spriteHeliceDerecha[ i ].w = 299;
					spriteHeliceDerecha[ i ].h = 19;
				}
			}

			if( !TEXTURA_HELICE_IZQUIERDA.cargarImagen( "imag/sprites/R-Shobu/aspasLeft.png") )
			{
				printf( "Fallo sprite HELICE izq\n" );
				success = false;
			}
			else
			{	
				for (i = 0;i<ANIMACION_HELICE;i++){
					spriteHeliceIzquierda[ i ].x = i*299;
					spriteHeliceIzquierda[ i ].y = 0;
					spriteHeliceIzquierda[ i ].w = 299;
					spriteHeliceIzquierda[ i ].h = 34;
				}
			}
		}
		~Helicoptero(){
			TEXTURA_HELICOPTERO_DERECHA.free();
			TEXTURA_HELICOPTERO_IZQUIERDA.free();
			TEXTURA_HELICE_DERECHA.free();
			TEXTURA_HELICE_IZQUIERDA.free();
		}
		void mover(){
			if (derecha) moverDerecha();
			else moverIzquierda();
		}
		void moverDerecha(){
			posx+=5;
			if (posx+ancho>800) {
				derecha=false;
				frameHelicopteroIzquierda=0;
				frameHelicopteroDerecha=0;
				cout << "reinicio frames" <<endl;
			}
		}
		void moverIzquierda(){
			posx-=5;
			if (posx<0) {
				derecha=true;
				frameHelicopteroDerecha=0;
				frameHelicopteroIzquierda=0;
				cout << "reinicio frames" <<endl;
			}
		}
		void render(){
			if (derecha) animacionDerecha();
			else animacionIzquierda();
			renderBombas();
		}
		void animacionDerecha(){
			SDL_Rect* currentClipHelice = &spriteHeliceDerecha[ frameHeliceDerecha / 4];
			TEXTURA_HELICE_DERECHA.render(posx,posy,currentClipHelice);

			SDL_Rect* currentClipHelicoptero = &spriteHelicopteroDerecha[ frameHelicopteroDerecha / 4];
			TEXTURA_HELICOPTERO_DERECHA.render(posx,posy+10,currentClipHelicoptero);

			++frameHeliceDerecha;

			if( frameHeliceDerecha / 4 >= ANIMACION_HELICE )
			{
				frameHeliceDerecha = 0;
			}	

			++frameHelicopteroDerecha;

			if( frameHelicopteroDerecha / 4 >= ANIMACION_HELICOPTERO_DERECHA )
			{
				frameHelicopteroDerecha--;
			}	
		}
		void animacionIzquierda(){
			SDL_Rect* currentClipHelice = &spriteHeliceIzquierda[ frameHeliceIzquierda / 4];
			TEXTURA_HELICE_IZQUIERDA.render(posx,posy,currentClipHelice);
			SDL_Rect* currentClipHelicoptero = &spriteHelicopteroIzquierda[ frameHelicopteroIzquierda / 4];
			TEXTURA_HELICOPTERO_IZQUIERDA.render(posx,posy+10,currentClipHelicoptero);

			++frameHeliceIzquierda;
			if( frameHeliceIzquierda / 4 >= ANIMACION_HELICE )
			{
				frameHeliceIzquierda = 0;
			}	

			++frameHelicopteroIzquierda;
			if( frameHelicopteroIzquierda / 4 >= ANIMACION_HELICOPTERO_IZQUIERDA )
			{
				frameHelicopteroIzquierda--;
			}	
		}
		void disparar(){
			Bomba *bomba = new Bomba(posx+(ancho/2),posy+alto-30);
			bombas.push_front(bomba);
			cantBombas++;
		}
		void renderBombas(){
			bool vive;
			if (cantBombas==0) return;
			for(int i = 0; i < cantBombas; i++){
        		Bomba *bomba=bombas[i];
        		vive=bomba->caer();
        		if (vive) {
        			bomba->render();
        		}
			}
			if (!vive) {
				bombas.pop_back();
				cantBombas--;
			}
		}
};


#define CERRADO 0
#define CERRANDO 1
#define ABRIENDO 2
#define ABIERTO 3

#define NODISPARANDO 0
#define PREPARANDOSE 1
#define PREPARADO 2
#define DISPARANDO 3
#define TERMINADO 4

class Ovni{

	private:
		int puerta=CERRADO;
		int laser=CERRADO;
		int disparando=NODISPARANDO;

		int posx;
		int posy;
		int alto;
		int ancho;
		Textura TEXTURA_CARGA_TIRO;
		Textura TEXTURA_PUERTA;
		Textura TEXTURA_SACANDO_LASER;
		Textura TEXTURA_DISPARO;
		Textura TEXTURA_MOVIMIENTO;

		const static int ANIMACION_CARGA_TIRO=20;
		SDL_Rect spriteCargandoTiro[ ANIMACION_CARGA_TIRO ];
		int frameCarga;

		const static int ANIMACION_PUERTA=23;
		SDL_Rect spritePuerta[ ANIMACION_PUERTA ];
		int framePuerta;

		const static int ANIMACION_LASER=15;
		SDL_Rect spriteSacandoLaser[ ANIMACION_LASER ];
		int frameLaser;

		const static int ANIMACION_DISPARO=20;
		SDL_Rect spriteDisparo[ ANIMACION_DISPARO ];
		int frameTiro;

		const static int ANIMACION_MOVIMIENTO=24;
		SDL_Rect spriteMovimiento[ ANIMACION_MOVIMIENTO ];
		int frameMovimiento;

		bool derecha;
		int contador=0;
		bool quieto=false;


	public:
		Ovni(int x, int y){
			posx=x;
			posy=y;
			frameCarga=0;
			framePuerta=0;
			frameLaser=0;
			frameTiro=0;
			frameMovimiento=0;
			derecha=false;
			ancho=160;
			alto=117;
			int i;
			bool success=true;

			if( !TEXTURA_CARGA_TIRO.cargarImagen( "imag/sprites/Dai-Manji/chargeShot.png") )
				{
					printf( "Fallo cargar tiro\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_CARGA_TIRO;i++){
						spriteCargandoTiro[ i ].x = i*170;
						spriteCargandoTiro[ i ].y = 0;
						spriteCargandoTiro[ i ].w = 170;
						spriteCargandoTiro[ i ].h = 58;
					}
				}
			if( !TEXTURA_PUERTA.cargarImagen( "imag/sprites/Dai-Manji/openLaserDoor.png") )
				{
					printf( "Fallo cargar puerta\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_PUERTA;i++){
						spritePuerta[ i ].x = i*170;
						spritePuerta[ i ].y = 0;
						spritePuerta[ i ].w = 170;
						spritePuerta[ i ].h = 11;
					}
				}
			if( !TEXTURA_SACANDO_LASER.cargarImagen( "imag/sprites/Dai-Manji/sacandoLaser.png") )
				{
					printf( "Fallo cargar sacandoLaser\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_LASER;i++){
						spriteSacandoLaser[ i ].x = i*170;
						spriteSacandoLaser[ i ].y = 0;
						spriteSacandoLaser[ i ].w = 170;
						spriteSacandoLaser[ i ].h = 34;
					}
				}
			if( !TEXTURA_DISPARO.cargarImagen( "imag/sprites/Dai-Manji/disparoLaser.png") )
				{
					printf( "Fallo cargar disparo\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_DISPARO;i++){
						spriteDisparo[ i ].x = i*170;
						spriteDisparo[ i ].y = 0;
						spriteDisparo[ i ].w = 170;
						spriteDisparo[ i ].h = 175;
					}
				}
			if( !TEXTURA_MOVIMIENTO.cargarImagen( "imag/sprites/Dai-Manji/movimiento.png") )
				{
					printf( "Fallo cargar movimiento ovni\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_MOVIMIENTO;i++){
						spriteMovimiento[ i ].x = i*170;
						spriteMovimiento[ i ].y = 0;
						spriteMovimiento[ i ].w = 170;
						spriteMovimiento[ i ].h = 117;
					}
				}
		}
		void mover(){
			if (quieto) return;
			if (derecha) moverDerecha();
			else moverIzquierda();
		}
		void moverDerecha(){
			posx+=4;
			if (posx+ancho>800) {
				derecha=false;
			}
		}
		void moverIzquierda(){
			posx-=4;
			if (posx<0) {
				derecha=true;
			}
		}
		void animacionMover(){
			SDL_Rect* currentClip = &spriteMovimiento[ frameMovimiento];
			TEXTURA_MOVIMIENTO.render(posx,posy,currentClip);
			++frameMovimiento;

			if( frameMovimiento>= ANIMACION_MOVIMIENTO )
			{
				frameMovimiento = 0;
			}	
		}
		void animacionAbrirPuerta(){
			SDL_Rect* currentClip = &spritePuerta[ framePuerta/3];
			TEXTURA_PUERTA.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
			++framePuerta;

			if( framePuerta/3>= ANIMACION_PUERTA )
			{
				framePuerta--;
				puerta=ABIERTO;
			}
		}
		void animacionCerrarPuerta(){
			SDL_Rect* currentClip = &spritePuerta[ framePuerta/3];
			TEXTURA_PUERTA.render(posx,posy+alto-7,currentClip); //7 hardcodeado para qe qede bien
			--framePuerta;

			if( framePuerta/3 <= 0 )
			{
				framePuerta=0;
				puerta=CERRADO;
			}
		}
		void animacionSacarLaser(){
			SDL_Rect* currentClip = &spriteSacandoLaser[ frameLaser/3];
			TEXTURA_SACANDO_LASER.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
			++frameLaser;

			if( frameLaser/3>= ANIMACION_LASER )
			{
				frameLaser--;
				laser=ABIERTO;
				disparando=PREPARANDOSE;
			}
		}
		void renderLaserFuera(){
			SDL_Rect* currentClip = &spriteSacandoLaser[ frameLaser/3];
			TEXTURA_SACANDO_LASER.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
		}
		void renderLaserDentro(){
			SDL_Rect* currentClip = &spriteSacandoLaser[0];
			TEXTURA_SACANDO_LASER.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
		}
		void animacionGuardarLaser(){
			SDL_Rect* currentClip = &spriteSacandoLaser[ frameLaser/3];
			TEXTURA_SACANDO_LASER.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
			--frameLaser;

			if( frameLaser/3>= ANIMACION_LASER )
			{
				frameLaser=0;
				laser=CERRADO;
			}
		}
		void animacionCargarTiro(){
			SDL_Rect* currentClip = &spriteCargandoTiro[ frameCarga/3];
			TEXTURA_CARGA_TIRO.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
			++frameCarga;

			if( frameCarga/3>= ANIMACION_CARGA_TIRO )
			{
				frameCarga=0;
				disparando=PREPARADO;
				//puerta=ABIERTO;
				//laser=ABIERTO;
			}
		}
		void animacionDisparo(){
			SDL_Rect* currentClip = &spriteDisparo[ frameTiro/3];
			TEXTURA_DISPARO.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
			++frameTiro;

			if( frameTiro/3>= ANIMACION_DISPARO )
			{
				frameTiro=0;
				disparando=NODISPARANDO;
				laser=CERRANDO;
				puerta=CERRANDO;
			}
		}
		void renderPuertaCerrada(){
			SDL_Rect* currentClip = &spritePuerta[0];    //ESTE ES PUERTA CERRADA
			TEXTURA_PUERTA.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
		}
		void renderPuertaAbierta(){
 			SDL_Rect* currentClip = &spritePuerta[ANIMACION_PUERTA-1]; //ESTE ES PUERTA ABIERTA
			TEXTURA_PUERTA.render(posx,posy+alto-7,currentClip);//7 hardcodeado para qe qede bien
		}
		void render(){
			animacionMover();

			if (puerta==ABRIENDO) animacionAbrirPuerta();
			else if (puerta==ABIERTO) renderPuertaAbierta();
			else if (puerta==CERRANDO) animacionCerrarPuerta();
			else if (puerta==CERRADO) renderPuertaCerrada();

			if (laser==ABRIENDO) animacionSacarLaser();
			else if (laser==ABIERTO) renderLaserFuera();
			else if (laser==CERRANDO) animacionGuardarLaser();
			else if (laser==CERRADO) renderLaserDentro();

			if (disparando==PREPARANDOSE) animacionCargarTiro();
			else if((disparando==PREPARADO) || (disparando==DISPARANDO)) animacionDisparo(); 
		}
		void disparar(){
			puerta=ABRIENDO;
			laser=ABRIENDO;
			//disparando=PREPARANDOSE;
		}
};

#define NOGIRO 0
#define IZQUIERDA 1
#define DERECHA 2

class BalaTanque{
	private:
		int posx;
		int posy;
		Textura TEXTURA_BALA;
		Textura TEXTURA_EXPLOSION;
		const static int ANIMACION_EXPLOSION=6;
		SDL_Rect spriteExplosion[ ANIMACION_EXPLOSION ];
		int frameExplosion;
		bool explotar;
		bool existe;
		int ancho;
		int alto;
		bool derecha;
	public:
		BalaTanque(int x, int y, bool der){
			derecha=der;
			posx=x;
			posy=y;
			explotar=false;
			ancho=98; //TAMANIO DE LAF OTO DE LA BOMBA
			alto=47;
			frameExplosion=0;
			existe=true;
			if( !TEXTURA_BALA.cargarImagen( "imag/sprites/Di-Cokka/bala.png") )
			{
				printf( "Fallo imagen bomba\n" );
			}

			if( !TEXTURA_EXPLOSION.cargarImagen( "imag/sprites/Di-Cokka/colision.png") )
			{
				printf( "Fallo sprite colision\n" );
			}
			else
			{	
				for (int i = 0;i<ANIMACION_EXPLOSION;i++){
					spriteExplosion[ i ].x = i*363;
					spriteExplosion[ i ].y = 0;
					spriteExplosion[ i ].w = 363;
					spriteExplosion[ i ].h = 86;
				}
			}
		}
		bool mover(bool der){
			derecha=der;
			if (!existe) return false;
			if (explotar) return true;
			if (derecha) posx+=7;
			else posx-=7;
			if (posx>=800 || posx<0) explotar=true;
			return true;
		}
		void render(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x=posx;
			if (derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=(364-ancho+50);
			}
			if (!explotar) TEXTURA_BALA.render(posx,posy,NULL,0,NULL,flip);
			else{
				SDL_Rect* currentClip = &spriteExplosion[ frameExplosion / 4];
				TEXTURA_EXPLOSION.render(x,posy,currentClip,0,NULL,flip);
				++frameExplosion;

				if( frameExplosion / 4 >= ANIMACION_EXPLOSION )
				{
					existe=false;
				}	
			}
		}
};

class Tanque{
	private:
		int posx;
		int posy;
		int alto;
		int ancho;

		bool derecha;
		int girando;
		int disparando;

		Textura TEXTURA_MUERTE;
		Textura TEXTURA_MOVER;
		Textura TEXTURA_SALIDA_TIRO;
		Textura TEXTURA_DISPARAR;
		Textura TEXTURA_GIRAR;

		const static int ANIMACION_MUERTE=16;
		SDL_Rect spriteMuerte[ ANIMACION_MUERTE ];
		int frameMuerte;

		const static int ANIMACION_MOVER=8;
		SDL_Rect spriteMover[ ANIMACION_MOVER ];
		int frameMover;

		const static int ANIMACION_SALIDA_TIRO=9;
		SDL_Rect spriteSalidaTiro[ ANIMACION_SALIDA_TIRO ];
		int frameSalidaTiro;

		const static int ANIMACION_DISPARAR=7;
		SDL_Rect spriteDisparar[ ANIMACION_DISPARAR ];
		int frameDisparar;

		const static int ANIMACION_GIRAR=11;
		SDL_Rect spriteGirar[ ANIMACION_GIRAR ];
		int frameGirar;

		deque<BalaTanque*> balas;
		int cantBalas=0;

	public:
		Tanque(int x, int y){
			frameMuerte=0;
			frameMover=0;
			frameSalidaTiro=0;
			frameDisparar=0;
			frameGirar=0;
			posx=x;
			posy=y;
			derecha=false;
			ancho=152;
			alto=122;
			girando=NOGIRO;
			disparando=NODISPARANDO;
			cantBalas=0;

			int i;
			bool success=true;

			if( !TEXTURA_MUERTE.cargarImagen( "imag/sprites/Di-Cokka/death.png") )
				{
					printf( "Fallo muerte tanque\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_MUERTE;i++){
						spriteMuerte[ i ].x = i*363;
						spriteMuerte[ i ].y = 0;
						spriteMuerte[ i ].w = 363;
						spriteMuerte[ i ].h = 146;
					}
				}
			if( !TEXTURA_MOVER.cargarImagen( "imag/sprites/Di-Cokka/moving.png") )
				{
					printf( "Fallo mover tanque\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_MOVER;i++){
						spriteMover[ i ].x = i*363;
						spriteMover[ i ].y = 0;
						spriteMover[ i ].w = 363;
						spriteMover[ i ].h = 122;
					}
				}
			if( !TEXTURA_SALIDA_TIRO.cargarImagen( "imag/sprites/Di-Cokka/salidaDelCanon.png") )
				{
					printf( "Fallo salida del tiro tanque\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_SALIDA_TIRO;i++){
						spriteSalidaTiro[ i ].x = i*363;
						spriteSalidaTiro[ i ].y = 0;
						spriteSalidaTiro[ i ].w = 363;
						spriteSalidaTiro[ i ].h = 90;
					}
				}
			if( !TEXTURA_DISPARAR.cargarImagen( "imag/sprites/Di-Cokka/shooting.png") )
				{
					printf( "Fallo disparo tanque\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_DISPARAR;i++){
						spriteDisparar[ i ].x = i*363;
						spriteDisparar[ i ].y = 0;
						spriteDisparar[ i ].w = 363;
						spriteDisparar[ i ].h = 133;
					}
				}
			if( !TEXTURA_GIRAR.cargarImagen( "imag/sprites/Di-Cokka/turning.png") )
				{
					printf( "Fallo girar tanque\n" );
					success = false;
				}
			else
				{	
					for (i = 0;i<ANIMACION_GIRAR;i++){
						spriteGirar[ i ].x = i*363;
						spriteGirar[ i ].y = 0;
						spriteGirar[ i ].w = 363;
						spriteGirar[ i ].h = 122;
					}
				}
		}
		void animacionMover(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x=posx;
			if (derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=(364-ancho);
			}
			SDL_Rect* currentClip = &spriteMover[ frameMover];
			TEXTURA_MOVER.render(x,posy,currentClip,0,NULL,flip);
			++frameMover;

			if( frameMover>= ANIMACION_MOVER )
			{
				frameMover = 0;
			}	
		}
		void animacionMuerte(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x=posx;
			if (derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=(364-ancho);
			}
			SDL_Rect* currentClip = &spriteMuerte[ frameMuerte/5];
			TEXTURA_MUERTE.render(x,posy,currentClip,0,NULL,flip);
			++frameMuerte;

			if( frameMuerte/5>= ANIMACION_MUERTE )
			{
				frameMuerte = 0;
			}	
		}
		void animacionSalidaTiro(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x=posx-50;
			if (derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x= x-364+ancho+100;
			}
			SDL_Rect* currentClip = &spriteSalidaTiro[ frameSalidaTiro];
			TEXTURA_SALIDA_TIRO.render(x,posy-10,currentClip,0,NULL,flip);
			++frameSalidaTiro;

			if( frameSalidaTiro>= ANIMACION_SALIDA_TIRO )
			{
				frameSalidaTiro = 0;
				disparando=NODISPARANDO;
			}	
		}
		void animacionDisparar(){
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			int x=posx;
			if (derecha) {
				flip = SDL_FLIP_HORIZONTAL;
				x-=(364-ancho);
			}
			SDL_Rect* currentClip = &spriteDisparar[ frameDisparar/4];
			TEXTURA_DISPARAR.render(x,posy-10,currentClip,0,NULL,flip);
			++frameDisparar;

			if( frameDisparar/4>= ANIMACION_DISPARAR )
			{
				frameDisparar = 0;
				disparando=TERMINADO;
			}
		}
		void animacionGirarADerecha(){
			SDL_Rect* currentClip = &spriteGirar[ frameGirar/6];
			TEXTURA_GIRAR.render(posx,posy,currentClip);
			++frameGirar;

			if( frameGirar/6>= ANIMACION_GIRAR )
			{
				frameGirar--;
				girando=NOGIRO;
			}
		}
		void animacionGirarAIzquierda(){
			SDL_Rect* currentClip = &spriteGirar[ frameGirar/6];
			TEXTURA_GIRAR.render(posx,posy,currentClip);
			--frameGirar;

			if( frameGirar/6<= 0 )
			{
				frameGirar=0;
				girando=NOGIRO;
			}
		}
		void animacionGirar(){
			if (girando==DERECHA) {
				animacionGirarADerecha();
			}
			if (girando==IZQUIERDA) {
				animacionGirarAIzquierda();
			}
		}
		void mover(){
			if (girando!=NOGIRO || disparando==DISPARANDO) return;
			if (derecha) moverDerecha();
			else moverIzquierda();
		}
		void moverDerecha(){
			posx+=4;
			if (posx+ancho>800) {
				derecha=false;
				cout << "tengo que girar a la izquierda" << endl;
				girando=IZQUIERDA;
			}
		}
		void moverIzquierda(){
			posx-=4;
			if (posx<0) {
				derecha=true;
				cout << "tengo que girar a la derecha" << endl;
				girando=DERECHA;
			}
		}
		void disparar(){
			disparando=DISPARANDO;
			BalaTanque *bala = new BalaTanque(posx,posy,derecha);
			balas.push_front(bala);
			cantBalas++;
		}
		void render(){
			if (girando==NOGIRO) {
				if (disparando==DISPARANDO) animacionDisparar();
				else if (disparando==TERMINADO) {
					animacionSalidaTiro();
					animacionMover();
				}
				else animacionMover();
			}
			else animacionGirar();

			renderBalas();
		}
		void renderBalas(){
			//NO ME ANDA
			bool vive;
			if (cantBalas==0) return;
			for(int i = 0; i < cantBalas; i++){
        		BalaTanque *bala=balas[i];
        		vive=bala->mover(derecha);
        		if (vive) {
        			bala->render();
        		}
			}
			if (!vive) {
				balas.pop_back();
				cantBalas--;
			}
		}

};

class Programa
{
	private:
		int cantPersonajes=0;
		Personaje* personajes[4];
		Puntaje* puntajes[4];
		deque<Enemigo*> enemigos;
		int cantEnemigos;
		int posMapa=0;
		Background* fondo;
		int posBackground;
		PantallaFinal perdieronInutiles = PantallaFinal(4);

	public:

		Programa(){
			renderizador = NULL;
			ventana = NULL;
			gFont = NULL;
			posBackground=0;
			cantEnemigos=0;
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
						if( TTF_Init() == -1 )
						{
							printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
							success = false;
						}
					}
					/*PUNTAJESFONT = TTF_OpenFont( "imag/sprites/letras/puntos.ttf", 28 );
					if ( PUNTAJESFONT == NULL )
					{
						printf( "FALLO CARGANDO EL TIPO DE LETRA! SDL_ttf Error: %s\n", TTF_GetError() );
					}*/
				}
			}

			return success;
		}

		void entradas(){
			Entrada *entrada= new Entrada();
			entrada->pedirTodo();
			entrada->animaciones();
			entrada->animacionEsperando();
			delete(entrada);
		}

		void asignarID(Personaje* p){
			p->asignarID(cantPersonajes);
			personajes[cantPersonajes]=p;
			Puntaje* puntos = new Puntaje(50*(cantPersonajes+1),0);
			puntajes[cantPersonajes]=puntos;
			cantPersonajes++;
		}

		void render(){
			bool seMovio;
			posBackground= fondo->render(personajes[0]->getX());
			for (int i=0;i<cantPersonajes;i++){
				Personaje* p=personajes[i];
				seMovio=p->mover(4500);
				p->render(seMovio);
				Puntaje* p2=puntajes[i];
				p2->render();
			}
			for (int i=0;i<cantEnemigos;i++){
				Enemigo* e=enemigos[i];
				e->mover();
				e->render();
			}
		}

		void prepararEscenario(Background* back){
			fondo=back;
			fondo->prepararEscenario();
		}
		void manejarColisiones(){
			bool muerto=false;
			Enemigo* e;
			for (int p=0;p<cantPersonajes;p++){
				deque<Bala*> balas = personajes[p]->getBalas();
				if (balas.size()==0) return;
				for(int i = 0; i < balas.size(); i++){
					//cout << " for 1" << endl;
	        		Bala *bala=balas[i];
	        		if (cantEnemigos==0) continue;
	        		for (int j=0;j<cantEnemigos;j++){
	        			//cout << " for 2" << endl;
	        			e = enemigos[i];
	        			if (manejarColision(bala,e)){
	        				e->morir();
	        				bala->desaparecer();
	        				muerto=true;
	        				puntajes[p]->sumarPuntos(10);
	        			}
	        			else muerto=false;
	        		}
	        		if (muerto) {
	        			enemigos.pop_back();
	        			cantEnemigos--;
	        		}
				}
			}
		}
		bool manejarColision(Bala *bala,Enemigo* enemigo){
			
			int balax=bala->getPosx();
			int balay=bala->getPosy();
			int balaAlto=bala->getAlto();
			int balaAncho=bala->getAncho();

			int soldadox=enemigo->getPosx();
			int soldadoy=enemigo->getPosy();
			int soldadoAlto=enemigo->getAlto();
			int soldadoAncho=enemigo->getAncho();

			if ((balax+balaAncho < soldadox) || (balax>soldadox+soldadoAncho)) return false;
			if ((balay+balaAlto < soldadoy) || (balay>soldadoy+soldadoAlto)) return false;
			return true;
		}
		void agregarEnemigo(){
			Enemigo *e = new Enemigo(-posBackground+800,360);
			e->cargarImagen();
			enemigos.push_front(e);
			cantEnemigos++;
		}	
		void perdieronTodos(){
			perdieronInutiles.lose();
		}
		void ponerPuntajes(){
			bool seMovio;
			fondo->transparentar();
			for (int i=0;i<cantPersonajes;i++){
				Personaje* p=personajes[i];
				p->transparentar();				
			}
			for (int i=0;i<cantEnemigos;i++){
				Enemigo* e=enemigos[i];
				e->transparentar();
			};
			perdieronInutiles.pantallaPuntajes(300,300,300,300);
		}
		void finNivel(){
			SDL_Event e;
			bool quit=false;
			while (!quit){
				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear(renderizador);
				ponerPuntajes();
				render();
				SDL_RenderPresent( renderizador );
			
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit=true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						if (e.key.keysym.sym == SDLK_RETURN){
							quit=true;
						}
					}
				}
			}
		}
};


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	Programa programa;
	Background fondo;
	bool quit = false;
	int contador=0;

	if( !programa.iniciar() ){
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load mediaojo
		Personaje personaje;
		programa.asignarID(&personaje);
		//Helicoptero *helicoptero = new Helicoptero(800,50);
		//Ovni ovni=Ovni(800,50);
		Tanque tanque=Tanque(800,400);
		//BalaTanque bala=BalaTanque(0,300,true);


		if( !personaje.cargarImagen() || !fondo.agregar("imag/background/1200.png") || !fondo.agregar("imag/background/2400.png") || !fondo.agregar("imag/background/4800.png"))
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			programa.prepararEscenario(&fondo);
			programa.entradas();
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
				/*if (contador % 300 == 0) {
					programa.agregarEnemigo();
				}
				//if (contador %80 == 0) helicoptero->disparar();
				if (contador % 400 == 0) ovni.disparar();*/
				if (contador % 100 == 0) tanque.disparar();

				SDL_SetRenderDrawColor( renderizador, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear(renderizador);

				//helicoptero->mover();
				//ovni.mover();
				tanque.mover();
				//bala.mover();
				//int scroll=fondo.render(personaje.getX());
				//programa.ponerPuntajes();
				programa.render();
				//helicoptero->render();
				//ovni.render();
				tanque.render();
				//bala.render();
				//programa.finNivel();
				programa.manejarColisiones();
				SDL_RenderPresent( renderizador );
				contador++;

				/*if (contador % 500 == 0) {
					programa.perdieronTodos();
				}*/

			}
		}
	}
}
