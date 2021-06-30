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
#include <lttoolbox/input_file.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <libgen.h>
#include <getopt.h>

using namespace std;

UFILE * open_output(string const &filename)
{
	UFILE *output = u_fopen(filename.c_str(), "w", NULL, NULL);
  if(!output)
  {
    cerr << "Error: can't open output file '";
    cerr << filename.c_str() << "'." << endl;
    exit(EXIT_FAILURE);
  }
  return output;
}

void help_message(char *progname)
{
	cerr << "USAGE: " << basename(progname) << " arx_file [input [output]]" << endl;
	cerr << "       " << basename(progname) << " -z arx_file [input [output]]" << endl;
	cerr << "  arx_file   Anaphora Resolution rules file (apertium-xxx-yyy.xxx-yyy.arx)" << endl;
	cerr << "  -z         null-flushing output on \\0" << endl;
	cerr << "  -h         shows this message" << endl;

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

	InputFile input;
	UFILE* output = u_finit(stdout, NULL, NULL);

	switch(argc - optind)
	{
		case 1: // if only one argument left, it has to be the arx_file
			arxFileName = argv[argc - 1];
			break;

		case 2: // if two arguments, it has to be arx_file and input_file
			arxFileName = argv[argc - 2];
			input.open_or_exit(argv[argc - 1]);
			break;

		case 3: // if three arguments, it has to be arx_file, input file and output file
			arxFileName = argv[argc - 3];
			input.open_or_exit(argv[argc - 2]);
			output = open_output(argv[argc - 1]);
			break;

		default:
			help_message(argv[0]);
			break;
	}

	UChar32 input_char;

	UString input_stream;

	UString final_ref;
	Scoring score_module;
	unsigned int gen_id = 0;

	UString sl_form;
	UString tl_form;
	vector<UString> sl_tags;
	vector<UString> tl_tags;
	UString sl_lemma;
	UString tl_lemma;

	ParseArx arx_file;
	int parse_arx_retval = arx_file.parseDoc(arxFileName);

	if(parse_arx_retval != 0)
		exit(EXIT_FAILURE);

	int flag_LU = 0;

	input_char = input.get();

	while(input_char!=U_EOF)
	{
		if(nullFlush && input_char == '\0')
		{
			u_fputc(input_char, output);
			u_fflush(output);

			input_stream.clear();
			sl_form.clear();
			tl_form.clear();
			sl_tags.clear();
			tl_tags.clear();
			sl_lemma.clear();
			tl_lemma.clear();
			gen_id = 0;
			score_module.clear();

			final_ref.clear();

			flag_LU = 0;
		}

		else if(input_char == '\\')
		{
			if(flag_LU == 0)
			{
				u_fputc(input_char, output);

				input_char = input.get();

				u_fputc(input_char, output);
			}
			else
			{
				input_stream.push_back(input_char);
				u_fputc(input_char, output);

				input_char = input.get();

				u_fputc(input_char, output);
				input_stream.push_back(input_char);
			}
		}
		else
		{
			if(flag_LU == 0)
			{
				u_fputc(input_char, output);

				if(input_char == '^')
					flag_LU = 1;
			}

			else if(flag_LU == 1)
			{
				if(input_char == '$')
				{
					gen_id++;

					u_fputc('/', output); //for adding ref

					flag_LU = 0;

					ParseLexicalUnit LU(input_stream);

					tl_form = LU.get_tl_form();
					tl_tags = LU.get_tl_tags();
					sl_form = LU.get_sl_form();
					sl_tags = LU.get_sl_tags();
					sl_lemma = LU.get_sl_lemma();
					tl_lemma = LU.get_tl_lemma();

					if(!tl_form.empty())
					{
						int retval;

						retval = score_module.add_word(gen_id, sl_form, sl_tags, tl_form, sl_lemma, tl_lemma, arx_file, debug_flag);
						//If retval is 0, nothing will be added in side ref

						//If retval is 1, we call get_antecedent() and add it to ref
						if(retval == 1)
						{
							final_ref = score_module.get_antecedent(debug_flag);

							write(final_ref, output);
						}
					}

					input_stream.clear();
				}
				else
				{
					input_stream.push_back(input_char);
				}

				u_fputc(input_char, output);

			}
		}

		input_char = input.get();
	}

	u_fclose(output);

	return 0;
}
