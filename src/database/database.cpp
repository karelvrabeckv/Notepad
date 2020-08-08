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

#include "database.hpp"
#include "../types/note.hpp"
#include "../types/plaintext/plaintext.hpp"
#include "../types/shoppingcart/shoppingcart.hpp"
#include "../types/tasklist/tasklist.hpp"

//------------------------------------------------

/* the clearCommand method clears the whole database and deallocates all saved notes */
void notesDatabase::clearCommand (void)
{
    if (isDatabaseEmpty())
        return;
    
    for (auto & noteIt : notes)
        delete noteIt;
    notes.clear();
    
    cout << "PROGRAM: Database was cleared." << endl;
} // CLEAR COMMAND

//------------------------------------------------

/* the createCommand method creates a new note of a specific type with user-typed properties */
void notesDatabase::createCommand (void)
{
    string typedName,
           typedTag,
           typedCategory,
           typedType,
           typedFormatted;
    
    //------------------------------------------------
    
    while (cout << "Type a NAME of the note: " && cin >> typedName)
    {
        if (!isFileInDatabase(typedName))
            break;
    } // while
    
    cout << "Type a name of the TAG: ";
    cin >> typedTag;
    
    cout << "Type a name of the CATEGORY: ";
    cin >> typedCategory;
    
    cout << "What KIND of note do you want to create?" << endl
         << "[ShoppingCart | PlainText | TaskList]" << endl;
    
    typedFormatted = NO;
    note* newNote = NULL;
	
    while (cin >> typedType)
    {
        if (typedType == "ShoppingCart")
        {
            newNote = new shoppingCart(typedName, typedTag, typedCategory, typedType, typedFormatted);
            break;
        } // if
        else if (typedType == "PlainText")
        {
            while (cout << "Will the file be FORMATTED? (yes/no): " &&
                   cin >> typedFormatted &&
                   typedFormatted != YES &&
                   typedFormatted != NO)
            {
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << INVALID_VALUE << endl;
            } // while
            newNote = new plainText(typedName, typedTag, typedCategory, typedType, typedFormatted);
            break;
        } // else if
        else if (typedType == "TaskList")
        {
            newNote = new taskList(typedName, typedTag, typedCategory, typedType, typedFormatted);
            break;
        } // else if
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << "PROGRAM: Unknown kind of note." << endl;
            continue;
        } // else
    } // while
    
    cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // clears the I/O buffer
    newNote->createContent(); // creates the note content
    notes.push_back(newNote); // adds a new note to the database
    cout << "PROGRAM: Note successfully created and saved to the database." << endl;
} // CREATE COMMAND

//------------------------------------------------

/* the deleteCommand method deletes and deallocates a specific note */
void notesDatabase::deleteCommand (const string & userInput)
{
    if (isDatabaseEmpty())
        return;
    
    for (auto noteIt = notes.begin(); noteIt != notes.end(); noteIt++)
        if ((*noteIt)->getName() == userInput)
        {
            delete *noteIt;
            notes.erase(noteIt); // removes the note from the vector
            cout << "PROGRAM: The note \"" << userInput << "\" was deleted." << endl;
            return;
        } // if
            
    cout << "PROGRAM: Database does not contain this note." << endl;
} // DELETE COMMAND

//------------------------------------------------

