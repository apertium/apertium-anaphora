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

void help_message(char *progname)
{
	wcerr << "USAGE: " << basename(progname) << " arx_file" << endl;
	wcerr << "       " << basename(progname) << " -z arx_file" << endl;
	wcerr << "  arx_file   Anaphora Resolution rules file (apertium-xxx-yyy.xxx-yyy.arx)" << endl;

	//wcerr << "  input      input file, standard input by default" << endl;
	//wcerr << "  output     output file, standard output by default" << endl;

	wcerr << "  -z         null-flushing output on \\0" << endl;
	wcerr << "  -h         shows this message" << endl;

	exit(EXIT_FAILURE);
}

static int debug_flag; //flag set by --debug

int main(int argc, char **argv)
{
	char *arxFileName = nullptr;

	int nullFlush = 0;

	LtLocale::tryToSetLocale();

	int c;

	while (1)
	{
		static struct option long_options[] =
		{
			{"debug", no_argument,       &debug_flag, 1},
			{"null-flush",     no_argument,       0, 'z'},
			{"help",  no_argument,       0, 'h'},
			{0, 0, 0, 0}
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "zh", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
		break;

		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
			break;
			fprintf (stderr, "option %s", long_options[option_index].name);
			if (optarg)
			fprintf (stderr, " with arg %s", optarg);
			fprintf (stderr, "\n");
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

	if(debug_flag)
		fprintf(stderr, "Debug Flag is set.\n");

	if(argc - optind != 1) 
		help_message(argv[0]);

	arxFileName = argv[optind]; //Name of Arx File is the remaining argument

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

	input_char = fgetwc(stdin);

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
				fputwc(input_char, stdout);

				input_char = fgetwc(stdin);

				fputwc(input_char, stdout);
			}
			else //inside LU
			{
				input_stream.push_back(input_char);
				fputwc(input_char, stdout);

				input_char = fgetwc(stdin);

				fputwc(input_char, stdout);
				input_stream.push_back(input_char);
			}
		}
		else
		{
			if(flag_LU == 0) //Not Part of an LU
			{
				fputwc(input_char, stdout);

				if(input_char == L'^')
					flag_LU = 1;
			}

			else if(flag_LU == 1) //Part of an LU
			{
				if(input_char == L'$')
				{
					gen_id++; //generate ids for LUs

					fputwc(L'/', stdout); //for adding ref

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
							final_ref = score_module.get_antecedent();

							fputws(final_ref.c_str(), stdout); //add antecedent to side ref of LU
						}
					}

					input_stream.clear();
				}
				else
				{
					input_stream.push_back(input_char);
				}

				fputwc(input_char, stdout);

			}
		}

		input_char = fgetwc(stdin);
	}

	//fclose(fin);

	return 1;
}

