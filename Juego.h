//
// Created by franch on 28/11/16.
//

#ifndef METALZCURRA_JUEGO_H
#define METALZCURRA_JUEGO_H

#include <SDL2/SDL_render.h>
#include "Cliente.h"
#include "VistaPersonaje.h"
#include "VistaBala.h"
#include "VistaEnemigo.h"
#include "VistaBonus.h"
#include "VistaBoss.h"
#include "Background.h"
#include "Texto.h"
#include "VistaPuntajes.h"
#include "FinalScreen.h"


class VistaPuntajes;	// Para que pueda compilar al estar una clase incluida en la otra
class GrayOutHandler;

class Juego {

private:
	SDL_Renderer *renderizador;
	SDL_Window *ventana;
	SDL_Rect *camera;
	Cliente *cliente;
	unordered_map<int, VistaPersonaje *> vistasPersonajes;
	unordered_map<int, VistaBala *> vistasBalas;
	unordered_map<int, VistaEnemigo *> vistasEnemigos;
	unordered_map<int, VistaBonus *> visitasBonuses;
	unordered_map<int, VistaBoss *> vistasBoss;
	VistaBoss *bossActual;
	int lastKeyPressed;
	Background *fondo;
	int posFondo;
	int screenWidth = 800;
	int screenHeight = 600;
	int cantidadUsuarios;
	int modoJuego;
	int idBonus;
	bool enterHabilitado = false;
	VistaPuntajes* puntajes;
	FinalScreen* pantallaFinal = NULL;
	unordered_map<int, VistaBala *> vistasBalasVivas;
	unordered_map<int, VistaEnemigo *> vistasEnemigosVivos;
	int nivelActual;

	// TEXUTRAS
	Textura *TEXTURA_BALA;
	Textura *TEXTURA_BALA_ENEMIGA;
	Textura *TEXTURA_BALA_HMGUN;
	Textura *TEXTURA_BALA_SHOTGUN;
	Textura *TEXTURA_BALA_RLAUNCHER;
	vector<Textura *> TEXTURAS_ENEMIGOS;
	Textura *TEXTURA_HMGUN;
	Textura *TEXTURA_SGUN;
	Textura *TEXTURA_RLAUNCHER;
	Textura *TEXTURA_KILLALL;
	Textura *TEXTURA_RECOVER;
	Textura *TEXTURA_BALA_RSHOBU;

	// PROTOCOLO
	int tipoObjeto, id, state, posX, posy, posCam, conectado, spriteIdx, aim, saltando, puntaje;
	int miId;

	// Atributos para sala de espera
	Texto *textoip;
	Texto *textopuerto;
	Texto *textonombre;
	Texto *esperandoTexto;
	Texto *puntitos;
	Textura *neoGeo;
	Textura *fondoInicial;
	Textura *TEXTURA_EXPLOSION1;
	Textura *TEXTURA_EXPLOSION2;
	Textura *TEXTURA_EXPLOSION3;
	Textura *TEXTURA_METAL;
	SDL_Rect spriteEntrada1[10];
	SDL_Rect spriteEntrada2[10];
	SDL_Rect spriteEntrada3[10];
	SDL_Rect spriteMetal[7];
	string ip;
	string puerto;
	string nombre;

	GrayOutHandler* grayOutHandler = NULL;
	bool isRunning = false;

public:
	static Uint8 LIMITE_SUPERIOR_TRANSPARENCIA;
	static int LIMITE_SUPERIOR_TRANSPARENCIA_NEGATIVA;
	static Uint8 transparenciaActual;
	static Uint8 LIMITE_INFERIOR_TRANSPARENCIA;

	Juego();

	void close();

	void presentacion();

	void liberarPresentacion();

	void cargarEntrada();

	void entrada();

	void liberarEntrada();

	bool iniciar();

	void transparentar(Uint8 alpha);

	SDL_Renderer *getRenderer();

	SDL_Window *getVentana();

	SDL_Rect *getCamera();

	Cliente *getCliente();

	Background *getBackground();

	int getPosX();

	VistaPersonaje *getPersonajeById(int id);

	VistaBala *getBalaById(int id);

	VistaEnemigo *getEnemigoById(int id);

	VistaBonus *getBonusById(int id);

	VistaBoss *getBossById(int id);

	string getIp();

	string getPuerto();

	string getNombre();

	void setPosX(int x);

	void setBackground(Background *fondo);

	void setCliente(Cliente *client);

	void setCamara(SDL_Rect *camara);

	void addPersonaje(int id, VistaPersonaje *pj);

	void addBala(int id, VistaBala *bala);

	void addEnemigo(int id, VistaEnemigo *enemigo);

	void addBonus(int id, VistaBonus *bonus);

	void addBoss(int id, VistaBoss *boss);

	void addBalaViva(int id, VistaBala *bala);

	void addEnemigoVivo(int id, VistaEnemigo *enemigo);

	void removeBala(int id);

	void removeEnemigo(int id);

	bool existeBala(int id);

	bool existeEnemigo(int id);

	void conectar();

	void handleEvent(SDL_Event &e);

	void moverCamara(int id);

	void renderizar();

	int getPersonajeMasMovido();

	void jugadoresInicio();

	void salaDeEspera();

	void recibirEscenario();

	void recibirCapas();

	void recibirPersonajes();

	void recibirNuevoBackground();

	void recibirNuevasCapas();

	void crearBalas();

	void crearEnemigos();

	void crearBonuses();

	void crearBoss();

	void seleccionarBossSiguiente();

	void cargarTexturaBala();

	void cargarTexturaEnemigo();

	void cargarTexturaBonus();

	void parsearUpdateVista(string update);

	int getTipoObjeto();

	void actualizarPersonaje();

	void actualizarBala();

	void actualizarSpriteDisparo();

	void actualizarEnemigo();

	void actualizarBonus();

	void nuevoBonus();

	void actualizarBoss();

	void crearVistaPuntajes();

	void actualizarImpacto();

	void actualizarQuietos();

	void recibirUsuarios();

	string getNombreUsuarioById(int id);

	bool puedePasarDeNivel();

	void setPuedePasarDeNivel(bool aux);

	void actualizarPuntaje();

	void setTransparenciaActual(Uint8 aux);

	Uint8* getTransparenciaActual();

	bool haFinalizadoJuego();
};

#endif //METALZCURRA_JUEGO_H
