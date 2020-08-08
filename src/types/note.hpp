#pragma once

#include "../constants.hpp"

using namespace std;

//------------------------------------------------

//////////////////////////////////////////////////
// NOTE
//////////////////////////////////////////////////

/** NOTE is a BASE CLASS inherited by PLAINTEXT, SHOPPINGCART and TASKLIST */
struct note
{
    
	protected:
	
		string name,
			   tag,
			   category,
			   type,
			   formatted;

	public:
			   
		virtual ~note (void) {};

		/** pure virtual methods used for working with note content */
		virtual void createContent (void)                    = 0;
		virtual void exportContent (ofstream & fileOut)      = 0;
		virtual bool changeContent (void)                    = 0;
		virtual bool importContent (ifstream & fileIn)       = 0;
		virtual void showContent   (int editor = EDITOR_OFF) = 0;

		/** other virtual methods */
		virtual bool doesItContain (const string & str)      = 0;
		virtual bool checkPercent  (const string & str,
									int value)               {return false;}
		virtual bool checkPrice    (const string & str,
									int value)               {return false;}

		/** setters of note */
		void setName      (const string & userEditorInput) {name      = userEditorInput;}
		void setTag       (const string & userEditorInput) {tag       = userEditorInput;}
		void setType      (const string & userEditorInput) {type      = userEditorInput;}
		void setCategory  (const string & userEditorInput) {category  = userEditorInput;}
		void setFormatted (const string & userEditorInput) {formatted = userEditorInput;}

		/** getters of note */
		string getName      (void) {return name;}
		string getTag       (void) {return tag;}
		string getType      (void) {return type;}
		string getCategory  (void) {return category;}
		string getFormatted (void) {return formatted;}
		
}; // NOTE
