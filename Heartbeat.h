#ifndef METALZCURRA_HEARTBEAT_H
#define METALZCURRA_HEARTBEAT_H

#include <pthread.h>
#include <stdexcept>
#include "auxiliares.h"

using namespace std;


class NoSePudoCrearThreadHeartBeatException : public runtime_error {

public:
	NoSePudoCrearThreadHeartBeatException() : runtime_error("No se pudo crear el thread Heartbeat") {	}
};
class NoSePudoCerrarThreadHeartBeatException : public runtime_error {

public:
	NoSePudoCerrarThreadHeartBeatException() : runtime_error("No se pudo cerrar el thread Heartbeat") {	}
};

class Heartbeat {

private:
	int sockFileDesc;
	pthread_t* heartBeatTh;
	arghb_t* argHeartB;
	bool isOn;
	bool isPaused;

public:
	Heartbeat(int FD);

	~Heartbeat();

	void On();

	void Off();

	void Pause();

	void Resume();
};


#endif //METALZCURRA_HEARTBEAT_H