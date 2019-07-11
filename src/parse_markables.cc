#include <cstdio>
#include <string>
#include <cstdlib>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

//  g++ parse_markables.cc -I /opt/local/include/libxml2/ -L /usr/lib -lxml2 -lz -lpthread -lm

void
parseCatItem (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *tagsAttr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"cat-item"))) 
	    {
	    	tagsAttr = xmlGetProp(cur, (const xmlChar *)"tags");
	    	printf("catItem: %s\n", tagsAttr);
		    xmlFree(tagsAttr);
		    
		    //key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
 	    }

		cur = cur->next;
	}
    return;
}

void
parseCats (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *nameAttr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"def-cat")))
	    {
	    	nameAttr = xmlGetProp(cur, (const xmlChar *)"n");
	    	printf("catName: %s\n", nameAttr);
		    xmlFree(nameAttr);	
	    	
	    	parseCatItem(doc,cur);
	    } 
	    	
		cur = cur->next;
	}
    return;
}

static void
parseDoc(char *docname) 
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
		 
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int
main(int argc, char **argv) 
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