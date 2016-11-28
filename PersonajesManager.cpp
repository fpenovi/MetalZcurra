//
// Created by nestor on 27/11/16.
//

#include "ObjectManager.h"
#include "PersonajesManager.h"

using namespace chrono;

struct argpersonajes {
    bool* isKhOn;
    bool* isKhPaused;
};


void* personajesManagerFunc(void* argKh) {

    bool* isKhOn = ( (argpersonajes_t*) argKh )->isKhOn;
    bool* isKhPaused = ( (argpersonajes_t*) argKh)->isKhPaused;
    ObjectManager* objectManager = ObjectManager::getInstance();
    unordered_map<string, list<Mensaje*>*>* conectadosHash = objectManager->getConectadosHash();
    unordered_map<string, pthread_mutex_t>* mutexesHash = objectManager->getMutexesHash();
    unordered_map<int, Personaje*>* personajes = objectManager->getPersonajesHash();
    int cantUsers = objectManager->getCantidadUsuarios();

    time_point<high_resolution_clock> start;
    start = high_resolution_clock::now();
    microseconds intervalo(200000);	// 200ms

    int sprite = 0;

    while (*isKhOn) {
        time_point<high_resolution_clock> actual;
        actual = high_resolution_clock::now();

        auto deltaTiempo = actual.time_since_epoch() - start.time_since_epoch();
        auto elapsed_ms = duration_cast<microseconds>(deltaTiempo);

        if (elapsed_ms.count() >= intervalo.count()) {

            if (*isKhOn && !(*isKhPaused)) {
                int i = 0;
                int j = 0;
                int datos[cantUsers * 2];

                sprite++;
                if ( sprite >= 4 ) sprite = 0;

                for (i ; i < cantUsers ; i++){

                    Personaje* pj = (*personajes)[i+1];

                    if (pj->getQuieto()){
                        pj->setVelx(0);
                        pj->moverX();
                    }

                    datos[j++] = pj->getQuieto();
                    datos[j++] = pj->getDireccion();
                }

                ProtocoloVistaUpdate update;

                update.setTipoObjeto(8);
                update.setObject_id(sprite);

                i = 0;

                if (i < cantUsers*2) update.setEstado(datos[i]);
                else update.setEstado(0);
                i++;

                if (i < cantUsers*2) update.setX(datos[i]);
                else update.setX(0);
                i++;

                if (i < cantUsers*2) update.setY(datos[i]);
                else update.setY(0);
                i++;

                if (i < cantUsers*2) update.setPosCamara(datos[i]);
                else update.setPosCamara(0);
                i++;

                if (i < cantUsers*2) update.setConectado(datos[i]);
                else update.setConectado(0);
                i++;

                if (i < cantUsers*2) update.setSpriteIndex(datos[i]);
                else update.setSpriteIndex(0);
                i++;

                if (i < cantUsers*2) update.setApuntando(datos[i]);
                else update.setApuntando(0);
                i++;

                if (i < cantUsers*2) update.setSaltando(datos[i]);
                else update.setSaltando(0);

                int result;
                string mensaje = update.toString();

                for (auto kv : *conectadosHash) {

                    Mensaje* mensajeNuevo = new Mensaje("Server", kv.first, mensaje);

                    result = pthread_mutex_lock(&((*mutexesHash)[kv.first]));
                    if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");

                    kv.second->push_back(mensajeNuevo);

                    result = pthread_mutex_unlock(&((*mutexesHash)[kv.first]));
                    if (result != 0) perror("Fallo el pthread_mutex_lock en agregar msjs (a todos)");
                }


            }

            start = chrono::system_clock::now();
        }
    }

    return NULL;
}


PersonajesManager::PersonajesManager() {
    isOn = false;
    argPersonajes = NULL;
    personajesManagerTH = NULL;
}

PersonajesManager::~PersonajesManager() {
    delete personajesManagerTH;
    delete argPersonajes;
}

void PersonajesManager::On() {

    personajesManagerTH = new pthread_t;

    if (!personajesManagerTH)
        throw NoSePudoCrearThreadPersonajesManagerException();

    argPersonajes = new argpersonajes_t;

    if (!argPersonajes)
        throw NoSePudoCrearThreadPersonajesManagerException();

    isOn = true;
    isPaused = false;

    argPersonajes->isKhOn = &isOn;
    argPersonajes->isKhPaused = &isPaused;

    if (pthread_create(personajesManagerTH, NULL, personajesManagerFunc, argPersonajes))
        throw NoSePudoCrearThreadPersonajesManagerException();
}

void PersonajesManager::Off() {
    if (!isOn)
        return;

    isOn = false;
    pthread_join(*personajesManagerTH, NULL);
}

void PersonajesManager::Pause() {
    if (!isOn)
        return;

    isPaused = true;
}

void PersonajesManager::Resume() {
    if (!isOn)
        return;

    isPaused = false;
}