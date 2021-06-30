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

#include "pattern_arx.h"
#include "parse_arx.h"

#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>
#include <lttoolbox/string_utils.h>

using namespace std;

void print_markable(acceptable_patterns inp)
{
	for (auto& i : inp) {
		cerr <<"Pattern:\n";

		for (auto& j : i) {
			cerr << j.name << endl;
		}
	}
}

bool
contains(const vector<UString>& tags, const UString& tag)
{
	return (std::find(tags.begin(), tags.end(), tag) != tags.end());
}

bool
contains_any(const vector<UString>& tags, const vector<UString>& candidates)
{
	for (auto& it : candidates) {
		if(std::find(tags.begin(), tags.end(), it) != tags.end())
			return true;
	}

	return false;
}

bool
check_acceptable_tags(const vector<UString>& input_tags, const UString& input_sl_lemma, const acceptable_tags& check_tags) //check has-tags, exclude-tags, lemma
{
	for (auto& i : check_tags) {
		bool flag_contains_all = true;

		for (auto& j : i.has_tags) {
			if(j == "*"_u) //ignore * in the tags list
				continue;

			if(!contains(input_tags, j)) {
				//if the has-tag is NOT in the input LU tags
				flag_contains_all = false;
				break;
			}
		}

		if (!flag_contains_all) continue;

		if (contains_any(input_tags, i.exclude_tags)) continue;

		if (!i.lemma.empty()) {
			if (!StringUtils::caseequal(input_sl_lemma, i.lemma)) {
				flag_contains_all = false;
				continue;
			}
		}

		if (flag_contains_all) return true;
	}

	return false;
}

parameter_return check_pattern_name(const vector<UString>& input_tags, const UString& input_sl_lemma, const unordered_map<UString, acceptable_tags>& parameter_names)
{
	parameter_return retval;
	retval.found = 0;

	for (auto& it : parameter_names) {
		if(check_acceptable_tags(input_tags, input_sl_lemma, it.second))
		{
			retval.found = 1;
			retval.parameter_name = it.first;

			return retval;
		}
	}

	return retval;
}


deque< vector<unique_LU> > add_properties(deque< vector<unique_LU> > context, ParseArx arx_file)
{
	unordered_map<UString, acceptable_patterns> arx_markables = arx_file.get_markables();
	unordered_map<UString, acceptable_tags> arx_cats = arx_file.get_cats();

	for (unordered_map<UString, acceptable_patterns>::iterator it = arx_markables.begin(); it != arx_markables.end(); it++ )
	{
		//for each markable
		UString markable_name = it->first;
		acceptable_patterns patterns_list = it->second;

		for(acceptable_patterns::iterator i = patterns_list.begin(); i!=patterns_list.end(); ++i) //go through patterns in the markable
		{
			//for each pattern
			vector<markable_pattern> current_pattern = *i;
			auto len_pattern = current_pattern.size();

			for(deque< vector<unique_LU> >::iterator m = context.begin(); m!=context.end(); ++m) //go through sentences in the queue of context
			{
				if(len_pattern > (*m).size()) //if pattern is longer then sentence length then skip
					continue;

				for (vector<unique_LU>::iterator n = (*m).begin(); n+len_pattern-1 !=(*m).end(); ++n) //go through LUs in sentence to look for the pattern with a sliding window of size = pattern length
				{
					int match_flag = 0;

					for(size_t x = 0; x < len_pattern; ++x)
					{
						acceptable_tags pattern_item_tags = arx_cats[current_pattern[x].name];

						if(check_acceptable_tags((*(n+x)).pos_tags, (*(n+x)).sl_lemma, pattern_item_tags)) //comparing current LU tags to pattern tags and lemma
						{
							match_flag = 1;

						}
						else
						{
							match_flag = 0;
							break;
						}
					}

					if(match_flag == 1)
					{
						//Add Property to the LUs

						for(size_t x = 0; x < len_pattern; ++x)
						{
							((*(n+x)).properties).push_back(markable_name);

							if(current_pattern[x].head == 1)
							{
								((*(n+x)).properties).push_back("head"_u); //
							}
						}

					}
				}
			}
		}
	}

	return context;
}
