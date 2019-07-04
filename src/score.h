#ifndef _MITKOVSCORE_
#define _MITKOVSCORE_

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
	int score;
};

void showq(deque < vector<unique_LU> > gq);

int contains(vector<wstring> tags, wstring tag);
int contains_any(vector<wstring> tags, vector<wstring> candidates);

class Scoring
{
private:
	deque< vector<unique_LU> > context; //A queue of sentences. Each sentence is a vector of Lexical Units
	vector<unique_LU> antecedent_list; //A list of antecedents

public:
	void add_word(unsigned int input_id, wstring input_wordform, vector< wstring > pos_tags, wstring input_tl_wordform);
	void apply_indicators(unique_LU anaphor);
	//void referential_distance();
	//wstring get_antecedent();
	void clear();
};


#endif