//
// Created by juancho on 02/10/16.
//

#ifndef METALZCURRA_TEXTURA_H
#define METALZCURRA_TEXTURA_H

using namespace std;

class Textura{
private:
    //The actual hardware texture
    SDL_Texture* textura;
    int ancho;
    int alto;
    SDL_Renderer* renderizador;


public:
    Textura(SDL_Renderer* renderizador);
    ~Textura();
    bool cargarImagen( std::string path);
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    int getAncho();
    int getAlto();
    void free();
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
};


#endif //METALZCURRA_TEXTURA_H
