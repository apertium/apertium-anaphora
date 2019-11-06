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


#include "parse_arx.h"
#include "parse_biltrans.h"
#include "score.h"
#include "pattern_arx.h"

#include <lttoolbox/lt_locale.h>


#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <libgen.h>
#include <getopt.h>

using namespace std;

FILE * open_input(string const &filename)
{
  FILE *input = fopen(filename.c_str(), "r");
  if(!input)
  {
    wcerr << "Error: can't open input file '";
    wcerr << filename.c_str() << "'." << endl;
    exit(EXIT_FAILURE);
  }

  return input;
}

FILE * open_output(string const &filename)
{
  FILE *output = fopen(filename.c_str(), "w");
  if(!output)
  {
    wcerr << "Error: can't open output file '";
    wcerr << filename.c_str() << "'." << endl;
    exit(EXIT_FAILURE);
  }
  return output;
}

void help_message(char *progname)
{
	wcerr << "USAGE: " << basename(progname) << " arx_file [input [output]]" << endl;
	wcerr << "       " << basename(progname) << " -z arx_file [input [output]]" << endl;
	wcerr << "  arx_file   Anaphora Resolution rules file (apertium-xxx-yyy.xxx-yyy.arx)" << endl;

	//wcerr << "  input      input file, standard input by default" << endl;
	//wcerr << "  output     output file, standard output by default" << endl;

	wcerr << "  -z         null-flushing output on \\0" << endl;
	wcerr << "  -h         shows this message" << endl;

	exit(EXIT_FAILURE);
}



int main(int argc, char **argv)
{
	int debug_flag = 0; //flag set by --debug

	char *arxFileName = nullptr;

	int nullFlush = 0;

	LtLocale::tryToSetLocale();

	int c;

	while (1)
	{
		static struct option long_options[] =
		{
			{"debug", 		no_argument,	0,	'd'},
			{"null-flush",	no_argument,	0,	'z'},
			{"help",		no_argument,	0,	'h'},
			{0, 0, 0, 0}
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "dzh", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
		break;

		switch (c)
		{
		case 0:
			fprintf (stderr, "option %s", long_options[option_index].name);
			if (optarg)
			fprintf (stderr, " with arg %s", optarg);
			fprintf (stderr, "\n");
			break;

		case 'd':
			debug_flag = 1;
			break;

		case 'z':
			nullFlush = 1;
			break;

		case '?':
			/* getopt_long already printed an error message. */
			break;

		case 'h':
		default:
			help_message(argv[0]);
			break;
		}
	}

	FILE *input = stdin, *output = stdout;

	switch(argc - optind)
	{
		case 1: // if only one argument left, it has to be the arx_file
			arxFileName = argv[argc - 1]; 
			break;

		case 2: // if two arguments, it has to be arx_file and input_file
			arxFileName = argv[argc - 2];
			input = open_input(argv[argc - 1]); 
			break;

		case 3: // if three arguments, it has to be arx_file, input file and output file
			arxFileName = argv[argc - 3];
			input = open_input(argv[argc - 2]); 
			output = open_output(argv[argc - 1]);
			break;

		default:
			help_message(argv[0]);
			break;
	}

	wchar_t input_char;

	wstring input_stream;

	wstring final_ref;
	Scoring score_module;
	unsigned int gen_id = 0;

	wstring sl_form;
	wstring tl_form;
	vector<wstring> sl_tags;
	vector<wstring> tl_tags;

	ParseArx arx_file;
	arx_file.parseDoc(arxFileName);

	int flag_LU = 0;

	input_char = fgetwc(input);

	while(input_char!=EOF)
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
				fputwc(input_char, output);

				input_char = fgetwc(input);

				fputwc(input_char, output);
			}
			else //inside LU
			{
				input_stream.push_back(input_char);
				fputwc(input_char, output);

				input_char = fgetwc(input);

				fputwc(input_char, output);
				input_stream.push_back(input_char);
			}
		}
		else
		{
			if(flag_LU == 0) //Not Part of an LU
			{
				fputwc(input_char, output);

				if(input_char == L'^')
					flag_LU = 1;
			}

			else if(flag_LU == 1) //Part of an LU
			{
				if(input_char == L'$')
				{
					gen_id++; //generate ids for LUs

					fputwc(L'/', output); //for adding ref

					flag_LU = 0;

					ParseLexicalUnit LU(input_stream); //Parse Lexical Unit using parse_biltrans

					tl_form = LU.get_tl_form();
					tl_tags = LU.get_tl_tags();
					sl_form = LU.get_sl_form();
					sl_tags = LU.get_sl_tags();

					if(!tl_form.empty()) //if TL exists
					{
						int retval;

						retval = score_module.add_word(gen_id, sl_form, sl_tags, tl_form, arx_file); //Give word to Scoring Module
						//If retval is 0, nothing will be added in side ref
						
						//If retval is 1, we call get_antecedent() and add it to ref
						if(retval == 1)
						{
							final_ref = score_module.get_antecedent(debug_flag);

							fputws(final_ref.c_str(), output); //add antecedent to side ref of LU
						}
					}

					input_stream.clear();
				}
				else
				{
					input_stream.push_back(input_char);
				}

				fputwc(input_char, output);

			}
		}

		input_char = fgetwc(input);
	}

	//fclose(fin);

	return 0;
}

