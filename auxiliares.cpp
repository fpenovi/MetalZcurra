#include <iostream>
#include <unistd.h>
#include <time.h>
#include "auxiliares.h"

using namespace std;

void* heartBeatFunc(void* fd) {


	clock_t start = clock();
	time_t tiempo = time(0);

	while (true) {

		if ( (clock() - start) / CLOCKS_PER_SEC >= 5 ) {
			time_t tiempo= time(0);
			struct tm *tlocal = localtime(&tiempo);
			char output[128];
			strftime(output,128,"%d/%m/%y %H:%M:%S ",tlocal);
			string horaYFecha(output);
			cout << "Holas " << output << endl;
			start = clock();
		}
	}
	return NULL;
}