#ifndef _PATTERNREF_
#define _PATTERNREF_

#include "parse_ref.h"

#include<string>
#include<vector>

using namespace std;

int contains(vector<wstring> tags, wstring tag);
int contains_any(vector<wstring> tags, vector<wstring> candidates);

int check_acceptable_tags(vector<wstring> input_tags, acceptable_tags check_tags);

#endif