//
// Created by nestor on 30/11/16.
//

#ifndef METALZCURRA_VISTADICOKKA_H
#define METALZCURRA_VISTADICOKKA_H

#define NOGIRO 0
#define IZQUIERDA 1
#define DERECHA 2

#define NODISPARANDO 0
#define DISPARANDO 3
#define TERMINADO 4

#include "Textura.h"
#include "VistaBoss.h"

class VistaDiCokka : public VistaBoss {
private:
    int posx;
    int posy;
    int alto;
    int ancho;

    bool derecha;
    int girando;
    int disparando;

    Textura* TEXTURA_MUERTE;
    Textura* TEXTURA_MOVER;
    Textura* TEXTURA_SALIDA_TIRO;
    Textura* TEXTURA_DISPARAR;
    Textura* TEXTURA_GIRAR;

    const static int ANIMACION_MUERTE = 16;
    SDL_Rect spriteMuerte[ANIMACION_MUERTE];

    const static int ANIMACION_MOVER = 8;
    SDL_Rect spriteMover[ANIMACION_MOVER];

    const static int ANIMACION_SALIDA_TIRO = 9;
    SDL_Rect spriteSalidaTiro[ANIMACION_SALIDA_TIRO];

    const static int ANIMACION_DISPARAR = 7;
    SDL_Rect spriteDisparar[ANIMACION_DISPARAR];

    const static int ANIMACION_GIRAR = 11;
    SDL_Rect spriteGirar[ANIMACION_GIRAR];

    int frame;

public:
    VistaDiCokka(SDL_Renderer *render);

    bool cargarImagen();

    void animacionMover();

    void animacionMuerte();

    void animacionSalidaTiro();

    void animacionDisparar();

    void animacionGirarADerecha();

    void animacionGirarAIzquierda();

    void animacionGirar();

    void render();

    void setFrame(int aux);

    void setDisparando(bool aux);

    void setPosx(int aux);

    void setPosy(int aux);

    void setExiste(bool aux);

    void setDireccion(int posNueva);

    void morir();

    void setGirando(int aux);

    void setDisparando(int aux);

    void transparentar(Uint8 alpha);

    ~VistaDiCokka();
};

#endif //METALZCURRA_VISTADICOKKA_H
