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
#include <iostream>
using namespace std;

class ParserXML {
private:
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlNodePtr act;
public:
    ParserXML(char*);
    string TamVentana();
};


#endif //METALZCURRA_PARSERXML_H
