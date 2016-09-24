#include <iostream>
#include <unistd.h>
#include <time.h>
#include <chrono>
#include "auxiliares.h"

using namespace std;


struct arghb {
	int* fd;
	bool* ishbOn;
	bool* ishbPaused;
};


void* heartBeatFunc(void* arghb) {

	int FD = *(((arghb_t*) arghb)->fd);
	bool* ishbOn = (((arghb_t*) arghb)->ishbOn);
	bool* ishbPaused = (((arghb_t*) arghb)->ishbPaused);

	std::chrono::time_point<std::chrono::system_clock> start;

	start = std::chrono::system_clock::now();

	while (*ishbOn) {

		std::chrono::time_point<std::chrono::system_clock> actual;
		actual = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = actual.time_since_epoch() - start.time_since_epoch();

		if ( (elapsed_seconds.count()) >= 5 ) {

			if (*ishbOn && !(*ishbPaused))
				write(FD, "/H/\n", 4);

			start = std::chrono::system_clock::now();

		}
	}

	return NULL;
}