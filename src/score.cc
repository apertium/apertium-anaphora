#include "score.h"

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
    	
    	cout << "\n";
    	for (std::vector<unique_LU>::iterator i = temp_sentence.begin(); i != temp_sentence.end(); ++i)
    	{
    		wcout << (*i).wordform;
    		cout << ": " << (*i).score << " ";
    	}

    	cout << "\n";
    } 
    cout << '\n'; 
} 

void clearq(queue < vector<unique_LU> > q)
{
	while(!q.empty())
	{
		q.pop();
	}
}

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

void Scoring::add_word(unsigned int input_id, wstring input_wordform, vector< wstring > input_pos_tags, wstring input_tl_wordform)
{
	unique_LU input_LU = {input_id, input_wordform, input_tl_wordform, input_pos_tags, 0}; //initialise in context with score 0

	if(context.empty()) //if queue is empty 
	{
		vector<unique_LU> sentence; //initialise a sentence
		sentence.push_back(input_LU); //add the first word to the sentence

		context.push_back(sentence);

		if(contains(input_LU.pos_tags, L"sent")) //if sentence end (somehow the first LU is a sentence end)
		{
			vector<unique_LU> new_sentence;

			context.push_back(new_sentence); //add an empty sentence
		}
	}
	else //if queue is not empty
	{
		context.back().push_back(input_LU); //add word to the latest added sentence in the queue

		if(contains(input_LU.pos_tags, L"sent"))
		{
			vector<unique_LU> new_sentence;

			context.push_back(new_sentence); //add an empty sentence

			if(context.size() > 4)
				context.pop_front(); //remove the earliest added sentence (We only want current and three previous sentences in context)
		}
		else if( contains(input_LU.pos_tags, L"det") && contains(input_LU.pos_tags, L"pos")	)
		{
			apply_indicators(input_LU);
		}
	}
}

void Scoring::apply_indicators(unique_LU anaphor)
{
	int distance_marker = 2; //starts from 2 for current sentence and reduces till -1 as we go to previous sentences

	//Start going through sentences(current to earliest) and apply all indicators to modify scores of the NPs 
	for(deque< vector<unique_LU> >::reverse_iterator i = context.rbegin(); i!=context.rend(); ++i) //read through the queue in reverse
	{
		cout<<"\nSentence:" << distance_marker << "\n";
		for (vector<unique_LU>::iterator j = (*i).begin(); j!=(*i).end(); ++j) //read through sentence
		{
			if(contains((*j).pos_tags, L"n"))
			{
				unique_LU antecedent = *j; //create a temp copy of the potential antecedent

				//Check Agreement
				if(check_agreement(antecedent.pos_tags, anaphor.pos_tags))
				{
					//Add or Remove Indicators Here
					

					//Boosting Indicators


					//Impeding Indicators

					wcout << antecedent.wordform;
					cout << ": " << antecedent.score << "\n";
				}
				else
				{
					cout << "\nAgreement Failed for:";
					wcout << antecedent.wordform;
					cout << "\n";
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

	if(contains(anaphor_tags, L"sg") && contains(antecedent_tags, L"pl"))
		return 0;

	if(contains(anaphor_tags, L"pl") && contains(antecedent_tags, L"sg"))
		return 0;

	return 1;
}

/*
void Scoring::referential_distance(int distance)
{
	
}


wstring Scoring::get_antecedent()
{
	antecedent final_antecedent = {0, L"", -5};

	for(vector<antecedent>::iterator it=antecedent_list.begin();it!=antecedent_list.end();++it)
	{
		cout << "\n" << (*it).id << ": ";
		wcout << (*it).wordform;
		cout << " : " << (*it).score << "\n";

		if((*it).score >= final_antecedent.score) //picking the highest scored and latest added (most recent) antecedent
		{
			final_antecedent.id = (*it).id;
			final_antecedent.wordform = (*it).wordform;
			final_antecedent.score = (*it).score;
			final_antecedent.tl_wordform = (*it).tl_wordform;
		}
	}

	return final_antecedent.tl_wordform;
}
*/
void Scoring::clear() //use a destructor?
{
	context.clear(); //empty queue

	//unique_LU EmptyStruct;
	//anaphor = EmptyStruct; //empty anaphor variable
}