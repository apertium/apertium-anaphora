#include "parse_biltrans.h"

#include<vector>
#include<iostream>
#include<string>

using namespace std;

LexicalUnit::LexicalUnit(wstring input_LU)
{
	int seenSlash = 0;
	int seenTag = 0;

	wstring temptag;

	for (auto i = input_LU.begin(); i != input_LU.end(); ++i)
	{
		if(*i == '\\') //dealing with escaped characters
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
					++i;
					sl_form.push_back(*i);
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
					++i;
					tl_form.push_back(*i);
				}
			}
			else
			{
				break;
			}
		}

		else if(*i == '/')
			seenSlash++;

		else if(seenSlash == 0) //sl
		{
			sl_form.push_back(*i); //add to the sl form

			if(*i == '<') //start reading tag
				seenTag++;

			else if(seenTag == 1) //inside a tag
			{
				if(*i == '>') //if tag ends
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
		}

		else if(seenSlash == 1) //tl (only first entry in tl)
		{
			tl_form.push_back(*i); //add to the tl form

			if(*i == '<') //start reading tag
				seenTag++;

			else if(seenTag == 1) //inside a tag
			{
				if(*i == '>') //if tag ends
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
		}

		else //if tl has more than one entry
		{
			break;
		}
	}
}

wstring LexicalUnit::get_sl_form()
{
	return sl_form;
}

wstring LexicalUnit::get_tl_form()
{
	return tl_form;
}

vector< wstring > LexicalUnit::get_sl_tags()
{
	return sl_tags;
}

vector< wstring > LexicalUnit::get_tl_tags()
{
	return tl_tags;
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

	LexicalUnit lu(inputlu);

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