/* the editCommand method enables to customize a specific note */
void notesDatabase::editCommand (string & userInput)
{
    if (isDatabaseEmpty())
        return;

    for (auto & noteIt : notes)
    {
        if (noteIt->getName() == userInput)
        {
            cout << "<EDITOR>=================<ON>" << endl
                 << "Customize your note by /name," << endl
                 << "/tag, /category, /formatted"   << endl
                 << "(only for PlainText) and"      << endl
                 << "/content commands."            << endl
                 << "To leave the editor write"     << endl
                 << "/finish."                      << endl
                 << DOUBLE_LINE                     << endl; // introduction
            showNoteCommand(userInput, EDITOR_ON);
            
			//------------------------------------------------
			
			string userEditorInput;
            while (cin >> userEditorInput)
            {
                if (userEditorInput == "/name" && cin >> userEditorInput)
                {
                    noteIt->setName(userEditorInput);
                    userInput = userEditorInput; // the name has changed, needed for showing the note below
                } // if
                else if (userEditorInput == "/tag" && cin >> userEditorInput)
                    noteIt->setTag(userEditorInput);
                else if (userEditorInput == "/category" && cin >> userEditorInput)
                    noteIt->setCategory(userEditorInput);
                else if (userEditorInput == "/formatted" &&
                         noteIt->getType() == "PlainText" &&
                         cin >> userEditorInput)
                {
                    if (userEditorInput == YES || userEditorInput == NO)
                        noteIt->setFormatted(userEditorInput);
                    else
                    {
                        cout << INVALID_VALUE << endl;
                        continue;
                    } // else
                } // else if
                else if (userEditorInput == "/content")
                {
                    if (!noteIt->changeContent())
                        continue;
                } // else if
                else if (userEditorInput == FINISH)
                {
                    cout << "<EDITOR>================<OFF>" << endl;
                    return;
                } // else if
                else
                {
                    cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                    cout << "PROGRAM: Unknown command for EDITOR." << endl;
                    continue;
                } // else
                    
                showNoteCommand(userInput, EDITOR_ON); // shows the note
            } // while  
        } // if
    } // for
    
    cout << NOTE_NOT_FOUND << endl;
} // EDIT COMMAND

//------------------------------------------------

/* the exportCommand method exports all notes or a specific to a ".pb" file/s */
void notesDatabase::exportCommand (const string & userInput)
{    
    if (!opendir(DIRECTORY))
        mkdir(DIRECTORY, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); // creates a "./notes" folder for all notes

    if (isDatabaseEmpty())
        return;
  
    if (userInput == "all")
        exportAllCommand();
    else if (userInput == "find")
        findExportCommand(FIND_EXPORT);
    else
        exportNoteCommand(userInput);
} // EXPORT COMMAND

//------------------------------------------------

/* the exportAllCommand method exports all notes */
void notesDatabase::exportAllCommand (int method,
                                      vector <note*> *notes_tmp)
{
    ofstream fileOut;
    vector <note*> *ptr = &notes;
    
    //------------------------------------------------
    
    if (method == FIND_EXPORT)
        ptr = notes_tmp;
    
    for (auto & noteIt : *ptr)
    {
        fileOut.open("./notes/" + noteIt->getName() + SUFFIX); // creates and opens the file
        if (!fileOut)
        {
            cout << "PROGRAM: Error while creating an output file." << endl;
            return;
        } // if

        noteIt->exportContent(fileOut); // writes data into the output file
        fileOut.close(); // closes the file
    } // for
    
    cout << "PROGRAM: Note/s successfully exported." << endl;
} // EXPORT ALL COMMAND

//------------------------------------------------

/* the exportNoteCommand method exports a specific note */
void notesDatabase::exportNoteCommand (const string & userInput)
{
    ofstream fileOut;

    for (auto & noteIt : notes)
    {
        if (noteIt->getName() == userInput)
        {
            fileOut.open("./notes/" + noteIt->getName() + SUFFIX); // creates and opens the file
            if (!fileOut)
            {
                cout << "PROGRAM: Error while creating an output file." << endl;
                return;
            } // if

            noteIt->exportContent(fileOut); // writes data into the output file
            fileOut.close(); // closes the file
            cout << "PROGRAM: Note successfully exported." << endl;
            return;
        } // if
    } // for
    
    cout << NOTE_NOT_FOUND << endl;
} // EXPORT NOTE COMMAND

//------------------------------------------------

