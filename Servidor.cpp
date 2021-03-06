#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unordered_map>
#include <vector>
#include <list>
#include "auxiliares.h"
#include "Mensaje.h"
#include "Log.h"
#include <SDL2/SDL.h>
#include <signal.h>
#include "ProtocoloComando.h"
#include "ProtocoloVistaUpdate.h"
#include "Personaje.h"
#include "ObjectManager.h"
#include "ParserXML.h"
#include "HandleKeyHoldServer.h"
#include "HandleJumpServer.h"
#include "HandleDisparoServer.h"
#include "NivelManager.h"


#define MAX_CLIENTS 6
#define INDIVIDUAL 0
#define COLABORATIVO 1
#define GRUPAL 2

using namespace std;

struct argthread {
    int clientFD= 0;
    pthread_t* thread= NULL;
    char* user=NULL;
    char* clave= NULL;
    bool* quit = NULL;
};

class NoSePudoCrearServidorException : public runtime_error {

public:
    NoSePudoCrearServidorException(string error) : runtime_error(error) {
    }
};


class XmlModoDeJuegoException: public runtime_error {

public:
    XmlModoDeJuegoException(string error) : runtime_error(error) {}
};


class Servidor {

private:
    static int fileDescrpt;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t pesoCli_adrr;
    pthread_t threadControl;
    pthread_attr_t attr;
    bool serverOn = false;
    static int cantidadUsuarios;
    static vector<argthread_t*> conectados;
    static pthread_mutex_t mutex_login;
    static Log logger;
    static ObjectManager* objectManager;
    static unordered_map<string, list<Mensaje*>*> conectadosHash;
    static unordered_map<string, pthread_mutex_t> mutexesHash;
    static int modoJuego;

    static void *controlInput(void *serverStatus) {

        cout << "ESPERANDO CLIENTES" << endl << "Presione * para salir..." << endl;

        string input;
        while (true) {
            cin >> input;

            if (input.compare("*") == 0) {
                *((bool *) serverStatus) = false;
                cerrarConexiones();
                usleep(100000);
                shutdown(fileDescrpt, SHUT_RDWR);
                delete objectManager;
                delete NivelManager::getInstance();
                return NULL;
            }
        }
    }

    static void cerrarConexiones() {
        size_t numConexiones = conectados.size();

        for (size_t i = 0; i < numConexiones; i++) {
            argthread_t *actual = conectados[i];
            shutdown(actual->clientFD, SHUT_RDWR);
        }

        for (auto kv : conectadosHash)
            delete kv.second;
    }

    static bool pedirLogin(FILE *mensajeCliente, argthread_t *arg) {

        size_t len = 0;
        char* user = NULL;
        char* pass = NULL;
        ssize_t bytesLeidos = 0;

        // Pido el usuario al cliente
        bytesLeidos = getline(&user, &len, mensajeCliente);
        if (bytesLeidos <= 0){
            perror("ERROR --> Al leer nombre de usuario\n");
            return false;
        }
        bytesLeidos = getline(&pass, &len, mensajeCliente);
        if (bytesLeidos <= 0){
            perror("ERROR --> Al leer contrasena\n");
            return false;
        }

        free(pass);
        pass = (char *) "1234";

        int result = pthread_mutex_lock(&mutex_login);
        if (result != 0) perror("Fallo el pthread_mutex_lock en login");

        // Chequeo si el user ya esta conectado
/*        vector<argthread_t *>::iterator it;
        for (it = conectados.begin(); it != conectados.end();) {
            if (it.operator*()->user != NULL){
                if (strcmp(it.operator*()->user,user) == 0){
                    cout << user << "Ya esta conectado" << endl;
                    free(user);
                    result = pthread_mutex_unlock(&mutex_login);
                    if (result != 0) perror("Fallo el pthread_mutex_unlock en login");
                    return false;
                }
            }
            it++;
        }*/

        for (argthread_t* cliente : conectados){
            if (cliente->user != NULL){
                if (strcmp(cliente->user, user) == 0) {
                    cout << user << "Ya esta conectado" << endl;
                    free(user);
                    result = pthread_mutex_unlock(&mutex_login);
                    if (result != 0) perror("Fallo el pthread_mutex_unlock en login");
                    return false;
                }
            }
        }

        result = pthread_mutex_unlock(&mutex_login);
        if (result != 0) perror("Fallo el pthread_mutex_unlock en login");

        arg->user = user;
        arg->clave = pass;
        return true;
    }

