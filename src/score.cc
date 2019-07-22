#include "score.h"
#include "parse_ref.h"
#include "pattern_ref.h"

#include<string>
#include<vector>
#include<iostream>
#include<queue>
#include<deque>

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


	//Start going through sentences(current to earliest) and apply all indicators to modify scores of the NPs 
	for(deque< vector<unique_LU> >::reverse_iterator i = context_with_prop.rbegin(); i!=context_with_prop.rend(); ++i) //read through the queue in reverse
	{
		firstNP = 1; //firstNP flag true

		for (vector<unique_LU>::iterator j = (*i).begin(); j!=(*i).end(); ++j) //read through sentence
		{
			cout << "\n";
			wcerr << (*j).wordform;
			cout << ": ";
			print_tags((*j).properties);
			cout << "\n";

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
					if(contains(antecedent_LU.properties, L"PP"))
					{
						temp_score -= 1; //Prepositional NP
					}

					//Add to Antecedent List with Score
					antecedent antecedent_with_score = {antecedent_LU, temp_score};
					antecedent_list.push_back(antecedent_with_score);
				}
				else
				{
					cerr << "\nAgreement Failed for:";
					wcerr << antecedent_LU.wordform;
					cerr << "\n";
				}
			}
		}

		if(distance_marker > -1)
			distance_marker--;
	}
}

int Scoring::check_agreement(vector<wstring> antecedent_tags, vector<wstring> anaphor_tags)
{
	if(contains(anaphor_tags, L"f") && contains(antecedent_tags, L"m"))
		return 0;

	if(contains(anaphor_tags, L"m") && contains(antecedent_tags, L"f"))
		return 0;

	return 1;
}


wstring Scoring::get_antecedent()
{
	unique_LU final_antecedent_LU;
	antecedent final_antecedent = {final_antecedent_LU, -5};

	for(vector<antecedent>::reverse_iterator it=antecedent_list.rbegin();it!=antecedent_list.rend();++it) //read it in reverse so that we read from furthest to nearest
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