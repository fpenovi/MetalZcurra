/*
 * ParserXML.cpp
 *
 *  Created on: Oct 24, 2016
 *      Author: franch
 */

#include "ParserXML.h"
#include <strings.h>
#include <unistd.h>

ParserXML::ParserXML(char* docname) {

    doc = xmlParseFile(docname);
    docDef = xmlParseFile("resources/juego.xml");
    if (doc == NULL ) {
        fprintf(stderr,"Document not parsed successfully. \n");
        setearDefecto();
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        setearDefecto();
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "juego")) {
        fprintf(stderr,"document of the wrong type, root node != juego ");
        xmlFreeDoc(doc);
        setearDefecto();
    }
}

string ParserXML::TamVentana() {
    cur = xmlDocGetRootElement(doc);
    string tamVentana="",fallo="";
    cur=cur->xmlChildrenNode;
    while(cur != NULL){
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"ventana"))) {
            cur = cur->xmlChildrenNode;
            while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *) "ancho"))) {
                    act = cur;
                    tamVentana += string((char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
                    tamVentana += "$";

                }
                if ((!xmlStrcmp(cur->name, (const xmlChar *) "alto"))) {
                    tamVentana += string((char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
                    tamVentana += "$";

                }
                cur = cur->next;
            }
        }
        if (cur != NULL) cur= cur->next;
    }
    return tamVentana;
}

string ParserXML::tamNivel(){
    this->cur =xmlDocGetRootElement(doc);
    string tamNivel="",fallo="";
    cur=cur->xmlChildrenNode;
    while(cur != NULL){
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"nivel"))) {
            cur = cur->xmlChildrenNode;
            while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *) "ancho"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    tamNivel += string((char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
                    tamNivel += "$";

                }
                if ((!xmlStrcmp(cur->name, (const xmlChar *) "alto"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    tamNivel += string((char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
                    tamNivel += "$";

                }
                cur = cur->next;
            }
        }
        if (cur != NULL) cur= cur->next;
    }
    return tamNivel;

}

vector<string> ParserXML::spritesPlayers(){
    int i;
    xmlNodeSetPtr nodeset;
    xmlXPathObjectPtr result;
    vector<string> sprites, fallo;
    cur = xmlDocGetRootElement(doc);
    result = this->getnodeset ((xmlChar*)"//sprite");
    string datosSprite;
    if (result) {
        nodeset = result->nodesetval;
        for (i = 0; i < nodeset->nodeNr; i++) {
            datosSprite="";
            act = nodeset->nodeTab[i]->xmlChildrenNode;
            while (act != NULL) {
                if ((!xmlStrcmp(act->name, (const xmlChar *) "id"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosSprite += string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosSprite += "$";
                }
                if ((!xmlStrcmp(act->name, (const xmlChar *) "path"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosSprite +=  string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosSprite += "$";
                }
                if ((!xmlStrcmp(act->name, (const xmlChar *) "ancho"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosSprite +=  string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosSprite += "$";
                }
                if ((!xmlStrcmp(act->name, (const xmlChar *) "alto"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosSprite +=  string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosSprite += "$";
                }
                act = act->next;
            }
            sprites.push_back(datosSprite);

        }
        xmlXPathFreeObject(result);
    }

    return sprites;
}

vector<string> ParserXML::capas(){
    int i;
    xmlNodeSetPtr nodeset;
    xmlXPathObjectPtr result;
    vector<string> capas,fallo;
    cur = xmlDocGetRootElement(doc);
    result = this->getnodeset ((xmlChar*)"//capa");
    string datosCapa;
    if (result) {
        nodeset = result->nodesetval;
        for (i = 0; i < nodeset->nodeNr; i++) {
            datosCapa="";
            act = nodeset->nodeTab[i]->xmlChildrenNode;
            while (act != NULL) {
                if ((!xmlStrcmp(act->name, (const xmlChar *) "path"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosCapa +=  string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosCapa += "$";
                }
                if ((!xmlStrcmp(act->name, (const xmlChar *) "ancho"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosCapa +=  string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosCapa += "$";
                }
                if ((!xmlStrcmp(act->name, (const xmlChar *) "zindex"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosCapa +=  string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosCapa += "$";
                }
                act = act->next;
            }
            capas.push_back(datosCapa);

        }
        xmlXPathFreeObject(result);
    }

    return capas;
}

void ParserXML::setearDefecto() {
    doc = docDef;
}

vector<string> ParserXML::users() {
    int i;
    xmlNodeSetPtr nodeset;
    xmlXPathObjectPtr result;
    vector<string> fallo;
    vector<string> users;
    cur = xmlDocGetRootElement(doc);
    result = this->getnodeset ((xmlChar*)"//player");
    string datosUser;
    if (result) {
        nodeset = result->nodesetval;
        for (i = 0; i < nodeset->nodeNr; i++) {
            datosUser="";
            act = nodeset->nodeTab[i]->xmlChildrenNode;
            while (act != NULL) {
                if ((!xmlStrcmp(act->name, (const xmlChar *) "user"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosUser += string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosUser += "$";
                }
                if ((!xmlStrcmp(act->name, (const xmlChar *) "pass"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    datosUser +=  string((char*)xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    datosUser += "$";
                }

                act = act->next;
            }
            users.push_back(datosUser);

        }
        xmlXPathFreeObject(result);
    }
    return users;
}
xmlXPathObjectPtr ParserXML::getnodeset (xmlChar *xpath){

    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext(doc);
    if (context == NULL) {
        printf("Error in xmlXPathNewContext\n");
        return NULL;
    }
    result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);
    if (result == NULL) {
        printf("Error in xmlXPathEvalExpression\n");
        return NULL;
    }
    if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
        xmlXPathFreeObject(result);
        printf("No result\n");
        return NULL;
    }
    return result;
}

ParserXML::~ParserXML() {
	// TODO Auto-generated destructor stub
}