    static void kickearUsuario(argthread_t* arg) {
        vector<argthread_t*>::iterator it;

        int result = pthread_mutex_lock(&mutex_login);
        if (result != 0) perror("Fallo el pthread_mutex_lock en kick");

        for (it = conectados.begin(); it != conectados.end();) {

            if (it.operator*()->user == NULL){
                it = conectados.erase(it);
                result = pthread_mutex_unlock(&mutex_login);
                if (result != 0) perror("Fallo el pthread_mutex_unlock en kick");
                return;
            }

            else if (it.operator*()->user != NULL && arg->user != NULL){
                if (strcmp(it.operator*()->user, arg->user) == 0){
                    free(it.operator*()->user);
                    it = conectados.erase(it);
                    result = pthread_mutex_unlock(&mutex_login);
                    if (result != 0) perror("Fallo el pthread_mutex_unlock en kick");
                    return;
                }
            }
            it++;
        }

        result = pthread_mutex_unlock(&mutex_login);
        if (result != 0) perror("Fallo el pthread_mutex_unlock en kick");
    }

    static void agregarMensaje(char* emisorChar, char *textoInicial, ssize_t largo, HandleKeyHoldServer* handleKeyHold, HandleJumpServer* handleJump, int FD, HandleDisparoServer* handleDisparo) {

        if (textoInicial == NULL){
            printf("ERROR");
            exit(1);
        }

        string emisor(emisorChar);
        emisor.erase(emisor.length()-1);

        int* posX = objectManager->getPosX();
        int idEmisor = objectManager->getIdByUsername(emisor);
        Direccion* direccion = objectManager->getDireccionById(idEmisor);
        Personaje* personaje = objectManager->getObject(idEmisor);

        int key;
        int pressed;
        string stream(&textoInicial[6]); //Esto hace magia NO TOCAR

        ProtocoloComando::parse(stream, &key, &pressed);

        ProtocoloVistaUpdate update;
        int aux;
        bool enviar = false;

        if ( pressed == 1 ) {

            switch( key ) {

                case SDLK_LEFT:
                    personaje->setQuieto(false);
                    handleKeyHold->setKeyPressed(SDLK_LEFT);
                    handleKeyHold->Resume();

                    direccion->disparoAlaIzq();
                    break;

                case SDLK_RIGHT:
                    personaje->setQuieto(false);
                    handleKeyHold->setKeyPressed(SDLK_RIGHT);
                    handleKeyHold->Resume();

                    direccion->disparoAlaDer();
                    break;

                case SDLK_UP:
                    direccion->disparoArriba();
                    personaje->setAbajo(false);
                    personaje->setArriba(true);
                    break;

                case SDLK_DOWN:
                    direccion->disparoAbajo();
                    personaje->setArriba(false);
                    personaje->setAbajo(true);
                    break;

                case SDLK_x:
                    personaje->setQuieto(false);
                    handleJump->Resume();
                    break;

                case SDLK_RETURN:
                    personaje->setQuieto(false);
                    objectManager->reinicializarEscenario();

                    update.setTipoObjeto(10);
                    update.setObject_id(100);
                    update.setEstado(0);
                    update.setX(0);
                    update.setY(0);
                    update.setPosCamara(0);
                    update.setConectado(0);
                    update.setSpriteIndex(0);
                    update.setApuntando(0);
                    update.setSaltando(0);
                    update.setPuntaje(0);
                    enviar = true;
                    break;

                case SDLK_z:
                    handleDisparo->Resume();
                    break;

                case SDLK_i:
                    objectManager->modoInvencible();
                    break;
            }
        }

        else if ( pressed == 0 ) {

            //Adjust the velocity
            switch( key ) {

                case SDLK_LEFT:
                    if (handleKeyHold->getKeyPressed() == SDLK_LEFT) {
                        handleKeyHold->Pause();
                        handleKeyHold->setKeyPressed(0);
                        personaje->setQuieto(true);
                        direccion->setSolte(true);
                    }
                    if (direccion->isArriba() || direccion->isAbajo()) direccion->setIzquierda(false);
                    break;

                case SDLK_RIGHT:
                    if (handleKeyHold->getKeyPressed() == SDLK_RIGHT) {
                        handleKeyHold->Pause();
                        handleKeyHold->setKeyPressed(0);
                        personaje->setQuieto(true);
                        direccion->setSolte(true);
                    }
                    if (direccion->isArriba() || direccion->isAbajo()) direccion->setDerecha(false);
                    break;

                case SDLK_UP:
                    direccion->setArriba(false);
                    direccion->ultimaDireccion();
                    personaje->setArriba(false);
                    break;

                case SDLK_DOWN:
                    direccion->setAbajo(false);
                    direccion->ultimaDireccion();
                    personaje->setAbajo(false);
                    break;

                case SDLK_x:
                    handleJump->Pause();
                    if (direccion->isSolte()) personaje->setQuieto(true);;
                    break;

                case SDLK_RETURN:
                    break;

                case SDLK_z:
                    //handleDisparo->Pause();
                    break;

            }
        }

        if (enviar) {
            int result; // para el mutex

            string mensaje = update.toString();

            for (auto kv : conectadosHash) {

                Mensaje *mensajeNuevo = new Mensaje(emisor, kv.first, mensaje);

                result = pthread_mutex_lock(&mutexesHash[kv.first]);
                if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

                kv.second->push_back(mensajeNuevo);

                result = pthread_mutex_unlock(&mutexesHash[kv.first]);
                if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
            }

            if (NivelManager::getInstance()->hayMasNiveles()) {
                objectManager->pausarManagers();

                objectManager->liberarEnemigos();
                objectManager->reiniciarBonuses();
                NivelManager::getInstance()->siguienteNivel();
                objectManager->enviarNuevoBackground(emisor);

                objectManager->reanudarManagers();

                cout << "REINICIO LVL" << endl;
            }

            //else ToDo TERMINAR JUEGO
        }
    }

