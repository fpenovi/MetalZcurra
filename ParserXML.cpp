/*
 * ParserXML.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: franch
 */

#include "ParserXML.h"


ParserXML::ParserXML(string docname) {

    doc = xmlParseFile(docname.c_str());
    docDef = xmlParseFile("juego.xml");
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
}

vector<string> ParserXML::tamVentana() {
    cur = xmlDocGetRootElement(doc);
    vector<string> tamVentana;
    cur=cur->xmlChildrenNode;

    while(cur != NULL){
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"ventana"))) {
            cur = cur->xmlChildrenNode;
            while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *) "ancho"))) {
                    act = cur;
                    string ancho = string((char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
                    tamVentana.push_back(ancho);
                }

                if ((!xmlStrcmp(cur->name, (const xmlChar *) "alto"))) {
                    string alto = string((char*)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
                    tamVentana.push_back(alto);

                }
                cur = cur->next;
            }
        }
        if (cur != NULL) cur= cur->next;
    }
    return tamVentana;
}

vector<string> ParserXML::xmlNiveles() {
	cur = xmlDocGetRootElement(doc);
	vector<string> xmlNiveles;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "niveles"))) {
			cur = cur->xmlChildrenNode;

			while (cur != NULL) {
				if ((!xmlStrcmp(cur->name, (const xmlChar *) "nivel"))) {
					act = cur;
					string xmlPathNivel = string((char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
					xmlNiveles.push_back(xmlPathNivel);
				}
				cur = cur->next;
			}
		}
		if (cur != NULL) cur = cur->next;
	}

	return xmlNiveles;
}

vector<string> ParserXML::opcionesJuego() {
	cur = xmlDocGetRootElement(doc);
	vector<string> opcionesJuego;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "opciones"))) {
			cur = cur->xmlChildrenNode;

			while (cur != NULL) {
				if ((!xmlStrcmp(cur->name, (const xmlChar *) "modo"))) {
					act = cur;
					string modoJuego = string((char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
					opcionesJuego.push_back(modoJuego);
				}

				if ((!xmlStrcmp(cur->name, (const xmlChar *) "jugadores"))) {
					act = cur;
					string cantJugadores = string((char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
					opcionesJuego.push_back(cantJugadores);
				}
				cur = cur->next;
			}
		}
		if (cur != NULL) cur = cur->next;
	}

	return opcionesJuego;
}

string ParserXML::nombreNivel() {
	this->cur = xmlDocGetRootElement(doc);

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "nivel"))) {
			cur = cur->xmlChildrenNode;

			while (cur != NULL) {
				if ((!xmlStrcmp(cur->name, (const xmlChar *) "nombre")))
					return string((char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));

				cur = cur->next;
			}
		}
		if (cur != NULL) cur= cur->next;
	}
	return "";
}

vector<string> ParserXML::tamNivel(){
	cur = xmlDocGetRootElement(doc);
	vector<string> opcionesJuego;

	while (cur != NULL) {
			if ((!xmlStrcmp(cur->name, (const xmlChar *) "nivel"))) {
				cur = cur->xmlChildrenNode;

				while (cur != NULL) {
					if ((!xmlStrcmp(cur->name, (const xmlChar *) "ancho"))) {
						act = cur;
						string ancho = string((char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
						opcionesJuego.push_back(ancho);
					}

					if ((!xmlStrcmp(cur->name, (const xmlChar *) "alto"))) {
						act = cur;
						string alto = string((char*) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
						opcionesJuego.push_back(alto);
					}
					cur = cur->next;
				}
			}
			if (cur != NULL) cur = cur->next;
		}

		return opcionesJuego;
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
                    capas.push_back(datosCapa);
                }
                act = act->next;
            }
        }
        xmlXPathFreeObject(result);
    }

    return capas;
}

vector<string> ParserXML::plataformas() {
    int i;
    xmlNodeSetPtr nodeset;
    xmlXPathObjectPtr result;
    vector<string> fallo;
    vector<string> plataformas;
    cur = xmlDocGetRootElement(doc);
    result = this->getnodeset ((xmlChar*)"//plataforma");

    if (result) {
    	nodeset = result->nodesetval;

    	for (i = 0; i<nodeset->nodeNr; i++) {
    		act = nodeset->nodeTab[i]->xmlChildrenNode;

    		while (act != NULL) {

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "x"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    string x = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    plataformas.push_back(x);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "y"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string y = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				plataformas.push_back(y);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "ancho"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string ancho = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				plataformas.push_back(ancho);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "alto"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string alto = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				plataformas.push_back(alto);
    			}
    			act = act->next;
    		}
    	}
    	xmlXPathFreeObject(result);
    }
    return plataformas;
}

vector<string> ParserXML::bonuses() {
    int i;
    xmlNodeSetPtr nodeset;
    xmlXPathObjectPtr result;
    vector<string> fallo;
    vector<string> bonuses;
    cur = xmlDocGetRootElement(doc);
    result = this->getnodeset ((xmlChar*)"//bonus");

    if (result) {
    	nodeset = result->nodesetval;

    	for (i = 0; i<nodeset->nodeNr; i++) {
    		act = nodeset->nodeTab[i]->xmlChildrenNode;

    		while (act != NULL) {

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "x"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    string x = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    bonuses.push_back(x);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "y"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string y = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				bonuses.push_back(y);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "tipo"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string tipo = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				bonuses.push_back(tipo);
    			}

    			act = act->next;
    		}
    	}
    	xmlXPathFreeObject(result);
    }
    return bonuses;
}

vector<string> ParserXML::enemigos() {
    int i;
    xmlNodeSetPtr nodeset;
    xmlXPathObjectPtr result;
    vector<string> fallo;
    vector<string> enemigos;
    cur = xmlDocGetRootElement(doc);
    result = this->getnodeset ((xmlChar*)"//enemigo");

    if (result) {
    	nodeset = result->nodesetval;

    	for (i = 0; i<nodeset->nodeNr; i++) {
    		act = nodeset->nodeTab[i]->xmlChildrenNode;

    		while (act != NULL) {

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "x"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    string x = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    enemigos.push_back(x);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "y"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string y = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				enemigos.push_back(y);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "delta"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string delta = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				enemigos.push_back(delta);
    			}

    			act = act->next;
    		}
    	}
    	xmlXPathFreeObject(result);
    }
    return enemigos;
}

vector<string> ParserXML::boss() {
    int i;
    xmlNodeSetPtr nodeset;
    xmlXPathObjectPtr result;
    vector<string> fallo;
    vector<string> boss;
    cur = xmlDocGetRootElement(doc);
    result = this->getnodeset ((xmlChar*)"//boss");

    if (result) {
    	nodeset = result->nodesetval;

    	for (i = 0; i<nodeset->nodeNr; i++) {
    		act = nodeset->nodeTab[i]->xmlChildrenNode;

    		while (act != NULL) {

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "x"))) {
                    if (! act->xmlChildrenNode ) return fallo;
                    string x = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
                    boss.push_back(x);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "y"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string y = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				boss.push_back(y);
    			}

    			if ((!xmlStrcmp(act->name, (const xmlChar *) "delta"))) {
    				if (! act->xmlChildrenNode ) return fallo;
    				string delta = string((char*) xmlNodeListGetString(doc, act->xmlChildrenNode, 1));
    				boss.push_back(delta);
    			}

    			act = act->next;
    		}
    	}
    	xmlXPathFreeObject(result);
    }
    return boss;
}

void ParserXML::setearDefecto() {
    doc = docDef;
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
