#include "Heartbeat.h"
#include "auxiliares.h"


using namespace std;

Heartbeat::Heartbeat(int FD) {
	sockFileDesc = FD;
	isOn = false;
	heartBeatTh = NULL;
}

void Heartbeat::On() {

	heartBeatTh = new pthread_t;

	if (!heartBeatTh)
		throw NoSePudoCrearThreadHeartBeatException();

	if (pthread_create(heartBeatTh, NULL, heartBeatFunc, &sockFileDesc))
		throw NoSePudoCrearThreadHeartBeatException();

	isOn = true;
}

void Heartbeat::Off() {
	if (!isOn)
		return;

	if (pthread_cancel(*heartBeatTh) != 0)
		throw NoSePudoCerrarThreadHeartBeatException();

	delete heartBeatTh;
	isOn = false;
	heartBeatTh = NULL;
}