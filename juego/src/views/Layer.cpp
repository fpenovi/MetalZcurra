/*
 * Layer.cpp
 *
 *  Created on: Oct 25, 2016
 *      Author: franch
 */

#include "Layer.h"
#include <iostream>

Layer::Layer(SDL_Renderer* renderizador){
    vuelta = 0;
    ancho = 0;
    alto = 0;
    scroll = 0;
    ultimaPos = 0;
    velocidad = 0;
    fondo = new Textura(renderizador);
}

int Layer::getAncho(){
    return ancho;
}

int Layer::getAlto(){
    return alto;
}

void Layer::render(int x, int y ){
    //cout << camara.x << endl;
    fondo->render(x,y);
}

void Layer::scrollear(int posJugadorx){
    //calculo lo que tiene que scrollear el fondo segun su velocidad
    int aux = posJugadorx/velocidad;
    //posJugadorx = posJugadorx/velocidad;

    //para qeu no se vaya al chori
    if (scroll > aux) scroll = -aux;

    //cuanto tiene que moverse
    int movimiento = aux - ultimaPos;

    //guardo ultima pos nueva
    ultimaPos = aux;

    //scrolleo
    scroll -= movimiento;

    //si me termine el layer empiezo denuevo
    if (-scroll > ancho) scroll = 0 ;

    //renderizo la pos del layer
    render(scroll,0);

    //renderizo tambien lo de adelante para que sea infinito
    if (-scroll + 800 > ancho) {  //PONGO 800 PORQUE ES EL SCREEN-WIDTH
        //cout << "entre al render"<<endl;
        render(scroll+ancho,0);
    }
}

bool Layer::cargarImagen(char* path){
    if (!fondo->cargarImagen(path)){
        cout << "error cargando imagen, en agregar Background" << endl;
        if (!fondo->cargarImagen("imag/background/cruz.png"))
            return false;
    }

    ancho = fondo->getAncho();
    alto = fondo->getAlto();
    return true;
}

void Layer::asignarVelocidad(double anchomax){

    velocidad = anchomax/ancho;
    //vuelta = int(anchomax) / ancho;
    //velocidad = velocidad / vuelta;
    cout << velocidad << endl;
    cout << vuelta << endl;
}

Layer::~Layer() {
    delete fondo;
}
