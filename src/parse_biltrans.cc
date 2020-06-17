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
		if(*i == L'\\')
		{
			if(seenSlash == 0)
			{
				if(seenTag == 1)
				{
					temptag.push_back(*i);
					sl_form.push_back(*i);
					++i;
					temptag.push_back(*i);
					sl_form.push_back(*i);
				}
				else
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
				if(seenTag == 1)
				{
					temptag.push_back(*i);
					tl_form.push_back(*i);
					++i;
					temptag.push_back(*i);
					tl_form.push_back(*i);
				}
				else
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

		else if(seenSlash == 0)
		{
			sl_form.push_back(*i);

			if(*i == L'<')
				seenTag++;

			else if(seenTag == 1)
			{
				if(*i == L'>')
				{
					seenTag--;
					sl_tags.push_back(temptag);

					temptag.clear();
				}
				else
				{
					temptag.push_back(*i);
				}
			}
      
      else
      {
        sl_lemma.push_back(*i);
      }
		}

		else if(seenSlash == 1)
		{
			tl_form.push_back(*i);
			
			if(*i == L'<')
				seenTag++;

			else if(seenTag == 1)
			{
				if(*i == L'>')
				{
					seenTag--;
					tl_tags.push_back(temptag);

					temptag.clear();
				}
				else
				{
          temptag.push_back(*i);
				}
			}
      
      else
      {
        tl_lemma.push_back(*i);
      }
		}

		else
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
