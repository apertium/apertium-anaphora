#include "score.h"

#include<string>
#include<vector>
#include<iostream>

using namespace std;

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

void Scoring::add_word(unsigned int input_id, wstring input_wordform, vector< wstring > pos_tags, wstring input_tl_wordform)
{
	unique_LU input_LU = {input_id, input_wordform};
	context.push_back(input_LU); //add to context

	if(contains(pos_tags, L"n")) //if word is a noun, add to antecedents list with score=2 as it is in current context(referential distance)
	{
		antecedent input_antecedent = {input_id, input_wordform, 2, input_tl_wordform};
		antecedent_list.push_back(input_antecedent);
	}

	if(contains(pos_tags, L"sent")) //if reached sentence boundary, reduce scores (referential distance)
		referential_distance();
}

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

void Scoring::clear()
{
	context.clear();
	antecedent_list.clear();
}