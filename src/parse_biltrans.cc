#include<vector>
#include<iostream>

using namespace std;

class LexicalUnit
{

private:

	vector<char> sl_form;
	vector<char> tl_form;

	vector< vector<char> > sl_tags;
	vector< vector<char> > tl_tags;

public:

	LexicalUnit(vector<char> input_LU)
	{
		int seenSlash = 0;
		int seenTag = 0;

		vector<char> temptag;

		for (auto i = input_LU.begin(); i != input_LU.end(); ++i)
		{
			if(*i == '/')
				seenSlash++;

			else if(seenSlash == 0) //sl
			{
				sl_form.push_back(*i); //add to the sl form

				if(*i == '<') //start reading tag
					seenTag++;

				else if(seenTag == 1) //inside a tag
				{
					if(*i == '>') //if tag ends
					{
						seenTag--;
						sl_tags.push_back(temptag); //add tag to list of sl tags

						temptag.clear();
					}
					else
					{
						temptag.push_back(*i); //add char to current tag
					}
				}
			}

			else //tl
			{
				tl_form.push_back(*i); //add to the tl form

				if(*i == '<') //start reading tag
					seenTag++;

				else if(seenTag == 1) //inside a tag
				{
					if(*i == '>') //if tag ends
					{
						seenTag--;
						tl_tags.push_back(temptag); //add tag to list of tl tags

						temptag.clear();
					}
					else
					{
						temptag.push_back(*i); //add char to current tag
					}
				}
			}
		}
	}

	vector<char> get_sl_form()
	{
		return sl_form;
	}

	vector<char> get_tl_form()
	{
		return tl_form;
	}

	vector< vector<char> > get_sl_tags()
	{
		return sl_tags;
	}

	vector< vector<char> > get_tl_tags()
	{
		return tl_tags;
	}
	
};

void print(vector<char> const &input)
{
	for (int i = 0; i < input.size(); i++) {
		std::cout << input.at(i);
	}
}

void print_tags(vector< vector<char> > const &input)
{
	for (int i = 0; i < input.size(); i++) {
		for(int j = 0; j < input.at(i).size(); j++) {
			cout << input.at(i).at(j);
		}
		cout << " ";
	}
}

int main()
{
	vector<char> inputlu;
	char input_char;
	
	input_char = fgetc(stdin);

	while(input_char != '\n')
	{
		inputlu.push_back(input_char);

		input_char = fgetc(stdin);
	}

	LexicalUnit lu(inputlu);

	cout << "SL: ";
	print(lu.get_sl_form());
	cout << endl << "SL tags: ";
	print_tags(lu.get_sl_tags());
	cout << endl << "TL: ";
	print(lu.get_tl_form());
	cout << endl << "TL tags: ";
	print_tags(lu.get_tl_tags());
	cout << endl;

	return 0;
}



