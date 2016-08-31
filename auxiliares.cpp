#include <string.h>
#include "auxiliares.h"

char* chomp(char* str) {

	if (*str && str[strlen(str)-1] == '\n') {
		str[strlen(str)-1] = '\0';
	}

	return str;

}