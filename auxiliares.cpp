#include <iostream>
#include <unistd.h>
#include <chrono>
#include <signal.h>
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

	chrono::time_point<chrono::system_clock> start;

	start = chrono::system_clock::now();

	while (*ishbOn) {

		chrono::time_point<chrono::system_clock> actual;
		actual = chrono::system_clock::now();

		chrono::duration<double> elapsed_seconds = actual.time_since_epoch() - start.time_since_epoch();

		if ( (elapsed_seconds.count()) >= 5 ) {

			if (*ishbOn && !(*ishbPaused)) {

				signal(SIGPIPE, SIG_IGN);
				ssize_t bytesEscritos = write(FD, "/H/\n", 4);

				if (bytesEscritos < 0) {
					perror("ERROR --> No se pudo enviar el heartbeat (Desconexion con el servidor)");
					close(FD);
					exit(1);
				}
			}
			start = chrono::system_clock::now();

		}
	}

	return NULL;
}
