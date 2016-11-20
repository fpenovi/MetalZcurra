//
// Created by nestor on 16/11/16.
//

#ifndef METALZCURRA_VISTAENEMIGO_H
#define METALZCURRA_VISTAENEMIGO_H

#include <vector>
#include "Textura.h"

class VistaEnemigo
{
private:
    int posx;
    int posy;
    int velocidad;
    bool existe;
    int id;
    int ancho;
    int alto;
    int	muerto;
    bool disparando;
    int cantPasos;

    int frame;
    SDL_RendererFlip flip;
    SDL_Rect* currentClip;

    Textura* TEXTURA_ENEMIGO_MUERTE1;
    Textura* TEXTURA_ENEMIGO_MUERTE2;
    Textura* TEXTURA_ENEMIGO_MIRANDO;
    Textura* TEXTURA_ENEMIGO_CORRIENDO;
    Textura* TEXTURA_ENEMIGO_DISPARANDO;
    Textura* TEXTURA_ENEMIGO_QUIETO;

    const static int ANIMACION_MUERTE1 = 15;
    const static int ANIMACION_MUERTE2 = 11;
    const static int ANIMACION_MIRANDO = 5;
    const static int ANIMACION_CORRIENDO = 12;
    const static int ANIMACION_DISPARANDO = 3;
    const static int ANIMACION_QUIETO = 20;

    SDL_Rect spriteMuerte1[ ANIMACION_MUERTE1 ];
    SDL_Rect spriteMuerte2[ ANIMACION_MUERTE2 ];
    SDL_Rect spriteMirando[ ANIMACION_MIRANDO ];
    SDL_Rect spriteCorriendo[ ANIMACION_CORRIENDO ];
    SDL_Rect spriteDisparando[ ANIMACION_DISPARANDO ];
    SDL_Rect spriteQuieto[ ANIMACION_QUIETO ];

public:
    VistaEnemigo(vector<Textura*> TEXTURAS_ENEMIGOS);

    ~VistaEnemigo();

    void render();

    bool cargarImagen();

    bool existeEnemigo();

    void setID(int nuevoID);

    int getPosx();

    int getPosy();

    int getAncho();

    int getAlto();

    void morir();

    int getId();

    void setPosX(int x);

    void setPosY(int y);

    void setExiste(bool exist);

    void animacionMuerte1();

    void animacionMuerte2();

    void animacionMirando();

    void animacionCorriendo();

    void animacionDisparando();

    void animacionQuieto();

    void setFrame(int aux);

    void setDisparando(int aux);

    void setCantPasos(int aux);

    void revivir();
};

#endif //METALZCURRA_VISTAENEMIGO_H
