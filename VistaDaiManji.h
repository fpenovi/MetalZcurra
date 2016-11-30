//
// Created by nestor on 29/11/16.
//

#ifndef METALZCURRA_VISTADAIMANJI_H
#define METALZCURRA_VISTADAIMANJI_H

#define CERRADO 0
#define CERRANDO 1
#define ABRIENDO 2
#define ABIERTO 3

#define NODISPARANDO 0
#define PREPARANDOSE 1
#define PREPARADO 2
#define DISPARANDO 3
#define TERMINADO 4

#include <SDL2/SDL_rect.h>
#include "Textura.h"
#include "VistaBoss.h"

class VistaDaiManji : public VistaBoss
{
private:
    int puerta = ABIERTO;
    int laser = ABIERTO;
    int disparando = NODISPARANDO;

    int posx;
    int posy;
    int alto;
    int ancho;
    Textura* TEXTURA_CARGA_TIRO;
    Textura* TEXTURA_PUERTA;
    Textura* TEXTURA_SACANDO_LASER;
    Textura* TEXTURA_DISPARO;
    Textura* TEXTURA_MOVIMIENTO;

    const static int ANIMACION_CARGA_TIRO = 20;
    SDL_Rect spriteCargandoTiro[ ANIMACION_CARGA_TIRO ];

    const static int ANIMACION_PUERTA = 23;
    SDL_Rect spritePuerta[ ANIMACION_PUERTA ];

    const static int ANIMACION_LASER = 15;
    SDL_Rect spriteSacandoLaser[ ANIMACION_LASER ];

    const static int ANIMACION_DISPARO = 20;
    SDL_Rect spriteDisparo[ ANIMACION_DISPARO ];

    const static int ANIMACION_MOVIMIENTO = 24;
    SDL_Rect spriteMovimiento[ ANIMACION_MOVIMIENTO ];

    bool derecha;
    int contador = 0;
    bool quieto = false;
    int frame = 0;

public:
    VistaDaiManji(SDL_Renderer* render);

    bool cargarImagen();

    void render();

    void setDisparando(bool aux);

    void setPosx(int aux);

    void setPosy(int aux);

    void setFrame(int aux);

    void setExiste(bool aux);

    void setDireccion(int posNueva);

    void morir();

    void transparentar(Uint8 alpha);

    ~VistaDaiManji();

    void renderPuertaAbierta();

    void renderPuertaCerrada();

    void animacionDisparo();

    void animacionCargarTiro();

    void animacionGuardarLaser();

    void renderLaserDentro();

    void renderLaserFuera();

    void animacionSacarLaser();

    void animacionCerrarPuerta();

    void animacionAbrirPuerta();

    void animacionMover();

    void setPuerta(int aux);

    void setLaser(int aux);

    void setDisparando(int aux);
};

#endif //METALZCURRA_VISTADAIMANJI_H
