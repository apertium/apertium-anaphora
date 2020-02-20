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
	unsigned int id;
	wstring wordform;
	wstring tl_wordform;
	vector<wstring> pos_tags;
	vector<wstring> properties;
};

struct antecedent
{
	unique_LU LU;
	int score;
};

struct parameter_return
{
	int found;
	wstring parameter_name;
};

int contains(vector<wstring> tags, wstring tag);
int contains_any(vector<wstring> tags, vector<wstring> candidates);

int check_acceptable_tags(vector<wstring> input_tags, acceptable_tags check_tags);
parameter_return check_pattern_name(vector<wstring> input_tags, unordered_map<wstring, acceptable_tags> parameter_names);

deque< vector<unique_LU> > add_properties(deque< vector<unique_LU> > context, ParseArx arx_file);

#endif