    static bool enviarMensaje(argthread_t* arg, char* linea, ssize_t* bytesLeidos, HandleKeyHoldServer* handler, HandleJumpServer* handleJump, int FD, HandleDisparoServer* handleDisparo) {

        if (*bytesLeidos < 0) {
            cout << " SE DESCONECTÓ " << arg->user << endl;
            free(linea);
            return false;
        }

        agregarMensaje(arg->user, linea, *bytesLeidos, handler, handleJump, FD, handleDisparo);
        return true;

    }

    static void *recibirMensajes(void* arg) {

        int sockNewFileDescrpt = ((argthread_t *) arg)->clientFD;
        string receptor = ((argthread_t *) arg)->user;
        bool* quit= ((argthread_t *) arg)->quit;

        receptor.erase(receptor.length() - 1);
        int result; //para el mutex

        // obtengo la sublista de mi jugador
        list<Mensaje*>* auxLista = conectadosHash[receptor];

        while(!(*quit)){

            if(auxLista->size() != 0){

                //cout << receptor << ": " << auxLista->size() << endl;

                Mensaje* mensaje = auxLista->front();
                string mensajeEmisor = mensaje->getMensaje();
                const char *mensajeChar = mensajeEmisor.c_str();

                signal(SIGPIPE, SIG_IGN);
                ssize_t bytesEscritos = write(sockNewFileDescrpt, mensajeChar, mensajeEmisor.length());

                if (bytesEscritos <= 0) {
                    perror("ERROR --> Cliente se desconectó inesperadamente");
                    return NULL;
                }

                //Lockeo el mutex para borrar de la lista del jugador
                result = pthread_mutex_lock(&mutexesHash[receptor]);
                if (result != 0) perror("Fallo el pthread_mutex_lock en recibir msj");

                auxLista->pop_front();

                result = pthread_mutex_unlock(&mutexesHash[receptor]);
                if (result != 0) perror("Fallo el pthread_mutex_lock en recibir msj");

                delete mensaje;
            }
        }
        return NULL;
    }

