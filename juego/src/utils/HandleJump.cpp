/*
 * HandleJump.cpp
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#include "HandleJump.h"
#include <chrono>
#include <signal.h>

using namespace chrono;

struct argkh {
    Cliente* cliente;
    int* keyPressed;
    bool* isKhOn;
    bool* isKhPaused;
};


void* handleJumpFunc(void* argKh) {

    bool* isKhOn = ( (argkh_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argkh_t*) argKh)->isKhPaused;
    int* keyPressed = ( (argkh_t*) argKh)->keyPressed;
    Cliente* cliente = ( (argkh_t*) argKh)->cliente;

    while (*isKhOn) {

        if (*isKhOn && !(*isKhPaused)) {

            for (int i = 0 ; i < 22 ; i++){
                string msj = to_string(*keyPressed);
                msj += "$1\n";
                cliente->enviarAusuario("TODOS", msj, false);
                usleep(40000);
            }
            *isKhPaused = true;
        }
    }

    return NULL;
}


HandleJump::HandleJump(Cliente *cliente) {
    this->cliente = cliente;
    isOn = false;
    argKeyHold = NULL;
    handleJumpTH = NULL;
    keyPressed = 0;
}

void HandleJump::On() {

    handleJumpTH = new pthread_t;

    if (!handleJumpTH)
        throw NoSePudoCrearThreadHandleJumpException();

    argKeyHold = new argkh_t;

    if (!argKeyHold)
        throw NoSePudoCrearThreadHandleJumpException();

    isOn = true;
    isPaused = false;

    argKeyHold->cliente = cliente;
    argKeyHold->isKhOn = &isOn;
    argKeyHold->isKhPaused = &isPaused;
    argKeyHold->keyPressed = &keyPressed;

    if (pthread_create(handleJumpTH, NULL, handleJumpFunc, argKeyHold))
        throw NoSePudoCrearThreadHandleJumpException();
}

void HandleJump::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*handleJumpTH, NULL);
}

void HandleJump::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void HandleJump::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}

void HandleJump::setKeyPressed(int key) {
    this->keyPressed = key;
}

HandleJump::~HandleJump() {
    delete handleJumpTH;
    delete argKeyHold;
}
