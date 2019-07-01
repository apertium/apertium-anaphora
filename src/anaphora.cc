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

	wchar_t input_char;

	input_char = fgetwc(stdin); //change to fgetwc ?

	wstring input_stream;

	wstring final_ref;
	Scoring score_module;
	unsigned int gen_id = 0;

	wstring sl_form;
	wstring tl_form;
	vector<wstring> sl_tags;
	vector<wstring> tl_tags;

	int flag_LU = 0;

	while(input_char!=EOF) // should I made feof(input_char) ?
	{
		if(nullFlush && input_char == L'\0') //nullFlush
		{
			input_stream.clear();
			sl_form.clear();
			tl_form.clear();
			sl_tags.clear();
			tl_tags.clear();
			gen_id = 0;
			score_module.clear();

			final_ref.clear();

			flag_LU = 0;
		}

		else if(input_char == L'\\') //dealing with escaped characters
		{
			if(flag_LU == 0) // not inside LU
			{
				fwprintf(stdout, L"%C", input_char);
				
				input_char = fgetc(stdin);
				
				fwprintf(stdout, L"%C", input_char);
			}
			else //inside LU
			{
				input_stream.push_back(input_char);
				fwprintf(stdout, L"%C", input_char);

				input_char = fgetc(stdin);
				
				fwprintf(stdout, L"%C", input_char);
				input_stream.push_back(input_char);
			}
		}
		else
		{
			if(flag_LU == 0) //Not Part of an LU
			{
				fwprintf(stdout, L"%C", input_char);

				if(input_char == L'^')
					flag_LU = 1;
			}

			else if(flag_LU == 1) //Part of an LU
			{
				if(input_char == L'$')
				{
					gen_id++; //generate ids for LUs

					fwprintf(stdout, L"/"); //for adding ref

					flag_LU = 0;
					ParseLexicalUnit LU(input_stream); //Parse Lexical Unit using parse_biltrans

					tl_form = LU.get_tl_form();  
					tl_tags = LU.get_tl_tags();
					sl_form = LU.get_sl_form();
					sl_tags = LU.get_sl_tags();

					if(!tl_form.empty()) //if TL exists
					{
						score_module.add_word(gen_id, sl_form, sl_tags, tl_form);

						if( (contains(sl_tags, L"det") && contains(sl_tags, L"pos") ) )//|| contains(temp_tags, L"prn") || contains(temp_tags, L"vblex") || contains(temp_tags, L"vbser") || contains(temp_tags, L"vbhaver") || contains(temp_tags, L"vbmod") )
							/* if TL tags has det and pos OR just prn OR any verb*/
						{
							final_ref = score_module.get_antecedent();
							wcout << final_ref; //add antecedent to side ref of LU //CHANGE
						}
					}

					input_stream.clear();
				}
				else
				{
					input_stream.push_back(input_char);
				}

				fwprintf(stdout, L"%C", input_char);
				
			}
		}

		input_char = fgetwc(stdin);
	}

	//fclose(fin);

	return 0;
}

