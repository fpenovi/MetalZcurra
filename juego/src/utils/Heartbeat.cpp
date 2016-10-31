/*
 * Heartbeat.cpp
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <signal.h>
#include "Heartbeat.h"

struct arghb {
	int* fd;
	bool* ishbOn;
	bool* ishbPaused;
};


void* heartBeatFunc(void* arghb) {

	int FD = *(((arghb_t*) arghb)->fd);
	bool* ishbOn = (((arghb_t*) arghb)->ishbOn);
	bool* ishbPaused = (((arghb_t*) arghb)->ishbPaused);

	chrono::time_point<chrono::system_clock> start;

	start = chrono::system_clock::now();

	while (*ishbOn) {

		chrono::time_point<chrono::system_clock> actual;
		actual = chrono::system_clock::now();

		chrono::duration<double> elapsed_seconds = actual.time_since_epoch() - start.time_since_epoch();

		if ( (elapsed_seconds.count()) >= 5 ) {

			if (*ishbOn && !(*ishbPaused)) {

				signal(SIGPIPE, SIG_IGN);
				ssize_t bytesEscritos = write(FD, "/H/\n", 4);

				if (bytesEscritos < 0) {
					perror("ERROR --> No se pudo enviar el heartbeat (Desconexion con el servidor)");
					close(FD);
					exit(1);
				}
			}
			start = chrono::system_clock::now();

		}
	}

	return NULL;
}


Heartbeat::Heartbeat(int FD) {
	sockFileDesc = FD;
	isOn = false;
	heartBeatTh = NULL;
	argHeartB = NULL;
}

void Heartbeat::On() {

	heartBeatTh = new pthread_t;

	if (!heartBeatTh)
		throw NoSePudoCrearThreadHeartBeatException();

	argHeartB = new arghb_t;

	if (!argHeartB)
		throw NoSePudoCrearThreadHeartBeatException();

	isOn = true;
	isPaused = false;

	argHeartB->fd = &sockFileDesc;
	argHeartB->ishbOn = &isOn;
	argHeartB->ishbPaused = &isPaused;

	if (pthread_create(heartBeatTh, NULL, heartBeatFunc, argHeartB))
		throw NoSePudoCrearThreadHeartBeatException();
}

void Heartbeat::Off() {

	if (!isOn)
		return;

	isOn = false;
	pthread_join(*heartBeatTh, NULL);
}

void Heartbeat::Pause() {

	if (!isOn)
		return;

	isPaused = true;
}

void Heartbeat::Resume() {

	if (!isOn)
		return;

	isPaused = false;
}

Heartbeat::~Heartbeat() {
	delete heartBeatTh;
	delete argHeartB;
}