/* the findExportCommand method finds/exports notes according to some attribute and filters */
void notesDatabase::findExportCommand (int method)
{
    string mode,
	       attribute,
		   filter,
		   confirm;
    vector <note*> notes_tmp = notes; // a copy of "notes" vector
    
    //------------------------------------------------
    
    if (isDatabaseEmpty())
        return;
    
    cout << "Select the searching mode: [General | Previous]"                    << endl
         << "General - searches through all notes in the database"               << endl
         << "Previous - searches through the results of previous searching only" << endl;
    while (!(cin >> mode) ||
           (mode != "General" &&
            mode != "Previous"))
    {
        cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
        cout << "PROGRAM: Invalid mode." << endl;
    } // while
    
    cout << "Type a determining attribute: [Tag | Category | Content]" << endl;
    while (!(cin >> attribute) ||
           (attribute != "Tag" &&
            attribute != "Category" &&
            attribute != "Content"))
    {
		cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
        cout << INVALID_ATTRIBUTE << endl;
    } // while
    
    while (1)
    {
        if (mode == "General") // refreshing the working vector
        {
            notes_tmp.clear();
            notes_tmp = notes;
        } // if
            
        cout << "Type a filter: [Default | AND | OR | Contained | Not_Contained | Percent | Price]" << endl;
        while (!(cin >> filter) ||
               (filter != "Default" &&
                filter != "AND" &&
                filter != "OR" &&
                filter != "Contained" &&
                filter != "Not_Contained" &&
                filter != "Percent" &&
                filter != "Price"))
        {
            if (filter == END)
            {
                notes_tmp.clear();
                cout << "PROGRAM: Finished with finding." << endl;
                return;
            } // else if
			cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << INVALID_FILTER << endl;
        } // while
        
        string str1, str2;
        int value;

        //------------------------------------------------
        
        cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // clears I/O buffer
        if (filter == "Default" ||
            filter == "Contained" ||
            filter == "Not_Contained")
        {
            cout << "Type a string: ";
            getline(cin, str1);
            
            filterForExportFinding(str1, attribute, filter, &notes_tmp);
        } // if
        else if (filter == "AND" ||
                 filter == "OR")
        {
            cout << "Type the first string: ";
            getline(cin, str1);    

            cout << "Type the second string: ";
            getline(cin, str2);
            
            filterForExportFinding(str1, str2, attribute, filter, &notes_tmp);
        } // else if
        else if (filter == "Percent" ||
                 filter == "Price")
        {
            cout << "Type a value: ";
            while (!(cin >> value) ||
                   value < 0 ||
                   (filter == "Percent" && value > 100))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE);
                cout << INVALID_VALUE << endl << "Type a value: ";
            } // while

            cout << "Type a comparator: [Less | Equal | More]" << endl;
            while (!(cin >> str1) ||
                   (str1 != "Less" &&
                    str1 != "Equal" &&
                    str1 != "More"))
            {
				cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << "PROGRAM: Invalid comparator." << endl;
            } // while
            
            filterForExportFinding(str1, value, filter, &notes_tmp);
        } // else if
        
        for (auto & noteIt : notes_tmp) // shows all filtered notes
            noteIt->showContent();
        
        if (notes_tmp.size())
            cout << "PROGRAM: Finished with finding." << endl;
        else
        {
            cout << NO_MATCHES_FOUND << endl;
            continue;
        } // else
        
        //------------------------------------------------
        
        if (method == FIND_EXPORT)
            cout << "Do you really want to export? (yes/no):" << endl;
        else
            continue;
        
        while (!(cin >> confirm) ||
               (confirm != YES &&
                confirm != NO))
        {
			cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << INVALID_ANSWER << endl;
        } // while
        
        //------------------------------------------------
        
        if (confirm == NO)
            continue;
        else if (confirm == YES)
            exportAllCommand(FIND_EXPORT, &notes_tmp);
    } // while
} // FIND COMMAND

//------------------------------------------------

/* the filterForExportFinding method is a searching function used by Default, Contained and Not_Contained filters */
void notesDatabase::filterForExportFinding (const string & str1,
                                            const string & attribute,
                                            const string & filter,
                                            vector <note*> *notes_tmp)
{
    for (auto tmpIt = (*notes_tmp).begin(); tmpIt != (*notes_tmp).end(); tmpIt++)
    {
        if ((filter == "Default"       && attribute == "Tag"      && (*tmpIt)->getTag() == str1) ||
            (filter == "Default"       && attribute == "Category" && (*tmpIt)->getCategory() == str1) ||
            (filter == "Default"       && attribute == "Content"  && (*tmpIt)->doesItContain(str1)) ||
            (filter == "Contained"     && (*tmpIt)->doesItContain(str1)) ||
            (filter == "Not_Contained" && !(*tmpIt)->doesItContain(str1)))
            continue;
        else
            (*notes_tmp).erase(tmpIt--); // deletes the element, important to get iterator back
    } // for
} // FILTER FOR FINDING

//------------------------------------------------

