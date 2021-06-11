/*
* Copyright (C) 2019 Apertium Project Management Committee <apertium-pmc@dlsi.ua.es>,
*               2019 Tanmai Khanna <khanna.tanmai@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _PARSEBILTRANS_
#define _PARSEBILTRANS_

#include <vector>
#include <string>
#include <lttoolbox/ustring.h>

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
	UString sl_form;

	/**
   	 * Target language word and tags
     */
	UString tl_form;

	/**
   	 * Source language tags
     */
	vector< UString > sl_tags;

	/**
   	 * Target language tags
     */
	vector< UString > tl_tags;

  /**
      * Source language lemma
     */
  UString sl_lemma;

  /**
      * Target language lemma
     */
  UString tl_lemma;

public:
	/**
   	 * Constructor to fill all variables
   	 * @param input_LU one lexical unit between ^ and $ (excluded)
     */
	ParseLexicalUnit(UString input_LU);

	/**
   	 * Return the Source Language Form
     */
	UString get_sl_form();

	/**
   	 * Return the Target Language Form
     */
	UString get_tl_form();

	/**
   	 * Return the Source Language Tags
     */
	vector< UString > get_sl_tags();

	/**
   	 * Return the Target Language Form
     */
	vector< UString > get_tl_tags();

  /**
      * Return the Source Language Lemma
     */
  UString get_sl_lemma();

  /**
      * Return the Target Language Lemma
     */
  UString get_tl_lemma();

};

#endif
