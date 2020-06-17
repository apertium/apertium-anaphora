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
#include <cwctype>

using namespace std;

void print_markable(acceptable_patterns inp) //testing function
{
	for(acceptable_patterns::iterator i = inp.begin(); i != inp.end(); i++)
	{
		cerr <<"Pattern:\n";

		for(vector<markable_pattern>::iterator j = (*i).begin(); j != (*i).end(); j++)
		{
			wcerr << (*j).name;
			cerr << "\n";
		}
	}
}

int contains(vector<wstring> tags, wstring tag)
{
	if(std::find(tags.begin(), tags.end(), tag) != tags.end())
		return 1;
	else
		return 0;
}

int contains_any(vector<wstring> tags, vector<wstring> candidates)
{
	for(vector<wstring>::iterator it=candidates.begin();it!=candidates.end();++it)
	{
		if(std::find(tags.begin(), tags.end(), *it) != tags.end())
			return 1; //if any of the tags in candidates matches the tags list
	}

	return 0; //if no matches
}

void toLower(basic_string<wchar_t>& s)
{
   for (basic_string<wchar_t>::iterator p = s.begin(); p != s.end(); ++p)
   {
      *p = towlower(*p);
   }
}

int check_acceptable_tags(vector<wstring> input_tags, wstring input_sl_lemma, acceptable_tags check_tags) //check has-tags, exclude-tags, lemma
{
	for (acceptable_tags::iterator i = check_tags.begin(); i != check_tags.end(); ++i)
	{

		int flag_contains_all = 1;

    vector<wstring> temp_tags = i->has_tags;
    vector<wstring> temp_exclude_tags = i->exclude_tags;
    
		for(std::vector<wstring>::iterator j = temp_tags.begin(); j != temp_tags.end(); ++j) //check for the tags in has-tags
		{
			if(*j == L"*") //ignore * in the tags list
				continue;

			if(!contains(input_tags, *j)) //if the has-tag is NOT in the input LU tags
			{
				flag_contains_all = 0;
				break;
			}
		}
    
    if(flag_contains_all == 0)
    {
      continue;
    }
    
    for(std::vector<wstring>::iterator j = temp_exclude_tags.begin(); j != temp_exclude_tags.end(); ++j) //check for the tags in exclude-tags
    {
      if(contains(input_tags, *j)) //if the exclude-tag IS in the input LU tags
      {
        flag_contains_all = 0;
        break;
      }
    }
    
    if(flag_contains_all == 0)
    {
      continue;
    }
    
    if(!(i->lemma).empty())
    {
      wstring temp_lemma = i->lemma;
      
      if(input_sl_lemma.length() == temp_lemma.length())
      {
        if(input_sl_lemma.compare(temp_lemma) != 0)
        {
          toLower(input_sl_lemma);
          toLower(temp_lemma);
          
          if(input_sl_lemma.compare(temp_lemma) != 0)
          {
            flag_contains_all = 0;
          }
        }
      }
      else
      {
        flag_contains_all = 0;
      }
    }
    
    if(flag_contains_all == 0)
    {
      continue;
    }
    else //if any tag list fully matched (i.e. has-tags present, exclude-tags absent)
    {
      return 1;
    }
	}

	return 0; //if it didn't return 1 then no tag list was fully matched
}

parameter_return check_pattern_name(vector<wstring> input_tags, wstring input_sl_lemma, unordered_map<wstring, acceptable_tags> parameter_names)
//find out if any of the anaphors match wrt tags, and if yes, return the unique name
{
	parameter_return retval;
	retval.found = 0;

	for (unordered_map<wstring, acceptable_tags>::iterator it = parameter_names.begin(); it != parameter_names.end(); it++)
	{
		wstring parameter_name = it->first;
		acceptable_tags parameter_tags= it->second;

		if(check_acceptable_tags(input_tags, input_sl_lemma, parameter_tags))
		{
			retval.found = 1;
			retval.parameter_name = parameter_name;

			return retval;
		}
	}
	
	return retval;
}


deque< vector<unique_LU> > add_properties(deque< vector<unique_LU> > context, ParseArx arx_file)
{
	unordered_map<wstring, acceptable_patterns> arx_markables = arx_file.get_markables();
	unordered_map<wstring, acceptable_tags> arx_cats = arx_file.get_cats();

	for (unordered_map<wstring, acceptable_patterns>::iterator it = arx_markables.begin(); it != arx_markables.end(); it++ ) //go through markables defined in xml file
	{
		//for each markable
		wstring markable_name = it->first;
		acceptable_patterns patterns_list = it->second;

		//print_markable(patterns_list);

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
						//this is the window -- check if pattern matches

						acceptable_tags pattern_item_tags = arx_cats[current_pattern[x].name]; //get pattern item tags from def-cats

						if(check_acceptable_tags((*(n+x)).pos_tags, (*(n+x)).sl_lemma, pattern_item_tags)) //comparing current LU tags to pattern tags and lemma
						{
							match_flag = 1;

						}
						else
						{
							match_flag = 0;
							break;
						}

						//wcerr << (*(n+x)).wordform;
					}

					if(match_flag == 1) //if the entire pattern matched
					{
						//Add Property to the LUs
						/*
						cerr << "\n";
						wcerr << markable_name;
						cerr << " Pattern Matched at: ";
						wcerr << (*n).wordform;
						cerr << "\n";
						*/

						for(size_t x = 0; x < len_pattern; ++x)
						{
							((*(n+x)).properties).push_back(markable_name); //add markable name to properties

							if(current_pattern[x].head == 1)
							{
								((*(n+x)).properties).push_back(L"head"); // add "head" to properties
							}
						}

					}
				}
			}
		}
	}

	return context;
}

