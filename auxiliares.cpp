#include <iostream>
#include <unistd.h>
#include <time.h>
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

	clock_t start = clock();

	while (*ishbOn) {

		if ( (clock() - start) / CLOCKS_PER_SEC >= 5 ) {

			if (*ishbOn && !(*ishbPaused))
				write(FD, "/H/\n", 4);

			start = clock();
		}
	}

	return NULL;
}