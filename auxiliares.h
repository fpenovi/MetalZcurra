#ifndef METALZCURRA_AUXILIARES_H
#define METALZCURRA_AUXILIARES_H


// Argumento que se le pasa a las funciones de los threads que se crean
// al atender a un nuevo cliente.
typedef struct argthread argthread_t;


// Función utilizada por los threads dedicados a los clientes en el servidor.
// Recibe por parametro un argthread_t.
void* procesarMensajes(void* arg);


// Función utilizada en el thread de control
// Recibe un puntero al estado del servidor y lee la entrada de usuario.
// En caso de el usuario ingresar '*', cambia el estado del servidor para comenzar el apagado.
void* controlInput(void* serverStatus);


#endif //METALZCURRA_AUXILIARES_H
