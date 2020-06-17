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

#include "parse_biltrans.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

ParseLexicalUnit::ParseLexicalUnit(wstring input_LU)
{
	int seenSlash = 0;
	int seenTag = 0;

	wstring temptag;

	for (std::wstring::iterator i = input_LU.begin(); i != input_LU.end(); ++i)
	{
		if(*i == L'\\') //dealing with escaped characters
		{
			if(seenSlash == 0) //sl
			{
				if(seenTag == 1) //in a tag
				{
					temptag.push_back(*i);
					sl_form.push_back(*i);
					++i;
					temptag.push_back(*i);
					sl_form.push_back(*i);
				}
				else //not in a tag
				{
					sl_form.push_back(*i);
          sl_lemma.push_back(*i);
					++i;
					sl_form.push_back(*i);
          sl_lemma.push_back(*i);
				}
			}
			else if(seenSlash == 1) //tl (only first entry)
			{
				if(seenTag == 1) //in a tag
				{
					temptag.push_back(*i);
					tl_form.push_back(*i);
					++i;
					temptag.push_back(*i);
					tl_form.push_back(*i);
				}
				else //not in a tag
				{
					tl_form.push_back(*i);
          tl_lemma.push_back(*i);
					++i;
					tl_form.push_back(*i);
          tl_lemma.push_back(*i);
				}
			}
			else
			{
				break;
			}
		}

		else if(*i == L'/')
			seenSlash++;

		else if(seenSlash == 0) //sl
		{
			sl_form.push_back(*i); //add to the sl form

			if(*i == L'<') //start reading tag
				seenTag++;

			else if(seenTag == 1) //inside a tag
			{
				if(*i == L'>') //if tag ends
				{
					seenTag--;
					sl_tags.push_back(temptag); //add tag to list of sl tags

					temptag.clear();
				}
				else
				{
					temptag.push_back(*i); //add char to current tag
				}
			}
      
      else
      {
        sl_lemma.push_back(*i);
      }
		}

		else if(seenSlash == 1) //tl (only first entry in tl)
		{
			tl_form.push_back(*i); //add to the tl form
			
			if(*i == L'<') //start reading tag
				seenTag++;

			else if(seenTag == 1) //inside a tag
			{
				if(*i == L'>') //if tag ends
				{
					seenTag--;
					tl_tags.push_back(temptag); //add tag to list of tl tags

					temptag.clear();
				}
				else
				{
					temptag.push_back(*i); //add char to current tag
				}
			}
      
      else
      {
        tl_lemma.push_back(*i);
      }
		}

		else //if tl has more than one entry
		{
			break;
		}
	}
}

wstring ParseLexicalUnit::get_sl_form()
{
	return sl_form;
}

wstring ParseLexicalUnit::get_tl_form()
{
	return tl_form;
}

vector< wstring > ParseLexicalUnit::get_sl_tags()
{
	return sl_tags;
}

vector< wstring > ParseLexicalUnit::get_tl_tags()
{
	return tl_tags;
}

wstring ParseLexicalUnit::get_sl_lemma()
{
  return sl_lemma;
}

wstring ParseLexicalUnit::get_tl_lemma()
{
  return tl_lemma;
}

/* //Uncomment to test this code

void print_tags(vector< wstring > input)
{
	for (int i = 0; i < input.size(); i++)
	{
		wcout << input[i];
		cout << " ";
	}
}

int main()
{
	wstring inputlu;
	char input_char;

	input_char = fgetc(stdin);

	while(input_char != '\n')
	{
		inputlu.push_back(input_char);

		input_char = fgetc(stdin);
	}

	ParseLexicalUnit lu(inputlu);

	cout << "SL: ";
	wcout << lu.get_sl_form();
	cout << endl << "SL tags: ";
	print_tags(lu.get_sl_tags());
	cout << endl << "TL: ";
	wcout << lu.get_tl_form();
	cout << endl << "TL tags: ";
	print_tags(lu.get_tl_tags());
	cout << endl;

	return 0;
}
*/
