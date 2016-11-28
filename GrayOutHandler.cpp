//
// Created by franch on 28/11/16.
//

#include "GrayOutHandler.h"


void* grayOutScreen(void* _argGrey) {

	arggrey_t* argGrey = (arggrey_t*) _argGrey;

	Uint8* transparenciaActualptr = argGrey->transparenciaActual;
	Uint8* transparenciaLimiteInferior = argGrey->transparenciaLimiteInferior;
	bool* isRunning = argGrey->isRunning;

	chrono::time_point<high_resolution_clock > start;
	time_point<high_resolution_clock> actual;
	start = high_resolution_clock::now();

	while (*transparenciaActualptr > *transparenciaLimiteInferior) {

		actual = high_resolution_clock::now();

		auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
		auto elapsed_ms = duration_cast<milliseconds>(deltaTiempo);

		if (elapsed_ms.count() >= argGrey->ms.count()) {
			*transparenciaActualptr -= 1;
			argGrey->juego->transparentar(*transparenciaActualptr);
			start = high_resolution_clock::now();
		}
	}

	*isRunning = false;

	return NULL;
}


GrayOutHandler::GrayOutHandler(Juego* juego, Uint8* transparenciaMax, Uint8* transparenciaMin, milliseconds ms, bool* isRunning) {

	this->argGrey = new arggrey_t{juego, transparenciaMax, transparenciaMin, ms, isRunning};
	this->grayOutTH = new pthread_t;
}


void GrayOutHandler::doWork() {
	pthread_create(grayOutTH, NULL, grayOutScreen, argGrey);
}


GrayOutHandler::~GrayOutHandler() {
	delete this->argGrey;
	delete this->grayOutTH;
}