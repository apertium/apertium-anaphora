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

#include "score.h"
#include "parse_ref.h"
#include "pattern_ref.h"

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <deque>

using namespace std;

void showq(deque < vector<unique_LU> > gq)
{
    for(std::deque < vector<unique_LU> >::iterator j = gq.begin(); j != gq.end(); ++j)
    {
    	vector<unique_LU> temp_sentence = *j;

    	cerr << "\n";
    	for (std::vector<unique_LU>::iterator i = temp_sentence.begin(); i != temp_sentence.end(); ++i)
    	{
    		wcerr << (*i).wordform;
    	}

    	cerr << "\n";
    }
    cerr << '\n';
}

int Scoring::add_word(unsigned int input_id, wstring input_wordform, vector< wstring > input_pos_tags, wstring input_tl_wordform, ParseRef ref_file)
{
	vector<wstring> temp_prop;
	unordered_map<wstring, acceptable_tags> ref_parameters = ref_file.get_parameters();

	unique_LU input_LU = {input_id, input_wordform, input_tl_wordform, input_pos_tags, temp_prop}; //initialise LU

	if(context.empty()) //if queue is empty
	{
		vector<unique_LU> sentence; //initialise a sentence
		sentence.push_back(input_LU); //add the first word to the sentence

		context.push_back(sentence);

		if(check_acceptable_tags(input_LU.pos_tags, ref_parameters[L"delimiter"]) ) //if sentence end (somehow the first LU is a sentence end)
		{
			vector<unique_LU> new_sentence;

			context.push_back(new_sentence); //add an empty sentence
		}
	}
	else //if queue is not empty
	{
		context.back().push_back(input_LU); //add word to the latest added sentence in the queue

		if(check_acceptable_tags(input_LU.pos_tags, ref_parameters[L"delimiter"]) )
		{
			vector<unique_LU> new_sentence;

			context.push_back(new_sentence); //add an empty sentence

			if(context.size() > 4)
				context.pop_front(); //remove the earliest added sentence (We only want current and three previous sentences in context)
		}
		else if( check_acceptable_tags(input_LU.pos_tags, ref_parameters[L"anaphor"]) ) //check if tags of current word match with anaphor tags in ref file
		{
			apply_indicators(input_LU, ref_file);
			return 1; //To show that something will be added in side ref
		}
	}

	return 0; //To show that nothing will be added in side ref
}

void Scoring::apply_indicators(unique_LU anaphor, ParseRef ref_file)
{
	int distance_marker = 2; //starts from 2 for current sentence and reduces till -1 as we go to previous sentences
	int temp_score;
	int firstNP; //first NP flag

	antecedent_list.clear(); //clear it from the last anaphor

	//Go through the context and add properties based on external file
	deque< vector<unique_LU> > context_with_prop = add_properties(context, ref_file); //dont add properties in the actual context (might wanna change)

	distance_marker = distance_marker - context_with_prop.size() + 1; //set distance to earliest sentence based on number of sentences in context

	//Get scores for markables in a variable
	unordered_map<wstring, int> markables_score = ref_file.get_markables_score();

	//Start going through sentences(earliest to current) and apply all indicators to modify scores of the NPs
	for(deque< vector<unique_LU> >::iterator i = context_with_prop.begin(); i!=context_with_prop.end(); ++i) //read through the queue in reverse
	{
		firstNP = 1; //firstNP flag true

		for (vector<unique_LU>::iterator j = (*i).begin(); j!=(*i).end(); ++j) //read through sentence
		{
			//cerr << "\n";
			//wcerr << (*j).wordform;
			//cerr << ": ";
			//print_tags((*j).properties);
			//cerr << "\n";

			if(check_acceptable_tags((*j).pos_tags, ref_file.get_parameters()[L"antecedent"]) ) // if it is antecedent (based on external xml file)
			{
				temp_score = 0;

				unique_LU antecedent_LU = *j; //create a temp copy of the potential antecedent

				//Check Agreement
				if(check_agreement(antecedent_LU.pos_tags, anaphor.pos_tags))
				{
					//Add or Remove Indicators Here
					temp_score += distance_marker; //Referential Distance (based on how close the antecedent is to the pronoun)

					//Boosting Indicators
					if(firstNP)
					{
						temp_score += 1; //First NP
						firstNP = 0;
					}

					//Impeding Indicators

					//Indicators from XML file (iterate through all markables that provided a score)

					for(unordered_map<wstring, int>::iterator x = markables_score.begin(); x != markables_score.end(); ++x)
					{
						//cout << "Checking for: ";
						//wcout << x->first;
						//cout << "\n";

						if(contains(antecedent_LU.properties, x->first)) //if markable name present in current antecedent
						{
							temp_score += x->second; //Add score to the temp score (could be negative also)
						}
					}

					//Add to Antecedent List with Score
					antecedent antecedent_with_score = {antecedent_LU, temp_score};
					antecedent_list.push_back(antecedent_with_score);
				}
				else
				{
					//cerr << "\nAgreement Failed for:";
					//wcerr << antecedent_LU.wordform;
					//cerr << "\n";
				}
			}
		}

		if(distance_marker < 2)
			distance_marker++;
	}
}

int Scoring::check_agreement(vector<wstring> antecedent_tags, vector<wstring> anaphor_tags)
{
	/*
	if(contains(anaphor_tags, L"f") && contains(antecedent_tags, L"m"))
		return 0;

	if(contains(anaphor_tags, L"m") && contains(antecedent_tags, L"f"))
		return 0;
		*/

	return 1; //for now, return 1 always
}


wstring Scoring::get_antecedent()
{
	unique_LU final_antecedent_LU;
	antecedent final_antecedent = {final_antecedent_LU, -5};

	for(vector<antecedent>::iterator it=antecedent_list.begin();it!=antecedent_list.end();++it) //read from furthest to nearest
	{
		//cerr << "\n" << (*it).LU.id << ": ";
		//wcerr << (*it).LU.wordform;
		//cerr << " : " << (*it).score << "\n";

		if((*it).score >= final_antecedent.score) //picking the highest scored and latest added (most recent) antecedent
			final_antecedent = (*it);
	}

	antecedent_list.clear();

	return final_antecedent.LU.tl_wordform;
}

void Scoring::clear() //use a destructor?
{
	context.clear(); //empty queue
	antecedent_list.clear(); //empty antecedent list
}