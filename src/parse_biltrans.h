#ifndef _PARSEBILTRANS_
#define _PARSEBILTRANS_

#include<vector>
#include<string>

using namespace std;

/**
 * Parsing Lexical Unit from biltrans for the Anaphora Module
 */
class ParseLexicalUnit
{

private:
	/**
   	 * Source language word and tags
     */
	wstring sl_form;

	/**
   	 * Target language word and tags
     */
	wstring tl_form;

	/**
   	 * Source language tags
     */
	vector< wstring > sl_tags;

	/**
   	 * Target language tags
     */
	vector< wstring > tl_tags;

public:
	/**
   	 * Constructor to fill all variables
   	 * @param input_LU one lexical unit between ^ and $ (excluded)
     */
	ParseLexicalUnit(wstring input_LU);

	/**
   	 * Return the Source Language Form
     */
	wstring get_sl_form();

	/**
   	 * Return the Target Language Form
     */
	wstring get_tl_form();

	/**
   	 * Return the Source Language Tags
     */
	vector< wstring > get_sl_tags();

	/**
   	 * Return the Target Language Form
     */
	vector< wstring > get_tl_tags();

};

#endif