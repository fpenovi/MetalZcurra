//
// Created by juancho on 12/10/16.
//

#include "ParserXML.h"

ParserXML::ParserXML(char* docname) {
    doc = xmlParseFile(docname);

    if (doc == NULL ) {
        fprintf(stderr,"Document not parsed successfully. \n");
        return;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "juego")) {
        fprintf(stderr,"document of the wrong type, root node != juego ");
        xmlFreeDoc(doc);
        return;
    }
}

string ParserXML::TamVentana() {
    string tamVentana;
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
    cout<<tamVentana<<endl;
    return tamVentana;
}