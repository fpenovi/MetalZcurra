//
// Created by duha on 17/10/16.
//

#include "Background.h"

Background::Background(SDL_Renderer *renderizador){
    //LA IDEA ES QUE ME MANDE UNA CANTIDAD Y AHI INICIALICE EL ARRAY DE TEXTURAS CON ESE TAMANIO
    //cantCapas=cantidadCapas;
    //capas[cantCapas];
    this->renderizador = renderizador;

    for (int i = 0 ; i < 2; i++){
        Layer* layer = new Layer(renderizador);
        capas.push_back(layer);
    }
}

void Background::render( int personajex){
    //RECORRO LA LISTA DE LAYERS EN ORDEN DE MAS LEJANO A MAS CERCANO Y RENDERIZO
    for (int i=0; i<contadorCapas;i++){
        Layer *layer = capas[i];
        layer->scrollear(personajex);
    }
}

bool Background::agregar(char* path){
    //AGREGO UNA CAPA SOLA Y GUARDO SI EL ANCHO ES EL MAYOR DE TODOS
    if (!capas[contadorCapas]->cargarImagen(path)) {
        cout << "error cargando imagen, en agregar Background"<< endl;
        return false;
    }
    int ancho = capas[contadorCapas]->getAncho();
    if (ancho>anchoMax) anchoMax=ancho;
    contadorCapas++;
    return true;
}

void Background::prepararEscenario(){
    //ASIGNO LAS VELOCIDADES DE CADA LAYER
    for (int i=0; i<contadorCapas ;i++){
        Layer *layer = capas[i];
        //layer->asignarVelocidad(double(anchoMax)/layer->getAncho());
        layer->asignarVelocidad(double(anchoMax));
    }
}

int Background::anchoMaximo(){
    return anchoMax;
}

void Background::transparentar(Uint8 alpha) {
    for (Layer* layer : this->capas)
        layer->transparentar(alpha);
}

Background::~Background() {
    for (int i = 0; i < capas.size(); i++) {
        Layer *layer = capas[i];
        delete layer;
    }
}