/* the filterForExportFinding method is a searching function used by AND and OR filters */
void notesDatabase::filterForExportFinding (const string & str1,
                                            const string & str2,
                                            const string & attribute,
                                            const string & filter,
                                            vector <note*> *notes_tmp)
{
    for (auto tmpIt = (*notes_tmp).begin(); tmpIt != (*notes_tmp).end(); tmpIt++)
    {
        if ((filter == "AND" && attribute == "Tag"      && (*tmpIt)->getTag() == str1       && (*tmpIt)->getTag() == str2) ||
            (filter == "AND" && attribute == "Category" && (*tmpIt)->getCategory() == str1  && (*tmpIt)->getCategory() == str2) ||
            (filter == "AND" && attribute == "Content"  && (*tmpIt)->doesItContain(str1)    && (*tmpIt)->doesItContain(str2)) ||
            (filter == "OR"  && attribute == "Tag"      && ((*tmpIt)->getTag() == str1      || (*tmpIt)->getTag() == str2)) ||
            (filter == "OR"  && attribute == "Category" && ((*tmpIt)->getCategory() == str1 || (*tmpIt)->getCategory() == str2)) ||
            (filter == "OR"  && attribute == "Content"  && ((*tmpIt)->doesItContain(str1)   || (*tmpIt)->doesItContain(str2))))
            continue;
        else
            (*notes_tmp).erase(tmpIt--); // deletes the element, important to get iterator back
    } // for
} // FILTER FOR FINDING

//------------------------------------------------

/* the filterForExportFinding method is a searching function used by Percent and Price filters */
void notesDatabase::filterForExportFinding (const string & str1,
                                            int value,
                                            const string & filter,
                                            vector <note*> *notes_tmp)
{
    for (auto tmpIt = (*notes_tmp).begin(); tmpIt != (*notes_tmp).end(); tmpIt++)
    {
        if ((filter == "Percent" && (*tmpIt)->checkPercent(str1, value)) ||
            (filter == "Price"   && (*tmpIt)->checkPrice(str1, value)))
            continue;
        else
            (*notes_tmp).erase(tmpIt--); // deletes the element, important to get iterator back
    } // for
} // FILTER FOR FINDING

//------------------------------------------------

/* the findImportCommand method imports notes according to name and filters */
void notesDatabase::findImportCommand (void)
{
    string filter,
		   confirm,
		   str1,
		   str2;
    vector <string> notes_names; // a vector containing names of files (including suffix!) located in the folder
    
    //------------------------------------------------
    
    if (isFolderEmpty())
        return;
    
    while (1)
    {
        cout << "Type a filter: [AND | OR]" << endl;
        while (!(cin >> filter) ||
               (filter != "AND" &&
                filter != "OR"))
        {
            if (filter == END)
            {
                notes_names.clear();
                cout << "PROGRAM: Finished with finding." << endl;
                return;
            } // else if
			cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << INVALID_FILTER << endl;
        } // while

        cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE);

		cout << "Type the first string: ";
		getline(cin, str1);    

		cout << "Type the second string: ";
		getline(cin, str2);

        //------------------------------------------------
        
        DIR* dirIn = opendir(DIRECTORY); // opens the directory
        struct dirent* file; // pointer to a structure symbolizing a file

        while ((file = readdir(dirIn)) != NULL) // passes the folder and checks all .pb files
        {
            string filename(file->d_name); // char* to string
            if (filename.find(SUFFIX, filename.length() - 3) != string::npos) // searches for string ".pb" at the end of the file name
            {
                if ((filter == "AND" && filename == str1 && filename == str2) ||
                    (filter == "OR" && (filename == str1 || filename == str2)))
                    notes_names.push_back(filename);
                else
                    continue;
            } // if
        } // while
        
        closedir(dirIn); // close the directory
        
        //------------------------------------------------
        
        for (auto & noteIt : notes_names) // shows all filtered notes
            cout << noteIt << " ";
        
        if (notes_names.size())
            cout << endl << "PROGRAM: Finished with finding." << endl;
        else
        {
            cout << NO_MATCHES_FOUND << endl;
            continue;
        } // else

        cout << "Do you really want to import? (yes/no): ";
        while (!(cin >> confirm) ||
               (confirm != YES &&
                confirm != NO))
        {
			cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << INVALID_ANSWER << endl;
        } // while
        
        if (confirm == NO)
        {
            notes_names.clear();
            continue;
        } // if
        else if (confirm == YES)
        {
            importAllCommand(FIND_IMPORT, &notes_names);
            notes_names.clear();
            return;
        } // else if
    } // while
} // FIND COMMAND

//------------------------------------------------

