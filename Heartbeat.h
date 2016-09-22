#ifndef METALZCURRA_HEARTBEAT_H
#define METALZCURRA_HEARTBEAT_H

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

class Heartbeat {

private:
	int sockFileDesc;
	pthread_t* heartBeatTh;
	bool isOn;

public:
	Heartbeat(int FD);

	void On();

	void Off();
};


#endif //METALZCURRA_HEARTBEAT_H