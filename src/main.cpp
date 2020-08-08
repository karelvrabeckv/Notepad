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

#include "constants.hpp"
#include "database/database.hpp"
#include "types/note.hpp"
#include "types/plaintext/plaintext.hpp"
#include "types/shoppingcart/shoppingcart.hpp"
#include "types/tasklist/tasklist.hpp"

using namespace std;

//------------------------------------------------

/** @mainpage
  * Notepad - The Program For Note Management<br>
  * ============================<br>
  * <br>
  * Name: Notepad (EN) / Poznámkový blok (CZ)<br>
  * Subject: BI-PA2<br>
  * Author: Karel Vrabec (vrabekar)<br>
  * School: FIT ČVUT<br>
  */
  
int main (void)
{
    notesDatabase nD; // a database of all notes
    
    //------------------------------------------------
    
    cout << "===<WELCOME TO MY NOTEPAD>===" << endl
         << "Create, edit, delete, show,"   << endl
		 << "find, sort, import and export" << endl
         << "your notes as you wish."       << endl
		 << "You can find more information" << endl
		 << "here: /help                  " << endl
         << DOUBLE_LINE                     << endl; // introduction
    
    //------------------------------------------------
    
    string userInput; // for commands typed by a user
    while (cin >> userInput)
    {
        if (userInput == "/clear")
            nD.clearCommand(); // clears the database
        else if (userInput == "/create")
            nD.createCommand(); // creates a new note
        else if (userInput == DELETE && cin >> userInput)
            nD.deleteCommand(userInput); // deletes a specific note
        else if (userInput == "/edit" && cin >> userInput)
            nD.editCommand(userInput); // edits a specific note
        else if (userInput == END)
            break; // ends this cycle and program
        else if (userInput == "/export" && cin >> userInput)
            nD.exportCommand(userInput); // exports note/s
        else if (userInput == "/find")
            nD.findExportCommand(); // finds notes
        else if (userInput == "/help")
            nD.helpCommand(); // shows all commands and their significance
        else if (userInput == "/import" && cin >> userInput)
            nD.importCommand(userInput); // imports note/s
        else if (userInput == "/show" && cin >> userInput)
            nD.showCommand(userInput); // shows note/s or database
        else if (userInput == "/sort")
            nD.sortCommand(); // sorts the notes
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << "PROGRAM: Unknown command." << endl; // other inputs
        } // else
    } // while
    
    return 0;
} // MAIN
