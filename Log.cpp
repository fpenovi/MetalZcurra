#include "Log.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

Log::Log(int n ){
    maxtext = n;
    archlog.open("log.txt",ios::out	| ios::app);
}
void Log::loggearMsj(string cliente , string receptor, string texto){
    if (maxtext !=0){
        texto= texto.substr(0,maxtext);
        texto.erase(texto.length()-1);
    }
    string horaYFecha = getHora();
    string textoLog = horaYFecha+cliente+" envio a "+receptor+": "+texto;
    archlog << textoLog<<endl;
}

void Log::loggearConexion(string cliente){
    archlog<<getHora()+ "se conecto "+ cliente<< endl;
}

void Log::inicializoServer() {
    archlog<<getHora()+ "se inicializo el servidor" << endl;

}

void Log::cierroServer() {
    archlog<<getHora()+"se cerro el servidor" << endl;

}

void Log::loggearDesconexionViolenta(string cliente){
    archlog<<getHora()+ "se perdio conexion con "+ cliente<< endl;
}

void Log::loggearDesconexionNormal(string cliente){
    archlog<< getHora()+"se desconecto: "+ cliente<< endl;
}

void Log::error(string texto){
    archlog<< getHora()+"error: "+texto<<endl;

}
string Log::getHora() {
    time_t tiempo= time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S ",tlocal);
    string horaYFecha(output);
    return horaYFecha;
}

void Log::cerrarLog() {
    archlog.close();
}

void Log::loggearRecepcion(string cliente){
    archlog<<getHora()+cliente+" recibio todos sus mensajes"<<endl;
}