/* the helpCommand method shows all commands and their significance */
void notesDatabase::helpCommand (void)
{
	cout << DOUBLE_LINE << endl
	     << "/clear - clears the database" << endl
		 << "/create - creates a new note" << endl
		 << "/delete - deletes a specific note from the database" << endl
		 << "/edit - starts the EDITOR" << endl
		 << "| /name - sets a new note name" << endl
		 << "| /tag - sets a new note tag" << endl
		 << "| /category - sets a new note category" << endl
		 << "| /formatted - sets whether the note is formatted or not" << endl
		 << "| /content - starts the CONTENT EDITOR" << endl
		 << "  | /add - adds item/task" << endl
		 << "  | /delete - deletes item/task" << endl
		 << "  | /change - changes item/task" << endl
		 << "  | /finish - leaves the CONTENT EDITOR" << endl
		 << "| /finish - leaves the EDITOR" << endl
		 << "/end - exits the program or finishes writing content/item/task etc." << endl
		 << "/export - exports some note/s to the NOTES directory" << endl
		 << "/find - finds notes in the database using filters" << endl
		 << "/import - imports some note/s from the NOTES directory" << endl
		 << "/show - shows some note/s from the database" << endl
		 << "/sort - sorts notes from the database" << endl
		 << DOUBLE_LINE << endl;
} // HELP COMMAND

//------------------------------------------------

/* the checkFormat method checks whether the imported file has correct formatting of a ".pb" file */
bool notesDatabase::checkFormat (const string & file,
                                 int method)
{
    ifstream fileIn;
    string tmp,
           savedName,
           savedTag,
           savedCategory,
           savedType,
           savedFormatted;
    unsigned i = 0;
    
    //------------------------------------------------

    fileIn.open("./notes/" + file + SUFFIX);
    
    while (getline(fileIn, tmp))
    {
        switch (i++)
        {
            case 0:
                if (!tmp.find("NAME: "))
                {
                    tmp.erase(0, 6);
                    savedName = tmp;
                    if (savedName != file) // name matching
                    {
                        cout << "PROGRAM: Name matching error." << endl;
                        return false;
                    } // if
                } // if
                else
                {
                    cout << "PROGRAM: Format error while reading NAME." << endl;
                    return false;
                } // else
                break;
                
            case 1:
                if (!tmp.find("TAG: "))
                {
                    tmp.erase(0, 5);
                    savedTag = tmp;
                } // if
                else
                {
                    cout << "PROGRAM: Format error while reading TAG." << endl;
                    return false;
                } // else
                break;
                
            case 2:
                if (!tmp.find("CATEGORY: "))
                {
                    tmp.erase(0, 10);
                    savedCategory = tmp;
                } // if
                else
                {
                    cout << "PROGRAM: Format error while reading CATEGORY." << endl;
                    return false;
                } // else
                break;
                
            case 3:
                if (!tmp.find("TYPE: "))
                {
                    tmp.erase(0, 6);
                    savedType = tmp;
                } // if
                else
                {
                    cout << "PROGRAM: Format error while reading TYPE." << endl;
                    return false;
                } // else
                break;
                
            case 4:
                if (!tmp.find("FORMATTED: "))
                {
                    tmp.erase(0, 11);
                    savedFormatted = tmp;
                    if (savedFormatted != YES &&
                        savedFormatted != NO)
                    {
                        cout << "PROGRAM: Invalid value in FORMATTED." << endl;
                        return false;
                    } // if
                } // if
                else
                {
                    cout << "PROGRAM: Format error while reading FORMATTED." << endl;
                    return false;
                } // else
                break;
        } // switch
        if (i >= 5) // stops cycle for importContent() that reads the following content
            break;
    } // while
    
    note* newNote = NULL;
    if (savedType == "ShoppingCart")
        newNote = new shoppingCart(savedName, savedTag, savedCategory, savedType, savedFormatted);
    else if (savedType == "PlainText")
        newNote = new plainText(savedName, savedTag, savedCategory, savedType, savedFormatted);
    else if (savedType == "TaskList")
        newNote = new taskList(savedName, savedTag, savedCategory, savedType, savedFormatted);
    else
    {
        cout << "PROGRAM: Invalid value in TYPE." << endl;
        return false;
    } // else

    if (!newNote->importContent(fileIn)) // imports content from the file
        return false;

    notes.push_back(newNote); // adds a new note to the database
    fileIn.close(); // closes the file
    return true;
} // CHECK FORMAT

//------------------------------------------------

