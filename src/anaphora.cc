#include "parse_biltrans.h"

#include<cstdio>
#include<fstream>
#include<string>
#include<iostream>

using namespace std;

void print(vector<char> const &input)
{
	for (int i = 0; i < input.size(); i++) 
	{
		fprintf(stdout, "%c", input.at(i));
	}
}

int contains(vector< vector<char> > tags, vector<char> tag)
{
	if(std::find(tags.begin(), tags.end(), tag) != tags.end())
		return 1;
	else
		return 0;
}

vector<char> string_to_vector(char *string_in)
{
	vector<char> temp;

	for(int i = 0; i < strlen(string_in); i++)
		temp.push_back(string_in[i]);

	return temp;
}

int main()
{
	char input_char;

	input_char = fgetc(stdin);

	vector<char> input_stream;
	vector<char> last_noun;

	vector<char> temp_form;
	vector< vector<char> > temp_tags;

	vector<char> antecedent_tag = string_to_vector("n");

	vector<char> r1_tag1 = string_to_vector("det");
	vector<char> r1_tag2 = string_to_vector("pos");

	vector<char> r2_tag1 = string_to_vector("prn");

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
						if(contains(temp_tags, antecedent_tag)) 
							/* if TL contains antecedent tag */
						{
							last_noun = temp_form;
						}

						if((contains(temp_tags, r1_tag1) && contains(temp_tags, r1_tag2)) || contains(temp_tags, r2_tag1))
							/* if TL tags has det and pos OR just prn*/
						{
							print(last_noun); //add last seen noun to LU
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

