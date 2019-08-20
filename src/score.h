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

#ifndef _MITKOVSCORE_
#define _MITKOVSCORE_

#include "parse_arx.h"
#include "pattern_arx.h"

#include <string>
#include <vector>
#include <queue>
#include <deque>

using namespace std;

void showq(deque < vector<unique_LU> > gq);

class Scoring
{
private:
	deque< vector<unique_LU> > context; //A queue of sentences. Each sentence is a vector of Lexical Units
	vector<antecedent> antecedent_list; //A list of antecedents

public:
	int add_word(unsigned int input_id, wstring input_wordform, vector< wstring > pos_tags, wstring input_tl_wordform, ParseArx arx_file);
	void apply_indicators(unique_LU anaphor, ParseArx arx_file);
	int check_agreement(vector<wstring> antecedent_tags, vector<wstring> anaphor_tags);
	wstring get_antecedent(int debug_flag);
	void clear();
};


#endif