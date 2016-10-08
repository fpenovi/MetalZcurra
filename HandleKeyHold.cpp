//
// Created by franch on 08/10/16.
//

#include <chrono>
#include <signal.h>
#include "HandleKeyHold.h"


using namespace chrono;

struct argkh {
	Cliente* cliente;
	int* keyPressed;
	bool* isKhOn;
	bool* isKhPaused;
};


void* handleKeyHoldFunc(void* argKh) {

	bool* isKhOn = ( (argkh_t*) argKh )->isKhOn;
	bool* isKhPaused = ( (argkh_t*) argKh)->isKhPaused;
	int* keyPressed = ( (argkh_t*) argKh)->keyPressed;
	Cliente* cliente = ( (argkh_t*) argKh)->cliente;

	time_point<high_resolution_clock> start;
	start = high_resolution_clock::now();
	microseconds intervalo(30000);

	while (*isKhOn) {
		time_point<high_resolution_clock> actual;
		actual = high_resolution_clock::now();

		auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
		auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

		if (elapsed_ms.count() >= intervalo.count()) {

			if (*isKhOn && !(*isKhPaused)) {
				string msj = to_string(*keyPressed);
				msj += "$1\n";
				cliente->enviarAusuario("TODOS", msj, false);
			}

			start = chrono::system_clock::now();
		}
	}

	return NULL;
}


HandleKeyHold::HandleKeyHold(Cliente *cliente) {
	this->cliente = cliente;
	isOn = false;
	argKeyHold = NULL;
	handleKeyHoldTH = NULL;
	keyPressed = 0;
}

HandleKeyHold::~HandleKeyHold() {
	delete handleKeyHoldTH;
	delete argKeyHold;
}

void HandleKeyHold::On() {

	handleKeyHoldTH = new pthread_t;

	if (!handleKeyHoldTH)
		throw NoSePudoCrearThreadHandleKeyHoldException();

	argKeyHold = new argkh_t;

	if (!argKeyHold)
		throw NoSePudoCrearThreadHandleKeyHoldException();

	isOn = true;
	isPaused = false;

	argKeyHold->cliente = cliente;
	argKeyHold->isKhOn = &isOn;
	argKeyHold->isKhPaused = &isPaused;
	argKeyHold->keyPressed = &keyPressed;

	if (pthread_create(handleKeyHoldTH, NULL, handleKeyHoldFunc, argKeyHold))
		throw NoSePudoCrearThreadHandleKeyHoldException();
}

void HandleKeyHold::Off() {
	if (!isOn)
		return;

	isOn = false;
}

void HandleKeyHold::Pause() {
	if (!isOn)
		return;

	isPaused = true;
}

void HandleKeyHold::Resume() {
	if (!isOn)
		return;

	isPaused = false;
}

void HandleKeyHold::setKeyPressed(int key) {
	this->keyPressed = key;
}