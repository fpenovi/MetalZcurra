/*
 * Heartbeat.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include <pthread.h>
#include <stdexcept>

using namespace std;


class NoSePudoCrearThreadHeartBeatException : public runtime_error {

public:
	NoSePudoCrearThreadHeartBeatException() : runtime_error("No se pudo crear el thread Heartbeat") {	}
};
class NoSePudoCerrarThreadHeartBeatException : public runtime_error {

public:
	NoSePudoCerrarThreadHeartBeatException() : runtime_error("No se pudo cerrar el thread Heartbeat") {	}
};


typedef struct arghb arghb_t;


class Heartbeat {

private:
	int sockFileDesc;
	pthread_t* heartBeatTh;
	arghb_t* argHeartB;
	bool isOn;
	bool isPaused;

public:
	Heartbeat(int FD);

	void On();

	void Off();

	void Pause();

	void Resume();

	virtual ~Heartbeat();
};

#endif /* HEARTBEAT_H_ */
