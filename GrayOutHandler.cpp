//
// Created by franch on 28/11/16.
//

#include "GrayOutHandler.h"


void* grayOutScreen(void* _argGrey) {

	arggrey_t* argGrey = (arggrey_t*) _argGrey;

	Uint8* transparenciaActualptr = argGrey->transparenciaActual;
	int* transparenciaLimiteInferior = argGrey->transparenciaLimiteInferior;
	bool* isRunning = argGrey->isRunning;
	int decremento = argGrey->decremento;

	chrono::time_point<high_resolution_clock > start;
	time_point<high_resolution_clock> actual;
	start = high_resolution_clock::now();

	while (*transparenciaActualptr > *transparenciaLimiteInferior) {

		actual = high_resolution_clock::now();

		auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
		auto elapsed_ms = duration_cast<milliseconds>(deltaTiempo);

		if (elapsed_ms.count() >= argGrey->ms.count()) {
			*transparenciaActualptr += decremento;
			argGrey->juego->transparentar(*transparenciaActualptr);
			if (*transparenciaActualptr == 255) break;
			start = high_resolution_clock::now();
		}
	}

	cout << "TRANSPARENCIA: " << (int)*transparenciaActualptr << endl;
	*isRunning = false;

	return NULL;
}


GrayOutHandler::GrayOutHandler(Juego* juego, Uint8* transparenciaMax, int* transparenciaMin, milliseconds ms, int decremento, bool* isRunning) {

	this->argGrey = new arggrey_t{juego, transparenciaMax, transparenciaMin, ms, decremento, isRunning};
	this->grayOutTH = new pthread_t;
}


void GrayOutHandler::doWork() {
	pthread_create(grayOutTH, NULL, grayOutScreen, argGrey);
}


GrayOutHandler::~GrayOutHandler() {
	delete this->argGrey;
	delete this->grayOutTH;
}