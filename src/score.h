#ifndef _MITKOVSCORE_
#define _MITKOVSCORE_

#include<string>
#include<vector>


struct unique_LU 
{
	unsigned int id;
	wstring wordform;
};

struct antecedent
{
	unsigned int id;
	wstring wordform;
	int score;
};

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

class Scoring
{
private:
	vector<unique_LU> context;
	vector<antecedent> antecedent_list;

public:
	void add_word(unsigned int input_id, wstring input_wordform, vector< wstring > pos_tags)
	{
		unique_LU input_LU = {input_id, input_wordform};
		context.push_back(input_LU); //add to context

		if(contains(pos_tags, L"n")) //if word is a noun, add to antecedents list with score=0
		{
			antecedent input_antecedent = {input_id, input_wordform, 0};
		}
	}

	void apply_indicators(); //Need to first understand what each indicator needs and how to apply

};


#endif