#include "Log.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

Log::Log(int n ){
    maxtext = n;
    archlog.open("log.txt",ios::out	| ios::app);
}
void Log::loggearMsj(string cliente , vector<string> reciben, string texto){
    if (maxtext !=0){
        texto= texto.substr(0,maxtext);
    }
    string receptores = procesarVector(reciben);
    string horaYFecha = getHora(reciben);
    string textoLog = horaYFecha+cliente+" envio a "+receptores+": "+texto;
    archlog << textoLog<< endl;
}

void Log::loggearConexion(string cliente){
    archlog<< "se conecto "+ cliente<< endl;
}


void Log::loggearDesconexion(string cliente){
    archlog<< "se desconecto "+ cliente<< endl;
}

string Log::getHora(const vector<string> &reciben) {
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

string Log::procesarVector(vector<string> vector) {
    string texto;
    for(int i=0;i<vector.size();i++ ){
        texto+=vector[i]+" ";
    }
    return texto;
}

// main de prueba, obviamente no va , lo adjunto para que lo prueben.
int main(){
	vector<string> b(2,"kaka");
	Log log(10);
    log.loggearMsj("juancho",b,"trabajen mas pajas");
    log.cerrarLog();
    return 0;
}


