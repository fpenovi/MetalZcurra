//
// Created by nestor on 05/11/16.
//

#include "Texto.h"

Texto::Texto(string tit, SDL_Renderer* render){
    titulo = tit;
    renderizador = render;
    texturaTitulo = new Textura(renderizador);
    texturaInput = new Textura(renderizador);
    inputText = "";
}

bool Texto::cargarTitulo(){
    bool success = true;
    gFont = TTF_OpenFont( "imag/neogray.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "FALLO CARGANDO EL TIPO DE LETRA! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //lo pongo en negro
        SDL_Color textColor = {255, 0, 0, 0xFF };
        if( !texturaTitulo->loadFromText( titulo, textColor, gFont ) )
        {
            printf( "Fallo cargando texto!\n" );
            success = false;
        }
    }
}

bool Texto::pedir(){

    //Main loop flag
    bool quit = false;

    SDL_Event e;

    //color del texto = rojo
    SDL_Color textColor = { 255, 0, 0, 0xFF };

    texturaInput->loadFromText( inputText.c_str(), textColor, gFont );

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
            texturaInput->loadFromText( inputText.c_str(), textColor, gFont  );
        }
            //Text is empty
        else
        {
            //Render space texture
            texturaInput->loadFromText( " ", textColor, gFont  );
        }
    }
    texturaTitulo->render(140,350,NULL, 0.0, NULL, SDL_FLIP_NONE );
    texturaInput->render(285,350,NULL, 0.0, NULL, SDL_FLIP_NONE);

    SDL_StopTextInput();
    return quit;
}


string Texto::getTexto(){
    return inputText;
}

void Texto::renderTitulo(int x,int y){

    texturaTitulo->render(x,y,NULL, 0.0, NULL, SDL_FLIP_NONE);
}
