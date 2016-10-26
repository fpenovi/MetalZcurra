/*
 * ParserXML.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#ifndef PARSERXML_H_
#define PARSERXML_H_

#include <vector>
#include <string>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

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
	virtual ~ParserXML();
};

#endif /* PARSERXML_H_ */
