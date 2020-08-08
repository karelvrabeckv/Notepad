#pragma once

#include "../../constants.hpp"
#include "../../database/database.hpp"

//////////////////////////////////////////////////
// PLAIN TEXT
//////////////////////////////////////////////////

/** PLAIN TEXT is a NOTE TYPE and CLASS DESCENDANT of NOTE */
class plainText : public note
{
    private:

        string content;
    
    public:

        plainText (const string & typedName,
                   const string & typedTag,
                   const string & typedCategory,
                   const string & typedType,
                   const string & typedFormatted)
        {
            name = typedName;
            tag = typedTag;
            category = typedCategory;
            type = typedType;
            formatted = typedFormatted;
        } // CONSTRUCTOR
        
        virtual ~plainText (void) {};
        
		/** the createContent method creates the content specific for PLAINTEXT */
        virtual void createContent (void);
		/** the exportContent method exports content specific for PLAINTEXT */
        virtual void exportContent (ofstream & fileOut);
		/** the changeContent method changes the content of a PLAINTEXT note */
        virtual bool changeContent (void);
		/** the importContent method imports content specific for PLAINTEXT */
        virtual bool importContent (ifstream & fileIn);
		/** the showContent method shows the content of PLAINTEXT note */
        virtual void showContent   (int editor = EDITOR_OFF);
        
		/** the doesItContain method tests whether it contains some string */
        virtual bool doesItContain (const string & str);

        /** the markdownListHeaderQuote method transforms lists, headers and quotes to a formatted text */
        void markdownListHeaderQuote (string & markdownContent) const;
		/** the markdownLinkImage method transforms links and images to a formatted text */
        void markdownLinkImage       (string & markdownContent) const;
		/** the markdownText method transforms unformatted text to a formatted */
        bool markdownText            (string & markdownContent) const;
        
}; // PLAIN TEXT
