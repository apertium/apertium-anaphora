#include "score.h"

#include<string>
#include<vector>
#include<iostream>
#include<queue>

using namespace std;

void showq(queue < vector<unique_LU> > gq) 
{ 
    queue < vector<unique_LU> > g = gq; 
    
    while (!g.empty()) 
    { 
    	vector<unique_LU> temp_sentence = g.front();
    	
    	cout << "\n";
    	for (std::vector<unique_LU>::iterator i = temp_sentence.begin(); i != temp_sentence.end(); ++i)
    	{
    		wcout << (*i).wordform;
    		cout << ": " << (*i).score << " ";
    	}

    	cout << "\n";

        g.pop(); 
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

		context.push(sentence);

		if(contains(input_LU.pos_tags, L"sent")) //if sentence end (somehow the first LU is a sentence end)
		{
			vector<unique_LU> new_sentence;

			context.push(new_sentence); //add an empty sentence
		}
	}
	else //if queue is not empty
	{
		context.back().push_back(input_LU); //add word to the latest added sentence in the queue

		if(contains(input_LU.pos_tags, L"sent"))
		{
			vector<unique_LU> new_sentence;

			context.push(new_sentence); //add an empty sentence

			if(context.size() > 4)
				context.pop(); //remove the earliest added sentence (We only want current and three previous sentences in context)
		}
		else if( contains(input_LU.pos_tags, L"det") && contains(input_LU.pos_tags, L"pos")	)
		{
			anaphor = input_LU;
			showq(context);
		}
	}
}

/*

void Scoring::referential_distance()
{
	for(vector<antecedent>::iterator it=antecedent_list.begin();it!=antecedent_list.end();++it)
	{
		if((*it).score > -1) //-1 is minimum score
			(*it).score--;
	}
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
	clearq(context); //empty queue

	unique_LU EmptyStruct;
	anaphor = EmptyStruct; //empty anaphor variable
}