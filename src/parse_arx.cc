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

void print_tags(vector<UString> input)
{
	for (auto& it : input) {
		cerr << it << " ";
	}
}

vector<UString> ParseArx::parseTags (UString tags)
{
	vector<UString> temp_tags_list;

	UString temptag;

	for (UString::iterator i = tags.begin(); i != tags.end(); ++i)
	{
		if(*i == '\\')
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

	if(!temptag.empty())
		temp_tags_list.push_back(temptag);

	return temp_tags_list;
}

void ParseArx::parseParameterItem (xmlDocPtr doc, xmlNodePtr cur, UString parameter_type, UString parameter_name)
//parameter_name: detpos, verbal, etc., parameter_type: anaphor, antecedent, etc.
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	item temp_item;

  while (cur != NULL)
  {
    temp_item.has_tags.clear();
    temp_item.exclude_tags.clear();
    temp_item.lemma.clear();

    if ((!xmlStrcmp(cur->name, (const xmlChar *)"parameter-item")))
    {

      Attr = xmlGetProp(cur, (const xmlChar *)"has-tags");
      if (Attr)
      {
        temp_item.has_tags = parseTags(to_ustring((const char*)Attr));
      }

      Attr = xmlGetProp(cur, (const xmlChar *)"exclude-tags");
      if (Attr)
      {
        temp_item.exclude_tags = parseTags(to_ustring((const char*)Attr));
      }

      Attr = xmlGetProp(cur, (const xmlChar *)"lemma");
      if (Attr)
      {
        temp_item.lemma = to_ustring((const char*)Attr);
      }

      parameters[parameter_type][parameter_name].push_back(temp_item);

      xmlFree(Attr);
     }

		cur = cur->next;
	}
    return;
}

void ParseArx::parseParameterTypes (xmlDocPtr doc, xmlNodePtr cur, UString parameter_name)
{
	UString parameter_type;

	cur = cur->xmlChildrenNode;

	while (cur != NULL)
	{
		if(cur->type == XML_ELEMENT_NODE)
		{
			parameter_type = to_ustring((const char*)cur->name);

      parseParameterItem(doc, cur, parameter_type, parameter_name);
    }

		cur = cur->next;
	}
    return;
}

void ParseArx::parseParameters (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *parameter_name;
	UString parameter_type;
	cur = cur->xmlChildrenNode;

	while (cur != NULL)
	{
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"def-parameter")))
    {
      parameter_name = xmlGetProp(cur, (const xmlChar *)"n");

      parseParameterTypes(doc,cur, to_ustring((const char*)parameter_name));
      xmlFree(parameter_name);
    }
    else if ((!xmlStrcmp(cur->name, (const xmlChar *)"delimiter")))
    {
      parameter_type = to_ustring((const char*)cur->name);

      parseParameterItem(doc, cur, parameter_type, "default"_u);
    }

		cur = cur->next;
	}
    return;
}

void ParseArx::parseCatItem (xmlDocPtr doc, xmlNodePtr cur, UString cat_name)
{
	xmlChar *Attr;
	cur = cur->xmlChildrenNode;

	item temp_item;

  while (cur != NULL)
  {
    temp_item.has_tags.clear();
    temp_item.exclude_tags.clear();
    temp_item.lemma.clear();

    if ((!xmlStrcmp(cur->name, (const xmlChar *)"cat-item")))
    {
      Attr = xmlGetProp(cur, (const xmlChar *)"has-tags");
      if (Attr)
      {
        temp_item.has_tags = parseTags(to_ustring((const char*)Attr));
      }

      Attr = xmlGetProp(cur, (const xmlChar *)"exclude-tags");
      if (Attr)
      {
        temp_item.exclude_tags = parseTags(to_ustring((const char*)Attr));
      }

      Attr = xmlGetProp(cur, (const xmlChar *)"lemma");
      if (Attr)
      {
        temp_item.lemma = to_ustring((const char*)Attr);
      }

      cats[cat_name].push_back(temp_item);

      xmlFree(Attr);
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

      parseCatItem(doc,cur, to_ustring((const char*)Attr));
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
      temp.name = to_ustring((const char*)Attr);

      xmlFree(Attr);

      Attr = xmlGetProp(cur, (const xmlChar *)"head");

      if(Attr != NULL)
      {
        temp.head = 1;
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

void ParseArx::parsePatterns (xmlDocPtr doc, xmlNodePtr cur, UString markable_name)
{
	xmlChar *Attr;

	cur = cur->xmlChildrenNode;

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

      UString score_ws = to_ustring((const char*)Attr);
    int score_int = stoi(score_ws);

      xmlChar *parameter_name = xmlGetProp(cur, (const xmlChar *)"parameter");

      if (parameter_name)
      {
        UString parameter_name_ws = to_ustring((const char*)parameter_name);
        parameter_markables_score[parameter_name_ws][markable_name] = score_int;
      }
      else
      {
        all_markables_score[markable_name] = score_int;
      }
    }

		cur = cur->next;
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

      parsePatterns(doc,cur, to_ustring((const char*)Attr));

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
		return -1;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL)
	{
		fprintf(stderr,"Empty Document!\n");
		xmlFreeDoc(doc);
		return 1;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "ref"))
	{
		fprintf(stderr,"Document of the wrong type! Root node should be ref.\n");
		xmlFreeDoc(doc);
		return 2;
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

parameters_datatype ParseArx::get_parameters()
{
	return parameters;
}

unordered_map<UString, acceptable_tags> ParseArx::get_cats()
{
	return cats;
}

unordered_map<UString, acceptable_patterns> ParseArx::get_markables()
{
	return markables;
}

unordered_map<UString, int> ParseArx::get_all_markables_score()
{
	return all_markables_score;
}

unordered_map<UString, int> ParseArx::get_parameter_markables_score(UString parameter_name)
{
	return parameter_markables_score[parameter_name];
}
