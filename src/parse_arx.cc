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
#include <iostream>
#include <lttoolbox/string_utils.h>
#include <lttoolbox/xml_walk_util.h>
#include <lttoolbox/i18n.h>

void print_tags(const vector<UString>& input)
{
	for (auto& it : input) {
		cerr << it << " ";
	}
}

vector<UString> ParseArx::parseTags (const UString& tags)
{
	vector<UString> temp_tags_list;

	UString temptag;

	for (UString::const_iterator i = tags.begin(); i != tags.end(); ++i)
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

void ParseArx::parseParameterItem (xmlNodePtr cur, UString parameter_type, UString parameter_name)
//parameter_name: detpos, verbal, etc., parameter_type: anaphor, antecedent, etc.
{
	for (auto pi : children(cur)) {
		if ((!xmlStrcmp(pi->name, (const xmlChar *)"parameter-item"))) {
			item temp_item;
			temp_item.has_tags = parseTags(getattr(pi, "has-tags"));
			temp_item.exclude_tags = parseTags(getattr(pi, "exclude-tags"));
			temp_item.lemma = getattr(pi, "lemma");
			parameters[parameter_type][parameter_name].push_back(temp_item);
		}
	}
}

void ParseArx::parseParameterTypes (xmlNodePtr cur, UString parameter_name)
{
	for (auto param : children(cur)) {
		parseParameterItem(param, to_ustring((const char*) param->name),
						   parameter_name);
	}
}

void ParseArx::parseParameters (xmlNodePtr cur)
{
	for (auto param : children(cur)) {
		if (!xmlStrcmp(param->name, (const xmlChar*)"def-parameter")) {
			parseParameterTypes(param, getattr(param, "n"));
		} else if (!xmlStrcmp(param->name, (const xmlChar*)"delimiter")) {
			parseParameterItem(param, "delimiter"_u, "default"_u);
		}
	}
}

void ParseArx::parseCatItem (xmlNodePtr cur, UString cat_name)
{
	for (auto ci : children(cur)) {
		if (!xmlStrcmp(ci->name, (const xmlChar*)"cat-item")) {
			item temp_item;
			temp_item.has_tags = parseTags(getattr(ci, "has-tags"));
			temp_item.exclude_tags = parseTags(getattr(ci, "exclude-tags"));
			temp_item.lemma = getattr(ci, "lemma");
			cats[cat_name].push_back(temp_item);
		}
	}
}

void ParseArx::parseCats (xmlNodePtr cur)
{
	for (auto cat : children(cur)) {
		if (!xmlStrcmp(cat->name, (const xmlChar*)"def-cat")) {
			parseCatItem(cat, getattr(cat, "n"));
		}
	}
}

vector<markable_pattern> ParseArx::parsePatternItem (xmlNodePtr cur)
{
	xmlChar *Attr;

	vector<markable_pattern> temp_pattern;

	for (auto pi : children(cur)) {
		if ((!xmlStrcmp(pi->name, (const xmlChar *)"pattern-item"))) {
			markable_pattern temp;

			Attr = xmlGetProp(pi, (const xmlChar *)"n");
			temp.name = to_ustring((const char*)Attr);

			xmlFree(Attr);

			Attr = xmlGetProp(pi, (const xmlChar *)"head");

			if(Attr != NULL) {
				temp.head = 1;
			} else {
				temp.head = 0;
			}

			xmlFree(Attr);

			temp_pattern.push_back(temp);
		}
	}
    return temp_pattern;
}

void ParseArx::parsePatterns (xmlNodePtr cur, UString markable_name)
{
	for (auto pat : children(cur)) {
		if ((!xmlStrcmp(pat->name, (const xmlChar *)"pattern"))) {
			vector<markable_pattern> temp_pattern = parsePatternItem(pat);

			markables[markable_name].push_back(temp_pattern);
		} else if ((!xmlStrcmp(pat->name, (const xmlChar *)"score"))) {
			int score_int = StringUtils::stoi(getattr(pat, "n"));

			xmlChar *param_name = xmlGetProp(cur, (const xmlChar*)"parameter");

			if (param_name) {
				UString name = to_ustring((const char*)param_name);
				parameter_markables_score[name][markable_name] = score_int;
			} else {
				all_markables_score[markable_name] = score_int;
			}
		}
	}
}

void ParseArx::parseMarkables (xmlNodePtr cur)
{
	for (auto m : children(cur)) {
		if ((!xmlStrcmp(m->name, (const xmlChar *)"markable"))) {
			parsePatterns(m, getattr(m, "n"));
		}
	}
}

int ParseArx::parseDoc(char *docname)
{
	xmlNodePtr cur;

	curDoc = xmlParseFile(docname);

	if (curDoc == nullptr )
	{
		I18n(ANA_I18N_DATA, "ana").error("ANA80010", {}, {}, false);
		return -1;
	}

	cur = xmlDocGetRootElement(curDoc);

	if (cur == NULL)
	{
		I18n(ANA_I18N_DATA, "ana").error("ANA80020", {}, {}, false);
		xmlFreeDoc(curDoc);
		return 1;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "ref"))
	{
		I18n(ANA_I18N_DATA, "ana").error("ANA80030", {}, {}, false);
		xmlFreeDoc(curDoc);
		return 2;
	}

	for (auto ch : children(cur)) {
		if ((!xmlStrcmp(ch->name, (const xmlChar*)"section-parameters"))) {
			parseParameters(ch);
		} else if ((!xmlStrcmp(ch->name, (const xmlChar*)"section-def-cats"))) {
			parseCats(ch);
		} else if ((!xmlStrcmp(ch->name, (const xmlChar*)"section-markables"))) {
			parseMarkables(ch);
		}
	}

	xmlFreeDoc(curDoc);
	curDoc = nullptr;
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
