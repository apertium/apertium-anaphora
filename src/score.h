#ifndef _MITKOVSCORE_
#define _MITKOVSCORE_

#include "parse_ref.h"

#include<string>
#include<vector>
#include<queue>
#include<deque>

using namespace std;

struct unique_LU 
{
	unsigned int id;
	wstring wordform;
	wstring tl_wordform;
	vector<wstring> pos_tags;
};

struct antecedent
{
	unique_LU LU;
	int score;
};

void showq(deque < vector<unique_LU> > gq);

int contains(vector<wstring> tags, wstring tag);
int contains_any(vector<wstring> tags, vector<wstring> candidates);

int check_acceptable_tags(vector<wstring> input_tags, acceptable_tags check_tags);

class Scoring
{
private:
	deque< vector<unique_LU> > context; //A queue of sentences. Each sentence is a vector of Lexical Units
	vector<antecedent> antecedent_list; //A list of antecedents

public:
	int add_word(unsigned int input_id, wstring input_wordform, vector< wstring > pos_tags, wstring input_tl_wordform, unordered_map<wstring, acceptable_tags> ref_parameters);
	void apply_indicators(unique_LU anaphor, unordered_map<wstring, acceptable_tags> ref_parameters);
	int check_agreement(vector<wstring> antecedent_tags, vector<wstring> anaphor_tags);
	wstring get_antecedent();
	void clear();
};


#endif