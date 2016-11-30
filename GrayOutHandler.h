//
// Created by franch on 28/11/16.
//

#ifndef METALZCURRA_GRAYOUTHANDLER_H
#define METALZCURRA_GRAYOUTHANDLER_H

#include <pthread.h>
#include <chrono>
#include "Juego.h"

using namespace chrono;

class Juego;

typedef struct {
	Juego* juego;
	Uint8* transparenciaActual;
	int* transparenciaLimiteInferior;
	milliseconds ms;
	int decremento;
	bool* isRunning;
} arggrey_t;


class GrayOutHandler {

private:
	pthread_t* grayOutTH;
	arggrey_t* argGrey;

public:
	GrayOutHandler(Juego* juego, Uint8* transparenciaMax, int* transparenciaMin, milliseconds ms, int decremento, bool* isRunning);
	void doWork();
	~GrayOutHandler();

};


#endif //METALZCURRA_GRAYOUTHANDLER_H
