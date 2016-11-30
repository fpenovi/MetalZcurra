//
// Created by nestor on 26/11/16.
//

#ifndef METALZCURRA_VISTABOSS_H
#define METALZCURRA_VISTABOSS_H

#include <SDL2/SDL_stdinc.h>

class VistaBoss
{
protected:
    int posx;
    int posy;
    bool existe;
    bool derecha;
	bool muerto = false;

public:
    virtual bool cargarImagen() = 0;

    virtual void render() = 0;

    virtual void setFrame(int aux) = 0;

    virtual void setPosx(int aux) = 0;

    virtual void setPosy(int aux) = 0;

	bool estaVivo();

	void setMuerto(bool muerto);

	virtual void transparentar(Uint8 alpha) = 0;

    virtual void setExiste(bool aux) = 0;

    virtual void setDireccion(int posNueva) = 0;

    virtual void morir() = 0;

};

#endif //METALZCURRA_VISTABOSS_H
