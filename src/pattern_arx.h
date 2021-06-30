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

#ifndef _PATTERNARX_
#define _PATTERNARX_

#include "parse_arx.h"

#include <string>
#include <vector>
#include <deque>

using namespace std;

struct unique_LU
{
	int id;
	UString wordform;
	UString tl_wordform;
	UString sl_lemma;
	UString tl_lemma;
	vector<UString> pos_tags;
	vector<UString> properties;
};

struct antecedent
{
	unique_LU LU;
	int score;
};

struct parameter_return
{
	int found;
	UString parameter_name;
};

bool contains(const vector<UString>& tags, const UString& tag);
bool contains_any(const vector<UString>& tags, const vector<UString>& candidates);

bool check_acceptable_tags(const vector<UString>& input_tags, const UString& input_sl_lemma, const acceptable_tags& check_tags);
parameter_return check_pattern_name(const vector<UString>& input_tags, const UString& input_sl_lemma, const unordered_map<UString, acceptable_tags>& parameter_names);

deque< vector<unique_LU> > add_properties(deque< vector<unique_LU> > context, ParseArx arx_file);

#endif
