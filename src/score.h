#ifndef _MITKOVSCORE_
#define _MITKOVSCORE_

#include "parse_ref.h"
#include "pattern_ref.h"

#include<string>
#include<vector>
#include<queue>
#include<deque>

using namespace std;

void showq(deque < vector<unique_LU> > gq);

class Scoring
{
private:
	deque< vector<unique_LU> > context; //A queue of sentences. Each sentence is a vector of Lexical Units
	vector<antecedent> antecedent_list; //A list of antecedents

public:
	int add_word(unsigned int input_id, wstring input_wordform, vector< wstring > pos_tags, wstring input_tl_wordform, ParseRef ref_file);
	void apply_indicators(unique_LU anaphor, ParseRef ref_file);
	int check_agreement(vector<wstring> antecedent_tags, vector<wstring> anaphor_tags);
	wstring get_antecedent();
	void clear();
};


#endif