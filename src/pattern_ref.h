#ifndef _PATTERNREF_
#define _PATTERNREF_

#include "parse_ref.h"

#include<string>
#include<vector>
#include<deque>

using namespace std;

struct unique_LU 
{
	unsigned int id;
	wstring wordform;
	wstring tl_wordform;
	vector<wstring> pos_tags;
	vector<wstring> properties;
};

struct antecedent
{
	unique_LU LU;
	int score;
};

int contains(vector<wstring> tags, wstring tag);
int contains_any(vector<wstring> tags, vector<wstring> candidates);

int check_acceptable_tags(vector<wstring> input_tags, acceptable_tags check_tags);

deque< vector<unique_LU> > add_properties(deque< vector<unique_LU> > context, ParseRef ref_file)

#endif