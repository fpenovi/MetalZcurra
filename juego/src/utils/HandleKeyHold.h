/*
 * HandleKeyHold.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef HANDLEKEYHOLD_H_
#define HANDLEKEYHOLD_H_

#include <stdexcept>
#include <pthread.h>
#include "../Cliente.h"

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

	void On();

	void Off();

	void Pause();

	void Resume();

	void setKeyPressed(int key);

	int getKeyPressed();

	virtual ~HandleKeyHold();
};

#endif /* HANDLEKEYHOLD_H_ */
