//
// Created by nestor on 15/11/16.
//

#ifndef METALZCURRA_ENEMIGO_H
#define METALZCURRA_ENEMIGO_H

#include <chrono>
#include "Bonus.h"

using namespace chrono;

class Enemigo {

private:
    int id;
    int	posx;
    int posy;
    int ancho;
    int	alto;
    int	muerto;
    int existe;
    int velocidad;
    int cantidadPasos;
    int delta;
	Bonus* bonus;
	Envolvente* envolvente;

    bool disparando;

    time_point<high_resolution_clock> start;
    time_point<high_resolution_clock> actual;
    int alan;
	int restriccionAlan = 2;

    const static int ANIMACION_MUERTE1 = 15;
    const static int ANIMACION_MUERTE2 = 11;
    const static int ANIMACION_MIRANDO = 5;
    const static int ANIMACION_CORRIENDO = 12;
    const static int ANIMACION_DISPARANDO = 3;
    const static int ANIMACION_QUIETO = 20;

    int frameMuerte1 = 0;
    int frameMuerte2 = 0;
    int frameMirando = 0;
    int frameCorriendo = 0;
    int frameDisparando = 0;
    int frameQuieto = 0;

	void droppearBonus();


public:
    Enemigo(int x, int y, int delta);

    bool mover();

    void morir();

    int getAncho();

    int getAlto();

    int getPosx();

    int getPosy();

    int getId();

    void setId(int id);

    void setPosx(int x);

    int estaMuerto();

    int getExiste();

    void crear();

    bool disparar();

    void animacionMuerte1();

    void animacionMuerte2();

    void animacionMirando();

    void animacionCorriendo();

    void animacionDisparando();

    void animacionQuieto();

    void setSprite();

    int getSprite();

    bool isDisparando();

    int getCantidadPasos();

    bool verificarAlan();

	Envolvente* getEnvolvente();

	void setBonus(Bonus* bonus);

	~Enemigo();
};

#endif //METALZCURRA_ENEMIGO_H
