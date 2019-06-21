#include "parse_biltrans.h"

#include<cstdio>
#include<fstream>
#include<string>
#include<iostream>
#include<vector>

using namespace std;

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

struct sal_score
{
	wstring wordform;
	int score;
};

int main()
{

	vector< sal_score > scores;
	sal_score unit;

	char input_char;

	input_char = fgetc(stdin);

	wstring input_stream;
	wstring last_noun;

	wstring temp_form;
	vector< wstring > temp_tags;

	int flag_LU = 0;

	while(input_char!=EOF)
	{

		if(input_char == '\\') //dealing with escaped characters
		{
			if(flag_LU == 0) // not inside LU
			{
				fprintf(stdout, "%c", input_char);
				
				input_char = fgetc(stdin);
				
				fprintf(stdout, "%c", input_char);
			}
			else //inside LU
			{
				input_stream.push_back(input_char);
				fprintf(stdout, "%c", input_char);

				input_char = fgetc(stdin);
				
				fprintf(stdout, "%c", input_char);
				input_stream.push_back(input_char);
			}
		}
		else
		{
			if(flag_LU == 0) //Not Part of an LU
			{
				fprintf(stdout, "%c", input_char);

				if(input_char == '^')
					flag_LU = 1;
			}

			else if(flag_LU == 1) //Part of an LU
			{
				if(input_char == '$')
				{
					fprintf(stdout, "/"); //for extra LU

					flag_LU = 0;
					LexicalUnit LU(input_stream); //Parse Lexical Unit using parse_biltrans

					temp_form = LU.get_tl_form();  
					temp_tags = LU.get_tl_tags();

					if(!temp_form.empty()) //if TL exists
					{
						if(contains(temp_tags, L"n")) 
							/* if TL contains antecedent tag */
						{
							last_noun = temp_form;
						}

						if((contains(temp_tags, L"det") && contains(temp_tags, L"pos")) || contains(temp_tags, L"prn"))
							/* if TL tags has det and pos OR just prn*/
						{
							wcout << last_noun; //add last seen noun to LU //CHANGE
						}
					}

					input_stream.clear();

				}
				else
				{
					input_stream.push_back(input_char);
				}

				fprintf(stdout, "%c", input_char);
				
			}
		}

		input_char = fgetc(stdin);
	}

	//fclose(fin);

	return 0;
}