    static void *procesarMensajes(void *arg) {
        char *linea;
        size_t len = 0;
        ssize_t bytesLeidos;
        ssize_t bytesEscritos;
        int sockNewFileDescrpt = ((argthread_t *) arg)->clientFD;
        pthread_t *thread = ((argthread_t *) arg)->thread;
        FILE *mensajeCliente = fdopen(sockNewFileDescrpt, "r");

        // Registro del usuario
        if (!pedirLogin(mensajeCliente, (argthread_t *) arg)) {
            if (write(sockNewFileDescrpt, "fallo la conexion al sistema.\n", 30) < 0 ) {
                perror("ERROR --> No se pudo informar de error al cliente");
                // ToDo logger
            }
            fclose(mensajeCliente);
            close(sockNewFileDescrpt);
            kickearUsuario((argthread_t*) arg);
            free(arg);
            free(thread);
            return NULL;    // Salgo del thread
        }

        bytesEscritos = write(sockNewFileDescrpt, "conectado al servidor\n", 22);
        string userCon(((argthread_t *) arg)->user);
        userCon.erase(userCon.length()-1);
        logger.loggearConexion(userCon);
        cout << "\033[1m\033[32mSe conectó \033[1m\033[32m" << ((argthread_t *) arg)->user << "\033[0m";

        // REGISTRO EL USUARIO Y LE ASIGNO UN ID VINCULADO A UN PERSONAJE
        cout << userCon << endl;
        objectManager->registerUser(userCon);

        auto it = conectadosHash.find(userCon);
        if (it == conectadosHash.end())
            conectadosHash[userCon] = new list<Mensaje*>;

        mutexesHash[userCon] = PTHREAD_MUTEX_INITIALIZER;
        objectManager->conectarPersonaje(userCon);

        // ENVIO DATOS
        objectManager->enviarEscenario(sockNewFileDescrpt, cantidadUsuarios, modoJuego);
        objectManager->enviarPersonajes(sockNewFileDescrpt, userCon);

        int cant;
        // SALA DE ESPERA
        while (conectadosHash.size() != cantidadUsuarios){
            cout << "USUARIOS CONECTADOS: " << conectados.size() << " / NECESARIOS: " << cantidadUsuarios << endl;
        }
        usleep(100000);

        // ENVIO USUARIOS
        unordered_map<string, int>* usuarios = objectManager->getTablaUsuarios();
        for (auto kv : *usuarios){
            string nombre = kv.first;
            string id = to_string(kv.second);
            string msj = nombre + "$" + id + "\n";
            ssize_t bytes = write(sockNewFileDescrpt, msj.c_str(), msj.size());
        }

        write(sockNewFileDescrpt, "$\n", 2);

        // Creo thread para enviar mensajes al cliente
        bool quit = false;
        (((argthread_t *) arg)->quit) = &quit;
        pthread_t *recibirThread = new pthread_t;
        pthread_create(recibirThread, NULL, recibirMensajes, arg);

        // Creo thread de movimiento
        HandleKeyHoldServer* handleKeyHoldServer = new HandleKeyHoldServer();
        handleKeyHoldServer->setEmisor(userCon);
        handleKeyHoldServer->On();
        handleKeyHoldServer->Pause();

        // Creo thread de salto
        HandleJumpServer* handleJumpServer = new HandleJumpServer();
        handleJumpServer->setKeyPressed(SDLK_UP);
        handleJumpServer->setEmisor(userCon);
        handleJumpServer->On();
        handleJumpServer->Pause();

        // Creo thread de disparo
        HandleDisparoServer* handleDisparoServer = new HandleDisparoServer();
        handleDisparoServer->setEmisor(userCon);
        handleDisparoServer->On();
        handleDisparoServer->Pause();

        while (true) {

            bytesLeidos = getline(&linea, &len, mensajeCliente);

            if (bytesLeidos < 0) {
                string userDesc(((argthread_t *) arg)->user);
                userDesc.erase(userDesc.length()-1);

                objectManager->desconectarPersonaje(userCon);
                enviarDesconexion(userCon);

                logger.loggearDesconexionViolenta(userDesc);
                cout << "\033[1;31mSe desconectó MAL " << ((argthread_t *) arg)->user << "\033[0m";
                free(linea);
                break;
            }

            //cout << "debug: Mensaje recibido del cliente: " << linea;

            // Opcion enviar mensaje
            if (strcmp(linea, "/E/\n") == 0) {
                free(linea);
                linea = NULL;
                bytesLeidos = getline(&linea, &len, mensajeCliente);

                // ENVIO MENSAJE
                if ( !enviarMensaje((argthread_t*) arg, linea, &bytesLeidos, handleKeyHoldServer, handleJumpServer, sockNewFileDescrpt, handleDisparoServer) )
                    break;
            }

                // Desconecto al cliente desde el servidor
            else if (strcmp(linea, "/D/\n") == 0) {
                free(linea);
                linea = NULL;
                string userDesc(((argthread_t *) arg)->user);
                userDesc.erase(userDesc.length()-1);

                objectManager->desconectarPersonaje(userCon);
                enviarDesconexion(userCon);

                logger.loggearDesconexionNormal(userDesc);
                cout << "\033[32mSe desconectó BIEN \033[32m" << ((argthread_t*) arg)->user << "\033[0m";
                break;
            }

            else if (strcmp(linea, "/Z/\n") == 0) {
                if ((bytesEscritos = write(sockNewFileDescrpt, "/Z/\n", 4)) <= 0) {
                    cout << "El cliente se desconecto del servidor1" << endl;
                    break;
                }
            }

            else if (strcmp(linea, "/H/\n") == 0) {
                free(linea);
                linea = NULL;
            }

            else {
                cout << "El cliente se desconecto del servidor2" << endl;
                free(linea);
                linea = NULL;
                break;
            }

            free(linea);
            linea = NULL;

        }

        kickearUsuario((argthread_t*) arg);
        *(((argthread_t *) arg)->quit) = true;
        pthread_join(*recibirThread, NULL);

        close(sockNewFileDescrpt);
        free(arg);
        free(thread);
        free(recibirThread);
        fclose(mensajeCliente);

        handleDisparoServer->Off();
        delete handleDisparoServer;

       
        handleKeyHoldServer->Off();
        delete handleKeyHoldServer;


        handleJumpServer->Off();
        delete handleJumpServer;
        return NULL;
    }

