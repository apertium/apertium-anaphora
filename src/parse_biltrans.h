#ifndef _PARSEBILTRANS_
#define _PARSEBILTRANS_

#include<vector>

using namespace std;

/**
 * Parsing Lexical Unit from biltrans for the Anaphora Module
 */
class LexicalUnit
{

private:
	/**
   	 * Source language word and tags
     */
	vector<char> sl_form;

	/**
   	 * Target language word and tags
     */
	vector<char> tl_form;

	/**
   	 * Source language tags
     */
	vector< vector<char> > sl_tags;

	/**
   	 * Target language tags
     */
	vector< vector<char> > tl_tags;

public:
	/**
   	 * Constructor to fill all variables
   	 * @param input_LU one lexical unit between ^ and $ (excluded)
     */
	LexicalUnit(vector<char> input_LU);

	/**
   	 * Return the Source Language Form
     */
	vector<char> get_sl_form();

	/**
   	 * Return the Target Language Form
     */
	vector<char> get_tl_form();

	/**
   	 * Return the Source Language Tags
     */
	vector< vector<char> > get_sl_tags();

	/**
   	 * Return the Target Language Form
     */
	vector< vector<char> > get_tl_tags();

};

#endif