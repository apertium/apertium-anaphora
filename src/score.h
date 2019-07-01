#ifndef _MITKOVSCORE_
#define _MITKOVSCORE_

#include<string>
#include<vector>

using namespace std;

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

int contains(vector<wstring> tags, wstring tag);
int contains_any(vector<wstring> tags, vector<wstring> candidates);

class Scoring
{
private:
	vector<unique_LU> context;
	vector<antecedent> antecedent_list;

public:
	void add_word(unsigned int input_id, wstring input_wordform, vector< wstring > pos_tags);
	void referential_distance();
	wstring get_antecedent();
	void clear();
};


#endif