#ifndef _PARSEREF_
#define _PARSEREF_

#include <cstdio>
#include <string>
#include <cstdlib>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

using namespace std;

class ParseRef
{
public:
	void parseDoc(char *docname);
	void parseParameters (xmlDocPtr doc, xmlNodePtr cur);
	void parseParameterItem (xmlDocPtr doc, xmlNodePtr cur);

	void parseCats (xmlDocPtr doc, xmlNodePtr cur);
	void parseCatItem (xmlDocPtr doc, xmlNodePtr cur); 

	void parseMarkables (xmlDocPtr doc, xmlNodePtr cur);
	void parsePatterns (xmlDocPtr doc, xmlNodePtr cur);
	void parsePatternItem (xmlDocPtr doc, xmlNodePtr cur); 
};

#endif