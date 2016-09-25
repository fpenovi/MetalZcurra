#include <iostream>
#include <unistd.h>
#include <chrono>
#include <signal.h>
#include "auxiliares.h"
#include "Cliente.h"

using namespace std;


struct arghb {
	int* fd;
	bool* ishbOn;
	bool* ishbPaused;
	//Cliente* client;
};


void* heartBeatFunc(void* arghb) {

	int FD = *(((arghb_t*) arghb)->fd);
	bool* ishbOn = (((arghb_t*) arghb)->ishbOn);
	bool* ishbPaused = (((arghb_t*) arghb)->ishbPaused);
	//Cliente clienteAux = *(((arghb_t*) arghb)->client);

	std::chrono::time_point<std::chrono::system_clock> start;

	start = std::chrono::system_clock::now();

	while (*ishbOn) {

		std::chrono::time_point<std::chrono::system_clock> actual;
		actual = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = actual.time_since_epoch() - start.time_since_epoch();

		if ( (elapsed_seconds.count()) >= 5 ) {

			if (*ishbOn && !(*ishbPaused)) {

				// Initialize file descriptor sets
				fd_set read_fds, write_fds, except_fds;
				FD_ZERO(&read_fds);
				FD_ZERO(&write_fds);
				FD_ZERO(&except_fds);
				FD_SET(FD, &write_fds);

				// Seteo el timeout a 5 segundos
				struct timeval timeout;
				timeout.tv_sec = 5;
				timeout.tv_usec = 0;

				if (int rv = select(FD + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {

					signal(SIGPIPE, SIG_IGN);
					ssize_t bytesEscritos = write(FD, "/H/\n", 4);

					if (bytesEscritos < 0) {
						perror("ERROR --> No se pudo enviar el heartbeat");
						//clienteAux.salir();
					}
				}

				else if(rv == 0)
				{
					perror("ERROR --> timeout");
				}
				else{
					perror("ERROR --> select");
				}

			}
			start = std::chrono::system_clock::now();

		}
	}

	return NULL;
}
