/*
 * ParserXML.h
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#ifndef PARSERXML_H_
#define PARSERXML_H_

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <vector>
#include <string>

using namespace std;

class ParserXML {

private:
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlNodePtr act;
    xmlXPathObjectPtr getnodeset (xmlChar* xpath);
    xmlDocPtr docDef;

public:
    ParserXML(string path);
    void setearDefecto();
    vector<string> tamVentana();
    vector<string> xmlNiveles();
    vector<string> opcionesJuego();
    string nombreNivel();
    vector<string> tamNivel();
    vector<string> capas();
    vector<string> plataformas();
    vector<string> enemigos();
    vector<string> bonuses();
    vector<string> boss();
};

#endif /* PARSERXML_H_ */
