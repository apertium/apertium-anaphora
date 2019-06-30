#include "parse_biltrans.h"
#include "score.h"

#include<cstdio>
#include<fstream>
#include<string>
#include<iostream>
#include<vector>

using namespace std;

int main(int argc, char **argv)
{
	int nullFlush = 0;

	if(argc > 1) //flag given
	{
		if(strcmp(argv[1], "-z") == 0)
			nullFlush = 1;
	}

	vector< sal_score > scores;
	sal_score unit;

	char input_char;

	input_char = fgetc(stdin); //change to fgetwc ?

	wstring input_stream;
	wstring last_noun;

	wstring temp_form;
	vector< wstring > temp_tags;

	int flag_LU = 0;

	while(input_char!=EOF) // should I made feof(input_char) ?
	{
		if(nullFlush && input_char == '\0') //nullFlush
		{
			scores.clear();
			input_stream.clear();
			last_noun.clear();
			temp_form.clear();
			temp_tags.clear();

			flag_LU = 0;
		}

		else if(input_char == '\\') //dealing with escaped characters
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
					ParseLexicalUnit LU(input_stream); //Parse Lexical Unit using parse_biltrans

					temp_form = LU.get_tl_form();  
					temp_tags = LU.get_tl_tags();

					if(!temp_form.empty()) //if TL exists
					{
						if(contains(temp_tags, L"n"))
							/* if TL contains antecedent tag */
						{
							last_noun = temp_form;
						}

						if( (contains(temp_tags, L"det") && contains(temp_tags, L"pos") ) || contains(temp_tags, L"prn") || contains(temp_tags, L"vblex") || contains(temp_tags, L"vbser") || contains(temp_tags, L"vbhaver") || contains(temp_tags, L"vbmod") )
							/* if TL tags has det and pos OR just prn OR any verb*/
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

