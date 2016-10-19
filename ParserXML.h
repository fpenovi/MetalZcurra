//
// Created by juancho on 12/10/16.
//

#ifndef METALZCURRA_PARSERXML_H
#define METALZCURRA_PARSERXML_H
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <iostream>
#include <vector>
using namespace std;

class ParserXML {
private:
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlNodePtr act;
    xmlXPathObjectPtr getnodeset (xmlChar *xpath);
    xmlDocPtr docDef;
public:
    ParserXML(char*);
    string TamVentana();
    string tamNivel();
    vector<string> spritesPlayers();
    vector<string> capas();
    void setearDefecto();
    vector<string> users();
};


#endif //METALZCURRA_PARSERXML_H
