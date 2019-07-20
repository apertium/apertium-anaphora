#include "pattern_ref.h"
#include "parse_ref.h"

#include<string>
#include<vector>

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

int check_acceptable_tags(vector<wstring> input_tags, acceptable_tags check_tags) //all tags in any tag list in check_tags must exist in input_tags
{
	for (acceptable_tags::iterator i = check_tags.begin(); i != check_tags.end(); ++i)
	{
		
		int flag_contains_all = 1;

		vector<wstring> temp_tags = *i;

		for(std::vector<wstring>::iterator j = temp_tags.begin(); j != temp_tags.end(); ++j)
		{

			if(*j == L"*") //ignore * in the tags list
				continue;

			if(!contains(input_tags, *j)) //if the required tag is NOT in the input LU tags
			{
				flag_contains_all = 0;
				break;
			}
			/*
			else
			{
				cerr << "FoundTag:";
				wcerr << *j;
				cerr <<"\n";
			}
			*/
		}

		if(flag_contains_all == 1) //if any tag list fully matched
			return 1; //else continue to next tag list
	}

	return 0; //if it didn't return 1 then no tag list was fully matched
}

check_acceptable_patterns(vector)


deque< vector<unique_LU> > add_properties(deque< vector<unique_LU> > context, ParseRef ref_file)
{
	unordered_map<wstring, acceptable_patterns> ref_markables = ref_file.markables;
	unordered_map<wstring, acceptable_tags> ref_cats = ref_file.cats;

	for (unordered_map<wstring, acceptable_patterns>::iterator::it = ref_markables.begin(); it != ref_markables.end(); it++ ) //go through markables defined in xml file
	{
		//for each markable
		wstring markable_name = it->first;
		acceptable_patterns patterns_list = it->second;

		for(acceptable_patterns::iterator i = patterns_list.begin(); i!=patterns_list.end(); ++i) //go through patterns in the markable
		{
			//for each pattern
			vector<markable_pattern> current_pattern = *i;

			for(deque< vector<unique_LU> >::iterator j = context.begin(); j!=context.end(); ++j) //go through queue of context to get sentences
			{
				for (vector<unique_LU>::iterator j = (*i).begin(); j!=(*i).end(); ++j) //go through sentence to look for the pattern with a sliding window of size = pattern length
				{
					
				}
			}

		}
	
	}
	
	/*
	for(deque< vector<unique_LU> >::iterator i = context.begin(); i!=context.end(); ++i)
	{
		
	}
	*/

	return context;
}

