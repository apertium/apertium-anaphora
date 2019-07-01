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

	char input_char;

	input_char = fgetc(stdin); //change to fgetwc ?

	wstring input_stream;

	wstring final_ref;
	Scoring score_module;
	unsigned int gen_id = 0;

	wstring temp_form;
	vector< wstring > temp_tags;

	int flag_LU = 0;

	while(input_char!=EOF) // should I made feof(input_char) ?
	{
		if(nullFlush && input_char == '\0') //nullFlush
		{
			input_stream.clear();
			temp_form.clear();
			temp_tags.clear();
			gen_id = 0;
			score_module.clear();

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
					gen_id++; //generate ids for LUs

					fprintf(stdout, "/"); //for adding ref

					flag_LU = 0;
					ParseLexicalUnit LU(input_stream); //Parse Lexical Unit using parse_biltrans

					temp_form = LU.get_tl_form();  
					temp_tags = LU.get_tl_tags();

					if(!temp_form.empty()) //if TL exists
					{
						score_module.add_word(gen_id, temp_form, temp_tags);

						if( (contains(temp_tags, L"det") && contains(temp_tags, L"pos") ) )//|| contains(temp_tags, L"prn") || contains(temp_tags, L"vblex") || contains(temp_tags, L"vbser") || contains(temp_tags, L"vbhaver") || contains(temp_tags, L"vbmod") )
							/* if TL tags has det and pos OR just prn OR any verb*/
						{
							//cout << "\n\nHERE!!\n\n";
							final_ref = score_module.get_antecedent();
							wcout << final_ref; //add antecedent to side ref of LU
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