    static void enviarDesconexion(string emisor){

        int idEmisor = objectManager->getIdByUsername(emisor);
        Personaje* personaje = objectManager->getObject(idEmisor);
        int* posX = objectManager->getPosX();

        ProtocoloVistaUpdate update;
        update.setTipoObjeto(1);
        update.setEstado(personaje->getSeMovio());
        update.setX(*posX);
        update.setY(personaje->getPosy());
        update.setObject_id(idEmisor);
        update.setPosCamara(personaje->getPosCamara());
        update.setConectado(personaje->getConectado());
        update.setApuntando(personaje->getDireccion());
        update.setSaltando(personaje->getSaltando());
        update.setPuntaje(personaje->getPuntaje());

        int result;
        string mensaje = update.toString();

        for (auto kv : conectadosHash) {

            Mensaje* mensajeNuevo = new Mensaje(emisor, kv.first, mensaje);

            result = pthread_mutex_lock(&mutexesHash[kv.first]);
            if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

            kv.second->push_back(mensajeNuevo);

            result = pthread_mutex_unlock(&mutexesHash[kv.first]);
            if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
        }

    }

public:
    Servidor(unsigned short int numPuerto) {

        bzero(&attr, sizeof(attr));

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        mutex_login = PTHREAD_MUTEX_INITIALIZER;


        // Creo thread de CONTROL
        if (pthread_create(&threadControl, &attr, controlInput, &serverOn) != 0) {
            cerr << "ERROR --> No se pudo crear thread de control" << endl;
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException("ERROR: No se pudo crear el Thread de Control");
        }


        bzero(&serv_addr, sizeof(serv_addr));   // Inicializo structs
        bzero(&cli_addr, sizeof(cli_addr));

        fileDescrpt = socket(AF_INET, SOCK_STREAM, 0);

        if (fileDescrpt < 0) {
            logger.error("no se pudo abrir el socket");
            cerr << "ERROR --> No se pudo abrir socket" << endl;
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException("ERROR: No se pudo abrir el socket");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(numPuerto);  // --> convierte de hostbyte order a network byte order

        // Aviso al SO que asocie el programa al socket creado
        if (bind(fileDescrpt, (const sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            logger.error("no se pudo crear servidor");
            close(fileDescrpt);
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException("ERROR: No se pudo hacer bind con el socket");
        }

        // Comienza a escuchar a los clientes que se quieren conectar y los va encolando
        if (listen(fileDescrpt, MAX_CLIENTS) < 0) {
            logger.error("se alcanzo la cant maxima de clientes");
            cerr << "ERROR --> Falla en listen" << endl;
            close(fileDescrpt);
            pthread_attr_destroy(&attr);
            throw NoSePudoCrearServidorException("ERROR: No se pudieron aceptar clientes");
        }

        serverOn = true;
        logger.inicializoServer();

    }

    void initJuego(char* docname) {
        string path(docname);
        ParserXML* parser = new ParserXML(path);

        objectManager->setTamVentana(parser->tamVentana());

        vector<string> opciones = parser->opcionesJuego();
        modoJuego = stoi(opciones[0]);
        cantidadUsuarios = stoi(opciones[1]);

        if (modoJuego != INDIVIDUAL && modoJuego != COLABORATIVO && modoJuego != GRUPAL)
            throw XmlModoDeJuegoException("XML ERROR: Modo de Juego inexistente. Opciones: \n0.INDIVIDUAL\n1.COLABORATIVO\n2.GRUPAL");

        if (cantidadUsuarios < 1 || cantidadUsuarios > 4)
            throw XmlModoDeJuegoException("XML ERROR: La cantidad de jugadores debe ser entre 1 y 4");

        if (modoJuego == GRUPAL && cantidadUsuarios < 4)
            throw XmlModoDeJuegoException("XML ERROR: Modo de Juego GRUPAL solo puede jugarse de a 4 jugadores");


        NivelManager::getInstance()->setXmlNiveles(parser->xmlNiveles());

        objectManager->setConectadosHash(&conectadosHash);
        objectManager->setMutexesHash(&mutexesHash);
        objectManager->crearPersonajes(cantidadUsuarios);
        objectManager->crearBalas(260);
        objectManager->crearBalasManager();
        objectManager->crearEnemigosManager();
        objectManager->crearBonusManager();
        objectManager->crearPersonajesManager(cantidadUsuarios);
        objectManager->crearGravedadManager();
    }

    void aceptarClientes() {

        while (serverOn) {
            int newFileDescrpt = accept(fileDescrpt, (sockaddr *) &cli_addr, &pesoCli_adrr);
            if (newFileDescrpt < 0) {
                // cerr << "ERROR --> No se pudo aceptar cliente" << endl;
                // ToDo logger
                continue;
            }


            argthread_t *arg = (argthread_t *) malloc(sizeof(argthread_t));

            if (!arg) {
                close(newFileDescrpt);  // rechazo cliente
                cerr << "ERROR --> Falta memoria para cliente" << endl;
                continue;
            }

            pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t));

            if (!thread) {
                close(newFileDescrpt); // rechazo cliente
                cerr << "ERROR --> Falta memoria para thread de cliente" << endl;
                continue;
            }

            arg->clientFD = newFileDescrpt;
            arg->thread = thread;
            arg->user = NULL;
            arg->clave = NULL;
            conectados.push_back(arg);
            if (pthread_create(thread, &attr, procesarMensajes, arg) != 0) {
                cerr << "ERROR --> Creación de thread fallida" << endl;
                conectados.pop_back();
                close(newFileDescrpt);      // Rechazar este cliente
                free(thread);
                free(arg);
            }


        }
        // ToDo delete del parser...
        logger.cierroServer();
        logger.cerrarLog();
    }
};

vector<argthread_t*> Servidor::conectados;
int Servidor::fileDescrpt;
pthread_mutex_t Servidor::mutex_login;
Log Servidor::logger(100);
ObjectManager* Servidor::objectManager = ObjectManager::getInstance();
unordered_map<string, list<Mensaje*>*> Servidor::conectadosHash;
unordered_map<string, pthread_mutex_t> Servidor::mutexesHash;
int Servidor::cantidadUsuarios;
int Servidor::modoJuego;

int main(int argc, char** argv) {

    char** xmlName;
    if (argc < 2) {
        fprintf(stderr, "Modo de Uso: %s <n° puerto> <xml(opcional)>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    unsigned short int numPuerto = (unsigned short) strtoull(argv[1], NULL, 0);

    Servidor server = Servidor(numPuerto);

    try {
        server.initJuego(argv[2]);
    } catch (XmlModoDeJuegoException& e) {
        cout << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    server.aceptarClientes();
    return 0;
}
