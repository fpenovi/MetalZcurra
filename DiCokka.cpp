//
// Created by nestor on 30/11/16.
//

#include "DiCokka.h"
#include "ObjectManager.h"

DiCokka::DiCokka(int x, int y, int delta) : Boss(x, y, delta) {
    this->velocidad = 7;
    this->vida = 3000;
    this->id = 3;
    this->envolvente->agregarComponente(new Rectangulo(&this->x, &this->y, 152, 122));
    girando=NOGIRO;
    disparando=NODISPARANDO;
}

void DiCokka::animacionMover(){
    ++frameMover;
    if( frameMover>= ANIMACION_MOVER ) frameMover = 0;
}

void DiCokka::animacionMuerte(){
    ++frameMuerte;
    if( frameMuerte >= ANIMACION_MUERTE ) frameMuerte = 0;
}

void DiCokka::animacionSalidaTiro(){
    ++frameSalidaTiro;
    if( frameSalidaTiro>= ANIMACION_SALIDA_TIRO ) {
        frameSalidaTiro = 0;
        disparando=NODISPARANDO;
    }
}
void DiCokka::animacionDisparar(){
    ++frameDisparar;
    if( frameDisparar >= ANIMACION_DISPARAR ) {
        frameDisparar = 0;
        disparando=TERMINADO;
    }
}
void DiCokka::animacionGirarADerecha(){
    ++frameGirar;
    if( frameGirar >= ANIMACION_GIRAR ) {
        frameGirar--;
        girando=NOGIRO;
    }
}
void DiCokka::animacionGirarAIzquierda(){
    --frameGirar;
    if( frameGirar <= 0 ) {
        frameGirar=0;
        girando=NOGIRO;
    }
}

void DiCokka::animacionGirar(){
    if (girando==DERECHA) {
        animacionGirarADerecha();
    }
    if (girando==IZQUIERDA) {
        animacionGirarAIzquierda();
    }
}

void DiCokka::mover(){
    if (!estaVivo()) existe = false;
    if (!disparando) disparar();
    if (girando!=NOGIRO || disparando==DISPARANDO) {
        setSprites();
        return;
    }
    if (derecha) moverDerecha();
    else moverIzquierda();
}

void DiCokka::moverIzquierda(){
    x -= velocidad;
    setSprites();
    if (x < 0) {
        derecha = true;
        girando=DERECHA;
        frameGirar = 0;
    }
}

void DiCokka::moverDerecha(){
    x += velocidad;
    setSprites();
    if (x + envolvente->getAncho() > 800) {
        derecha = false;
        girando=IZQUIERDA;
        frameGirar = 0;
    }
}


void DiCokka::setSprites(){
    if (girando==NOGIRO) {
        if (disparando==DISPARANDO) animacionDisparar();
        else if (disparando==TERMINADO) {
            animacionSalidaTiro();
            animacionMover();
        }
        else animacionMover();
    }
    else animacionGirar();
}


void DiCokka::crear() {
    ObjectManager* objectManager = ObjectManager::getInstance();
    if ((*(objectManager->getPosX()) + 800) > envolvente->getX() ) {
        existe = true;
        x = 800;
    }

}

int DiCokka::getSprites() {
    return 0;
}

void DiCokka::spriteDisparar() {
    disparando=DISPARANDO;
}

bool DiCokka::disparar() {
    microseconds intervalo(2000000);	// 2s
    actual = high_resolution_clock::now();

    auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
    auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

    if (elapsed_ms.count() >= intervalo.count() && estaVivo()) {
        spriteDisparar();
        ObjectManager* objectManager = ObjectManager::getInstance();
        objectManager->inicializarBalaTanque(x, y, derecha);
        objectManager->inicializarBalaTanque(x + 60, y, derecha);
        objectManager->inicializarBalaTanque(x + 120, y, derecha);
        start = chrono::system_clock::now();
    }
}

int DiCokka::getGirando() {
    return girando;
}

int DiCokka::getDisparando() {
    return disparando;
}

int DiCokka::getSpriteMover() {
    return frameMover;
}

DiCokka::~DiCokka() {}
