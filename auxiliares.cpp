#include <iostream>
#include <unistd.h>
#include "auxiliares.h"

using namespace std;


void* controlInput(void* serverStatus) {

	cout << "Entre al thread de control" << endl;

	string input;
	while (true) {
		cin >> input;

		if (input.compare("*") == 0) {
			*((bool*) serverStatus) = false;
			return NULL;
		}
	}
}