/* the importCommand method imports all notes or a specific to the database */
void notesDatabase::importCommand (const string & userInput)
{
    if (!opendir(DIRECTORY))
        mkdir(DIRECTORY, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); // creates a "./notes" folder for all notes

    if (isFolderEmpty())
        return;
    
    if (userInput == "all")
        importAllCommand();
    else if (userInput == "find")
        findImportCommand();
    else
        importNoteCommand(userInput);
} // IMPORT COMMAND

//------------------------------------------------

/* the importAllCommand method imports all notes to the database */
void notesDatabase::importAllCommand (int method,
                                      vector <string> *ptr)
{   
    DIR* dirIn = opendir(DIRECTORY); // opens the directory
    struct dirent* file; // pointer to a structure symbolizing a file
    
    while ((file = readdir(dirIn)) != NULL) // passes the folder and saves all ".pb" files
    {
        string filename(file->d_name); // char* to string
        if (filename.find(SUFFIX, filename.length() - 3) != string::npos) // searches for string ".pb" at the end of the file name
        {
            if (method == FIND_IMPORT &&
                find((*ptr).begin(), (*ptr).end(), filename) == (*ptr).end()) // import influenced by finding
                continue;
            filename.erase(filename.length() - 3, 3); // deletes the suffix
            if (isFileInDatabase(filename))
                continue;
            if (checkFormat(filename, method))
                cout << "PROGRAM: File \"" << filename << SUFFIX << "\" successfully imported." << endl;
        } // if
    } // while
    
    closedir(dirIn); // closes the directory
    cout << "PROGRAM: Finished with importing." << endl;
} // IMPORT ALL COMMAND

//------------------------------------------------

/* the importNoteCommand method imports a specific note to the database */
void notesDatabase::importNoteCommand (const string & userInput)
{
    DIR* dirIn = opendir(DIRECTORY); // opens the directory
    struct dirent* file; // pointer to a structure symbolizing a file
    
    while ((file = readdir(dirIn)) != NULL) // passes the folder and saves all ".pb" files
    {
        string filename(file->d_name); // char* to string
        if (filename.find(SUFFIX, filename.length() - 3) != string::npos && // searches for ".pb" at the end of the file name
            filename == userInput)
        {
            filename.erase(filename.length() - 3, 3); // deletes the suffix
            if (isFileInDatabase(filename))
                return;
            if (checkFormat(filename))
            {
                cout << "PROGRAM: File \"" << filename << SUFFIX << "\" successfully imported." << endl
                     << "PROGRAM: Finished with importing." << endl;
                return;
            } // if
        } // if
    } // while
    
    closedir(dirIn); // close the directory
    cout << "PROGRAM: Cannot import." << endl;
} // IMPORT NOTE COMMAND

//------------------------------------------------

/* the isDatabaseEmpty method tests whether the database is empty or not */
bool notesDatabase::isDatabaseEmpty (void) const
{
    if (!notes.size())
    {
        cout << "PROGRAM: Database is empty." << endl;
        return true;
    } // if
    
    return false;
} // IS DATABASE EMPTY

//------------------------------------------------

/* the isFileInDatabase method tests whether the file is already in the database or not */
bool notesDatabase::isFileInDatabase (const string & file) const
{
    for (auto & noteIt : notes)
        if (noteIt->getName() == file)
        {
            cout << "PROGRAM: Database already contains the note name \"" << file << "\"." << endl;
            return true;
        } // if

    return false;
} // IS FILE IN DATABASE

//------------------------------------------------

/* the isFolderEmpty method tests whether the folder is empty or not */
bool notesDatabase::isFolderEmpty (void) const
{
    DIR* dirIn = opendir(DIRECTORY);
    struct dirent* file;
    unsigned n = 0;
    
    while ((file = readdir(dirIn)) != NULL)
        n++;

    closedir(dirIn);
    
    if (n <= 2) // empty folder -> only "." and ".."
    {
        cout << "PROGRAM: Folder is empty." << endl;
        return true;    
    } // if

    return false;
} // IS FOLDER EMPTY

//------------------------------------------------

/* the showCommand method shows the content of note/s or the database */
void notesDatabase::showCommand (const string & userInput)
{
    if (isDatabaseEmpty())
        return;
        
    if (userInput == "database")
        showDatabaseCommand();
    else if (userInput == "all")
        showAllCommand();
    else
        showNoteCommand(userInput);
} // SHOW COMMAND

