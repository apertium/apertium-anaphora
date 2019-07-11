#include <cstdio>
#include <string>
#include <cstdlib>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "parse_markables.h"

//  g++ parse_markables.cc -I /opt/local/include/libxml2/ -L /usr/lib -lxml2 -lz -lpthread -lm

void parseCatItem (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"cat-item"))) 
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"tags");
	    	printf("catItem: %s\n", Attr);
		    xmlFree(Attr);
		    
		    //key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
 	    }

		cur = cur->next;
	}
    return;
}

void parseCats (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"def-cat")))
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"n");
	    	printf("catName: %s\n", Attr);
		    xmlFree(Attr);	
	    	
	    	parseCatItem(doc,cur);
	    } 
	    	
		cur = cur->next;
	}
    return;
}

void parsePatternItem (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"pattern-item"))) 
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"n");
	    	printf("patternItem: %s\n", Attr);
		    xmlFree(Attr);

		    Attr = xmlGetProp(cur, (const xmlChar *)"head");

		    if(Attr != NULL)
		    	printf("HEAD!\n");

		    xmlFree(Attr);
 	    }

		cur = cur->next;
	}
    return;
}

void parsePatterns (xmlDocPtr doc, xmlNodePtr cur) 
{
	cur = cur->xmlChildrenNode;

	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"pattern")))
	    	parsePatternItem(doc,cur);
	    	
		cur = cur->next;
	}
    return;
}

void parseMarkables (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"markable")))
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"n");
	    	printf("MarkableName: %s\n", Attr);
		    xmlFree(Attr);	
	    	
	    	parsePatterns(doc,cur);
	    } 
	    	
		cur = cur->next;
	}
    return;
}

static void parseDoc(char *docname) 
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);
	
	if (doc == NULL ) 
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) 
	{
		fprintf(stderr,"Empty Document!\n");
		xmlFreeDoc(doc);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "anaphora")) 
	{
		fprintf(stderr,"Document of the wrong type! Root node should be anaphora.\n");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) 
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"section-def-cats")))
		{
			parseCats (doc, cur);
		}

		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"section-markables")))
		{
			parseMarkables (doc, cur);
		}
		 
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int main(int argc, char **argv) 
{
	char *docname;
		
	if (argc <= 1) 
	{
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	parseDoc (docname);

	return (1);
}