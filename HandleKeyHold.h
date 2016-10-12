//
// Created by franch on 08/10/16.
//

#ifndef METALZCURRA_HANDLEKEYHOLD_H
#define METALZCURRA_HANDLEKEYHOLD_H

#include <stdexcept>
#include <pthread.h>
#include "Cliente.h"

using namespace std;

class NoSePudoCrearThreadHandleKeyHoldException : public runtime_error {

public:
	NoSePudoCrearThreadHandleKeyHoldException() : runtime_error("No se pudo crear el thread HandleKeyHold") {	}
};

class NoSePudoCerrarThreadHandleKeyHoldException : public runtime_error {

public:
	NoSePudoCerrarThreadHandleKeyHoldException() : runtime_error("No se pudo cerrar el thread HandleKeyHold") {	}
};


typedef struct argkh argkh_t;

class HandleKeyHold {

private:
	Cliente* cliente;
	pthread_t* handleKeyHoldTH;
	argkh_t* argKeyHold;
	bool isOn;
	bool isPaused;
	int keyPressed;

public:
	HandleKeyHold(Cliente* cliente);

	~HandleKeyHold();

	void On();

	void Off();

	void Pause();

	void Resume();

	void setKeyPressed(int key);
};


#endif //METALZCURRA_HANDLEKEYHOLD_H