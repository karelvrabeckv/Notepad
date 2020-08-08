#pragma once

#include "../constants.hpp"
#include "../types/note.hpp"

//////////////////////////////////////////////////
// NOTES DATABASE
//////////////////////////////////////////////////

/** NOTES DATABASE is a database of ALL NOTES */
class notesDatabase
{
    private:
        
		/** the database of pointers to all notes */
        vector <note*> notes;
        
    public:
        
        ~notesDatabase (void)
        {
            for (auto & noteIt : notes)
                delete noteIt;
        } // DESTRUCTOR
        
		/** the clearCommand method clears the whole database and deallocates all saved notes */
        void clearCommand  (void);
		/** the createCommand method creates a new note of a specific type with user-typed properties */
        void createCommand (void);
		/** the deleteCommand method deletes and deallocates a specific note */
        void deleteCommand (const string & userInput);
		/** the editCommand method enables to customize a specific note */
        void editCommand   (string & userInput);
        
		/** the exportCommand method exports all notes or a specific to a ".pb" file/s */
        void exportCommand (const string & userInput);
            /** the exportAllCommand method exports all notes */
			void exportAllCommand  (int method = FIND_DEFAULT,
                                    vector <note*> *notes_tmp = NULL);
			/** the exportNoteCommand method exports a specific note */
            void exportNoteCommand (const string & userInput);

		/** the findExportCommand method finds/exports notes according to some attribute and filters */
        void findExportCommand (int method = FIND_DEFAULT);
			/** the filterForExportFinding method is a searching function used by Default, Contained and Not_Contained filters */
            void filterForExportFinding (const string & str1,
                                         const string & attribute,
                                         const string & filter,
                                         vector <note*> *notes_tmp);
            /** the filterForExportFinding method is a searching function used by AND and OR filters */
			void filterForExportFinding (const string & str1,
                                         const string & str2,
                                         const string & attribute,
                                         const string & filter,
                                         vector <note*> *notes_tmp);
            /** the filterForExportFinding method is a searching function used by Percent and Price filters */
			void filterForExportFinding (const string & str1,
                                         int value,
                                         const string & filter,
                                         vector <note*> *notes_tmp);
        
		/** the findImportCommand method imports notes according to name and filters */
        void findImportCommand(void);
		/** the helpCommand method shows all commands and their significance */
        void helpCommand (void);
		/** the checkFormat method checks whether the imported file has correct formatting of a ".pb" file */
        bool checkFormat (const string & file,
                          int method = FIND_DEFAULT);
		
		/** the importCommand method imports all notes or a specific to the database */
        void importCommand (const string & userInput);
            /** the importAllCommand method imports all notes to the database */
			void importAllCommand  (int method = FIND_DEFAULT,
                                    vector <string> *ptr = NULL);
            /** the importNoteCommand method imports a specific note to the database */
			void importNoteCommand (const string & userInput);
		
		/** the isDatabaseEmpty method tests whether the database is empty or not */
        bool isDatabaseEmpty  (void) const;
        /** the isFileInDatabase method tests whether the file is already in the database or not */
		bool isFileInDatabase (const string & file) const;
        /** the isFolderEmpty method tests whether the folder is empty or not */
		bool isFolderEmpty    (void) const;
		
		/** the showCommand method shows the content of note/s or the database */
        void showCommand (const string & userInput);
			/** the showAllCommand method shows the content of all notes */
            void showAllCommand      (void);
			/** the showDatabaseCommand method shows the content of the database */
            void showDatabaseCommand (void);
			/** the showNoteCommand method shows the content of a specific note */
            void showNoteCommand     (const string & userInput,
                                      int editor = EDITOR_OFF);

		/** the sortCommand method sorts the notes according to some attribute and filter */
        void sortCommand (void);
			/** the decreaseSort function sorts the notes according to some attribute alphabetically from the highest to the lowest */
            friend bool decreaseSort (note* x,
                                      note* y);
			/** the increaseSort function sorts the notes according to some attribute alphabetically from the lowest to the highest */
            friend bool increaseSort (note* x,
                                      note* y);
        
}; // NOTES DATABASE
