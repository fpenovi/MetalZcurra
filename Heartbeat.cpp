#include "Heartbeat.h"


using namespace std;

struct arghb {
	int* fd;
	bool* ishbOn;
	bool* ishbPaused;
};

Heartbeat::Heartbeat(int FD) {
	sockFileDesc = FD;
	isOn = false;
	heartBeatTh = NULL;
	argHeartB = NULL;
}

Heartbeat::~Heartbeat() {
	delete heartBeatTh;
	delete argHeartB;
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

	/*if (pthread_cancel(*heartBeatTh) != 0)
		throw NoSePudoCerrarThreadHeartBeatException();*/


	isOn = false;
	delete heartBeatTh;
	heartBeatTh = NULL;
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