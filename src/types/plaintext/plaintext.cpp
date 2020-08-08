#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <stack>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>

#include "plaintext.hpp"
#include "../../database/database.hpp"

//------------------------------------------------

/* the createContent method creates the content specific for PLAINTEXT */
void plainText::createContent (void)
{
    string tmp;
    unsigned pos;

    //------------------------------------------------
    
    cout << "CONTENT:" << endl;
    while (getline(cin, tmp))
    {   
        if (tmp.find(END) != string::npos) // looks for "/end"
        {
            pos = tmp.find(END); // sets a position of "/end"
            tmp.erase(pos, 4); // deletes "/end" from the string
            content += tmp; // adds the line without "/end" to content
            break;
        } // if                 
        content += tmp + NEW_LINE;
    } // while
} // CREATE CONTENT

//------------------------------------------------

/* the exportContent method exports content specific for PLAINTEXT */
void plainText::exportContent (ofstream & fileOut)
{
    fileOut << "NAME: "      << name            << endl
            << "TAG: "       << tag             << endl
            << "CATEGORY: "  << category        << endl
            << "TYPE: "      << type            << endl
            << "FORMATTED: " << formatted       << endl
            << "CONTENT: "   << endl << content << endl;
} // EXPORT CONTENT

//------------------------------------------------

/* the changeContent method changes the content of a PLAINTEXT note */
bool plainText::changeContent (void)
{
    string oldStr, newStr;
    unsigned pos;
    
    //------------------------------------------------
    
    cout << "Write the text you want to change: ";
    cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // clears the I/O buffer
    getline(cin, oldStr);

    if (oldStr == "/all") // replaces the whole content
    {
        content = "";
        cout << "Write a new text to replace: " << endl;
        createContent();
        return true;
    } // if
    
    if (content.find(oldStr) != string::npos) // replaces a part of the content
    {
        pos = content.find(oldStr); // sets a position of the old text
        content.erase(pos, oldStr.length()); // deletes the old text from the string        
        
        cout << "Write a new text to replace: ";
        getline(cin, newStr);
        content.insert(pos, newStr);
        return true;
    } // if
    
    cout << "PROGRAM: These characters are not contained in the text." << endl;
    return false;
} // CHANGE CONTENT

//------------------------------------------------

/* the importContent method imports content specific for PLAINTEXT */
bool plainText::importContent (ifstream & fileIn)
{
    string tmp;
    
    //------------------------------------------------
    
    getline(fileIn, tmp);
    if (tmp.find("CONTENT: "))
    {
        cout << "PROGRAM: Format error while reading CONTENT." << endl;
        return false;
    } // if
    
    while (getline(fileIn, tmp))
        content += tmp + NEW_LINE;
    content.erase(content.length() - 1, 1); // removes the last '\n'
    
    return true;
} // IMPORT CONTENT

//------------------------------------------------

/* the showContent method shows the content of PLAINTEXT note */
void plainText::showContent (int editor)
{
    cout << SINGLE_LINE                << endl
         << "NAME: "      << name      << endl
         << "TAG: "       << tag       << endl
         << "CATEGORY: "  << category  << endl
         << "TYPE: "      << type      << endl
         << "FORMATTED: " << formatted << endl
         << "CONTENT: "                << endl
         << ARROW_LINE                 << endl;
    
    string markdownContent = content;
    if (formatted == YES &&
        editor == EDITOR_OFF)
    {
        markdownListHeaderQuote(markdownContent); // transforms lists, headers, quotes
        markdownLinkImage(markdownContent); // transforms images, links
        if (markdownText(markdownContent)) // transforms text
            cout << markdownContent << endl;
    }
    else
        cout << content << endl;
    
    cout << ARROW_LINE  << endl
         << SINGLE_LINE << endl;
} // SHOW CONTENT

//------------------------------------------------

/* the doesItContain method tests whether it contains some string */
bool plainText::doesItContain (const string & str)
{
    if (content.find(str) != string::npos)
        return true;

    return false;
} // DOES IT CONTAIN

//------------------------------------------------