//------------------------------------------------

/* the showAllCommand method shows the content of all notes */
void notesDatabase::showAllCommand (void)
{
    for (auto & noteIt : notes)
        noteIt->showContent();
} // SHOW ALL COMMAND

//------------------------------------------------

/* the showDatabaseCommand method shows the content of the database */
void notesDatabase::showDatabaseCommand (void)
{
    cout << DOUBLE_LINE                                          << endl
         << "[NAME] | [TAG] | [CATEGORY] | [TYPE] | [FORMATTED]" << endl
         << DOUBLE_LINE                                          << endl;
    
    for (auto & noteIt : notes)
        cout << noteIt->getName()      << " | "
             << noteIt->getTag()       << " | "
             << noteIt->getCategory()  << " | "
             << noteIt->getType()      << " | "
             << noteIt->getFormatted() << endl;
    
    cout << DOUBLE_LINE << endl;   
} // SHOW DATABASE COMMAND

//------------------------------------------------

/* the showNoteCommand method shows the content of a specific note */
void notesDatabase::showNoteCommand (const string & userInput,
                                     int editor)
{
    for (auto & noteIt : notes)
        if (noteIt->getName() == userInput)
        {
            noteIt->showContent(editor);
            return;
        } // if
    
    cout << NOTE_NOT_FOUND << endl;
} // SHOW NOTE COMMAND

//------------------------------------------------

/* the decreaseSort function sorts the notes according to some attribute alphabetically from the highest to the lowest */
template <string (note::*ptr)()>
bool decreaseSort (note* x,
                   note* y) {return (x->*ptr)() > (y->*ptr)();} // DECREASE SORT

//------------------------------------------------

/* the increaseSort function sorts the notes according to some attribute alphabetically from the lowest to the highest */
template <string (note::*ptr)()>
bool increaseSort (note* x,
                   note* y) {return (x->*ptr)() < (y->*ptr)();} // INCREASE SORT

//------------------------------------------------

/* the sortCommand method sorts the notes according to some attribute and filter */
void notesDatabase::sortCommand (void)
{
    string attribute,
           filter;
    
    //------------------------------------------------

    if (isDatabaseEmpty())
        return;

    cout << "Type a determining attribute: [Name | Tag | Category | Type | Formatted]" << endl;
    while (!(cin >> attribute) ||
           (attribute != "Name" &&
            attribute != "Tag" &&
            attribute != "Category" &&
            attribute != "Type" &&
            attribute != "Formatted"))
    {
        cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
        cout << INVALID_ATTRIBUTE << endl;
    } // while

    cout << "Type a filter: [Increase | Decrease]" << endl;
    while (!(cin >> filter) ||
           (filter != "Increase" &&
            filter != "Decrease"))
    {
        cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
        cout << INVALID_FILTER << endl;
    } // while
    
    if (attribute == "Name" && filter == "Decrease")
        sort(notes.begin(), notes.end(), decreaseSort<&note::getName>);
    else if (attribute == "Name" && filter == "Increase")
        sort(notes.begin(), notes.end(), increaseSort<&note::getName>);
    else if (attribute == "Tag" && filter == "Decrease")
        sort(notes.begin(), notes.end(), decreaseSort<&note::getTag>);
    else if (attribute == "Tag" && filter == "Increase")
        sort(notes.begin(), notes.end(), increaseSort<&note::getTag>);
    else if (attribute == "Category" && filter == "Decrease")
        sort(notes.begin(), notes.end(), decreaseSort<&note::getCategory>);
    else if (attribute == "Category" && filter == "Increase")
        sort(notes.begin(), notes.end(), increaseSort<&note::getCategory>);
    else if (attribute == "Type" && filter == "Decrease")
        sort(notes.begin(), notes.end(), decreaseSort<&note::getType>);
    else if (attribute == "Type" && filter == "Increase")
        sort(notes.begin(), notes.end(), increaseSort<&note::getType>);
    else if (attribute == "Formatted" && filter == "Decrease")
        sort(notes.begin(), notes.end(), decreaseSort<&note::getFormatted>);
    else if (attribute == "Formatted" && filter == "Increase")
        sort(notes.begin(), notes.end(), increaseSort<&note::getFormatted>);
    
    showDatabaseCommand();
    cout << "PROGRAM: Successfully sorted." << endl;
} // SORT COMMAND
