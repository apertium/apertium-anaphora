#ifndef _PARSEMARKABLES_
#define _PARSEMARKABLES_

using namespace std;

static void parseDoc(char *docname);
void parseCats (xmlDocPtr doc, xmlNodePtr cur);
void parseCatItem (xmlDocPtr doc, xmlNodePtr cur);  
void parseMarkables (xmlDocPtr doc, xmlNodePtr cur);
void parsePatterns (xmlDocPtr doc, xmlNodePtr cur);
void parsePatternItem (xmlDocPtr doc, xmlNodePtr cur);   

#endif