/* the markdownListHeaderQuote method transforms lists, headers and quotes to a formatted text */
void plainText::markdownListHeaderQuote (string & markdownContent) const
{
    vector <string> tags;
    size_t pos, len, newline, start, i;
    
    //------------------------------------------------
    
    tags.push_back("\n*");
    tags.push_back("\n-");
    tags.push_back("\n>");
    tags.push_back("######");
    tags.push_back("#####");
    tags.push_back("####");
    tags.push_back("###");
    tags.push_back("##");
    tags.push_back("#");
    
    for (auto & chanIt : tags) // goes through all the tags
    {
        pos = len = newline = start = 0;
        while (pos != string::npos) // goes until the end of the source string
        {
            pos = markdownContent.find(chanIt, start); // the first occurrence
            if (pos == string::npos)
                break;
            
            if (chanIt == "\n*" ||
                chanIt == "\n-")
                markdownContent.replace(pos, 2, "\n +");
            else if (chanIt == "\n>")
                markdownContent.replace(pos, 2, "\n |");
            else if (chanIt == "######" ||
                     chanIt == "#####" ||
                     chanIt == "####" ||
                     chanIt == "###" ||
                     chanIt == "##" ||
                     chanIt == "#")
            {
                if (pos != 0 && markdownContent.at(pos - 1) != NEW_LINE)
                {
                    start = pos + chanIt.size();
                    continue;
                } // if
                    
                len = chanIt.size(); // length of the tag
                markdownContent.erase(pos, chanIt.size()); // deletes the tag
                newline = markdownContent.find(NEW_LINE, pos + 1); // finds next \n
                
                if (newline == string::npos)
                {
                    newline = markdownContent.size() - 1; // newline as the last character
                    markdownContent.insert(newline + 1, "\n"); // gives \n behind the last character
                    newline++; // sets new line at the last character which is \n
                } // if

                for (i = 0; i < (7 - len); i++)
                    markdownContent.insert(newline + 1, "="); // gives multiple = behind \n
                
                markdownContent.insert(newline + i + 1, "\n"); // gives \n behind =
            } // else if  
        } // while
    } // for
} // MARKDOWN LIST HEADER QUOTE

//------------------------------------------------

/* the markdownLinkImage method transforms links and images to a formatted text */
void plainText::markdownLinkImage (string & markdownContent) const
{
    vector <string> tags;
    size_t start, middle, end;
    
    //------------------------------------------------
    
    tags.push_back("![");
    tags.push_back("[");
    
    for (auto & chanIt : tags) // goes through all the tags
    {
        start = middle = end = 0;
        while (start != string::npos) // goes until the end of the source string
        {
            start = markdownContent.find(chanIt, 0);
            middle = markdownContent.find("](", start + 1);
            end = markdownContent.find(")", middle + 1);
            
            if (start == string::npos ||
                middle == string::npos ||
                end == string::npos)
                break;
            else
            {
                if (chanIt == "![")
                    markdownContent.replace(start, end - start + 1, "<IMAGE>");
                else
                    markdownContent.replace(start, end - start + 1, "<LINK>");
            } // if
        } // while
    } // for
} // MARKDOWN LINK IMAGE

//------------------------------------------------

/* the markdownText method transforms unformatted text to a formatted */
bool plainText::markdownText (string & markdownContent) const
{
    stack <size_t> positions;
    vector <string> tags;
    size_t pos;
    
    //------------------------------------------------
    
    tags.push_back("**");
    tags.push_back("*");
    tags.push_back("__");
    tags.push_back("_");

    for (auto & chanIt : tags) // goes through all the tags
    {
        pos = markdownContent.find(chanIt, 0); // finds the first tag occurrence
        while (pos != string::npos) // goes until the end of the source string
        {
            positions.push(pos); // gets a position of the tag to the stack

            if (chanIt == "**" ||
                chanIt == "__")
            {
                markdownContent.replace(positions.top(), 2, "<bold>"); // replaces the first tag
                pos = markdownContent.find(chanIt, positions.top() + 1); // finds the second tag occurrence
                if (pos == string::npos)
                {
                    cout << "PROGRAM: Syntax error -> missing end tag to \"" << chanIt << "\"."<< endl;
                    return false;
                } // if
                markdownContent.replace(pos, 2, "</bold>");
            } // if
            else if (chanIt == "*" ||
                     chanIt == "_")
            {
                markdownContent.replace(positions.top(), 1, "<italic>");
                pos = markdownContent.find(chanIt, positions.top() + 1); // finds the second tag occurrence
                if (pos == string::npos)
                {
                    cout << "PROGRAM: Syntax error -> missing end tag to \"" << chanIt << "\"."<< endl;
                    return false;
                }
                markdownContent.replace(pos, 1, "</italic>");
            } // else
            
            positions.pop(); // cleans the top of the stack
            pos = markdownContent.find(chanIt, pos + 1); // finds another first tag occurrence
        } // while
    } // for
    
    return true;
} // MARKDOWN TEXT
