#ifndef METALZCURRA_AUXILIARES_H
#define METALZCURRA_AUXILIARES_H


#include <unordered_map>

using namespace std;

// Argumento que se le pasa a las funciones de los threads que se crean
// al atender a un nuevo cliente.
typedef struct argthread argthread_t;

// Argumento que se le pasa a la funcion heartBeatFunc.
typedef struct arghb arghb_t;

// Funcion de latido que corre en el thread de HeartBeat. Es la encargada de mandar un mensaje
// Al FD pasado por parametro cada N segundos para mantener viva la conexión con el servidor.
void* heartBeatFunc(void* arghb);

#endif //METALZCURRA_AUXILIARES_H