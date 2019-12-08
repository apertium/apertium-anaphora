/*
* Copyright (C) 2019 Apertium Project Management Committee <apertium-pmc@dlsi.ua.es>,
*               2019 Tanmai Khanna <khanna.tanmai@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "parse_arx.h"

#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <unordered_map>
#include <iostream>
#include <lttoolbox/xml_parse_util.h>

void print_tags(vector<wstring> input) //testing function
{
	for (size_t i = 0; i < input.size(); ++i)
	{
		wcerr << input[i];
		wcerr << " ";
	}
}

vector<wstring> ParseArx::parseTags (wstring tags)
{
	vector<wstring> temp_tags_list;

	wstring temptag;

	for (std::wstring::iterator i = tags.begin(); i != tags.end(); ++i)
	{
		if(*i == '\\') //dealing with escaped characters
		{
			temptag.push_back(*i);
			++i;
			temptag.push_back(*i);
		}
		else if(*i == ' ') //space separated tags
		{
			temp_tags_list.push_back(temptag);
			temptag.clear();
		}
		else
		{
			temptag.push_back(*i);
		}
	}

	if(!temptag.empty()) //if any tag remaining
		temp_tags_list.push_back(temptag);

	//print_tags(temp_tags_list);

	//cerr << "\n";

	return temp_tags_list;
}

void ParseArx::parseParameterItem (xmlDocPtr doc, xmlNodePtr cur, wstring parameter_name)
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	vector <wstring> temp_tags_list;

	while (cur != NULL)
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"parameter-item")))
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"has-tags");

	    	//fprintf(stderr, "ParameterItem: ");

	    	temp_tags_list = parseTags(XMLParseUtil::towstring(Attr));
	    	parameters[parameter_name].push_back(temp_tags_list);

	    	temp_tags_list.clear();

		    xmlFree(Attr);

		    //key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
 	    }

		cur = cur->next;
	}
    return;
}

void ParseArx::parseParameters (xmlDocPtr doc, xmlNodePtr cur)
{
	wstring parameter_name;

	cur = cur->xmlChildrenNode;

	while (cur != NULL)
	{
		if(cur->type == XML_ELEMENT_NODE)
		{
			parameter_name = XMLParseUtil::towstring(cur->name);

			//cerr << "\n";
	    	//wcerr << parameter_name;
	    	//cerr << "\n";

	    	parseParameterItem(doc,cur,parameter_name);
	    }

		cur = cur->next;
	}
    return;
}

void ParseArx::parseCatItem (xmlDocPtr doc, xmlNodePtr cur, wstring cat_name)
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	vector <wstring> temp_tags_list;

	while (cur != NULL)
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"cat-item")))
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"has-tags");
	    	//fprintf(stderr, "catItem: ");

	    	temp_tags_list = parseTags(XMLParseUtil::towstring(Attr));
		    cats[cat_name].push_back(temp_tags_list);

		    temp_tags_list.clear();

		    xmlFree(Attr);

		    //key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
 	    }

		cur = cur->next;
	}
    return;
}

void ParseArx::parseCats (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL)
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"def-cat")))
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"n");
	    	//fprintf(stderr, "catName: %s\n", Attr);

	    	parseCatItem(doc,cur, XMLParseUtil::towstring(Attr));
	    	xmlFree(Attr);
	    }

		cur = cur->next;
	}
    return;
}

vector<markable_pattern> ParseArx::parsePatternItem (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	vector<markable_pattern> temp_pattern;

	while (cur != NULL)
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"pattern-item")))
	    {
	    	markable_pattern temp;

	    	Attr = xmlGetProp(cur, (const xmlChar *)"n");
	    	temp.name = XMLParseUtil::towstring(Attr);

	    	//wcerr << temp.name;
	    	//cerr << " ";

		    xmlFree(Attr);

		    Attr = xmlGetProp(cur, (const xmlChar *)"head");

		    if(Attr != NULL)
		    {
		    	temp.head = 1;
		    	//fprintf(stderr, "[HEAD!]");
		    }
		    else
		    	temp.head = 0;

		    xmlFree(Attr);

		    temp_pattern.push_back(temp);
 	    }

		cur = cur->next;
	}

    return temp_pattern;
}

void ParseArx::parsePatterns (xmlDocPtr doc, xmlNodePtr cur, wstring markable_name)
{
	xmlChar *Attr;

	cur = cur->xmlChildrenNode;

	//wcerr << markable_name;
	//cerr << "\n";

	while (cur != NULL)
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"pattern")))
	    {
	    	vector<markable_pattern> temp_pattern = parsePatternItem(doc,cur);

	    	markables[markable_name].push_back(temp_pattern);
	    }

	    else if ((!xmlStrcmp(cur->name, (const xmlChar *)"score")))
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"n");
	    	wstring score_ws = XMLParseUtil::towstring(Attr);

	    	int score_int = std::stoi(score_ws);

	    	markables_score[markable_name] = score_int;
	    }

		cur = cur->next;

		//cerr << "\n";
	}
    return;
}

void ParseArx::parseMarkables (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	while (cur != NULL)
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"markable")))
	    {
	    	Attr = xmlGetProp(cur, (const xmlChar *)"n");
	    	//fprintf(stderr, "MarkableName: ");

	    	parsePatterns(doc,cur, XMLParseUtil::towstring(Attr));

	    	xmlFree(Attr);
	    }

		cur = cur->next;
	}
    return;
}

int ParseArx::parseDoc(char *docname)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL )
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return -1; //return error
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL)
	{
		fprintf(stderr,"Empty Document!\n");
		xmlFreeDoc(doc);
		return 1; //return error
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "ref"))
	{
		fprintf(stderr,"Document of the wrong type! Root node should be ref.\n");
		xmlFreeDoc(doc);
		return 2; //return error
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"section-parameters")))
		{
			parseParameters (doc, cur);
		}

		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"section-def-cats")))
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
	return 0;
}

unordered_map<wstring, acceptable_tags> ParseArx::get_parameters()
{
	return parameters;
}

unordered_map<wstring, acceptable_tags> ParseArx::get_cats()
{
	return cats;
}

unordered_map<wstring, acceptable_patterns> ParseArx::get_markables()
{
	return markables;
}

unordered_map<wstring, int> ParseArx::get_markables_score()
{
	return markables_score;
}

/* //Code for Testing
int main(int argc, char **argv)
{
	char *docname;

	if (argc <= 1)
	{
		fprintf(stderr, "Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];

	ParseArx ref;

	ref.parseDoc(docname);

	return (1);
}
*/