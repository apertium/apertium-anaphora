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

#ifndef _PARSEARX_
#define _PARSEARX_

#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <unordered_map>
#include <lttoolbox/xml_parse_util.h>

using namespace std;

struct item { //for cat-item and parameter-item
  vector<wstring> has_tags;
  vector<wstring> exclude_tags;
  wstring lemma;
};

typedef vector<item> acceptable_tags;

struct markable_pattern
{
	wstring name;
	int head;
};

typedef vector< vector<markable_pattern> > acceptable_patterns;

typedef unordered_map< wstring, unordered_map<wstring, acceptable_tags> > parameters_datatype;

void print_tags(vector< wstring > input);

class ParseArx
{
private:
	parameters_datatype parameters; //parameter type mapped to its parameter types, i.e. anaphor/antecedent mapped to a map which contains n="detpos" and n="verbal", etc.
	unordered_map<wstring, acceptable_tags> cats; //cat name mapped to acceptable tag lists

	unordered_map<wstring, acceptable_patterns> markables; //markable name mapped to acceptable pattern lists. Also each pattern has a head == 1
	unordered_map<wstring, int> all_markables_score; //markable name mapped to score of markable, will be applied on all anaphors
	unordered_map<wstring, unordered_map<wstring, int> > parameter_markables_score; //parameter name mapped to a mapping of markable and score (when parameter name is explicitly mentioned in arx)

public:
	int parseDoc(char *docname);
	void parseParameterTypes (xmlDocPtr doc, xmlNodePtr cur, wstring parameter_name);
	void parseParameterItem (xmlDocPtr doc, xmlNodePtr cur, wstring parameter_name, wstring parameter_type);
	void parseParameters (xmlDocPtr doc, xmlNodePtr cur);

	void parseCats (xmlDocPtr doc, xmlNodePtr cur);
	void parseCatItem (xmlDocPtr doc, xmlNodePtr cur, wstring cat_name);

	void parseMarkables (xmlDocPtr doc, xmlNodePtr cur);
	void parsePatterns (xmlDocPtr doc, xmlNodePtr cur, wstring markable_name);
	vector<markable_pattern> parsePatternItem (xmlDocPtr doc, xmlNodePtr cur);

	vector<wstring> parseTags (wstring tags);

	parameters_datatype get_parameters();
	unordered_map<wstring, acceptable_tags> get_cats();

	unordered_map<wstring, acceptable_patterns> get_markables();

	unordered_map<wstring, int> get_all_markables_score();
	unordered_map<wstring, int> get_parameter_markables_score(wstring parameter_name);
};

#endif
