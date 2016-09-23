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
	time_t tiempo = time(0);

	while (*ishbOn) {

		if ( (clock() - start) / CLOCKS_PER_SEC >= 5 ) {
			time_t tiempo = time(0);
			struct tm *tlocal = localtime(&tiempo);
			char output[128];
			strftime(output,128,"%d/%m/%y %H:%M:%S ",tlocal);
			//cout << "Holas " << output << endl; --> cout no es thread safe... esto caga el MENU

			if (*ishbOn && !(*ishbPaused))
				printf("Holas %s\n", output);

			start = clock();
		}
	}

	return NULL;
}