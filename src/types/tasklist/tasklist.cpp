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

#include "tasklist.hpp"
#include "../../database/database.hpp"

//------------------------------------------------

/* the createContent method creates the content specific for TASKLIST */
void taskList::createContent (void)
{
    string typedTask,
           typedProgress;

    //------------------------------------------------
    
    while (1)
    {
        cout << "Task: ";
        if (getline(cin, typedTask) && typedTask == END)
            return;
        
        //------------------------------------------------
        
        cout << "State (finished/not_finished): ";
        while (!(cin >> typedProgress) ||
               (typedProgress != FINISHED && typedProgress != NOT_FINISHED))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE);
            cout << "PROGRAM: Invalid state." << endl << "State (finished/not_finished): ";
        } // while
    
        //------------------------------------------------
        
        task* newTask = new task(typedTask, typedProgress);
        list.push_back(newTask);
        cout << "PROGRAM: Task successfully created and saved to the task list." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // clears the I/O buffer
    } // while
} // CREATE CONTENT

//------------------------------------------------

/* the exportContent method exports content specific for TASKLIST */
void taskList::exportContent (ofstream & fileOut)
{
    fileOut << "NAME: "      << name      << endl
            << "TAG: "       << tag       << endl
            << "CATEGORY: "  << category  << endl
            << "TYPE: "      << type      << endl
            << "FORMATTED: " << formatted << endl
            << "CONTENT: "                << endl;

    for (auto & taskIt : list)
    {
		fileOut << taskIt->getTask()     << endl
                << taskIt->getProgress() << endl;
    } // for
} // EXPORT CONTENT

//------------------------------------------------

/* the changeContent method changes the content of a TASKLIST note */
bool taskList::changeContent (void)
{
    string choice,
           typedCommand,
           typedTask,
           typedProgress;
    unsigned pos;
    
    //------------------------------------------------
    
    cout << "<CONTENT EDITOR>=========<ON>" << endl
         << "Customize the note content"     << endl
         << "by /add, /delete and /change"   << endl
         << "commands."                      << endl
         << "To leave the content editor"    << endl
         << "write /finish."                 << endl
		 << DOUBLE_LINE                      << endl; // introduction
    
    while (cin >> typedCommand)
    {
        if (typedCommand == ADD)
		{
			cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            createContent();
		}
        else if (typedCommand == DELETE)
        {            
            if (!(cin >> pos) ||
                pos < 1 ||
                pos > (list.size()))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << INVALID_POSITION << endl;
                isTaskListEmpty();
                continue;
            } // if
            
            list.erase(list.begin() + pos - 1); // deletes the task
        } // else if
        else if (typedCommand == CHANGE)
        {           
            if (!(cin >> pos) ||
                pos < 1 ||
                pos > list.size())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << INVALID_POSITION << endl;
                isTaskListEmpty();
                continue;
            } // if
            
            cout << "What do you want to change: [Task | Progress]" << endl;
            while (!(cin >> choice) ||
                   (choice != "Task" &&
                    choice != "Progress"))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << INVALID_CHOICE << endl;
                continue;
            } // while
            
            if (choice == "Task" && cout << "Write a new task: ")
            {
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                getline(cin, typedTask);
                list.at(pos - 1)->setTask(typedTask);
            } // if
            else if (choice == "Progress" && cout << "Write a new progress: ")
            {
                while (!(cin >> typedProgress) ||
                       (typedProgress != FINISHED &&
                        typedProgress != NOT_FINISHED))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                    cout << "PROGRAM: Invalid progress." << endl << "Write a new progress: ";
                } // while
                list.at(pos - 1)->setProgress(typedProgress);
            } // else if
        } // else if    
        else if (typedCommand == FINISH)
        {
            cout << "<CONTENT EDITOR>========<OFF>" << endl;
            break;
        } // else if
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << "PROGRAM: Unknown command for CONTENT EDITOR." << endl;
            continue;
        } // else
        
        showContent(); // shows the note
    } // while
    
    return true;
} // CHANGE CONTENT

//------------------------------------------------

/* the importContent method imports content specific for TASKLIST */
bool taskList::importContent (ifstream & fileIn)
{
    string tmp,
           fileTask,
           fileProgress;
    
    //------------------------------------------------
    
    getline(fileIn, tmp);
    if (tmp.find("CONTENT: "))
    {
        cout << "PROGRAM: Format error while reading CONTENT." << endl;
        return false;
    } // if
    
    while (getline(fileIn, fileTask))
    {
        if (!getline(fileIn, fileProgress) ||
            (fileProgress != FINISHED && fileProgress != NOT_FINISHED))
        {
            cout << "PROGRAM: Format error while reading PROGRESS." << endl;
            return false;
        } // if
        
        task* newTask = new task(fileTask, fileProgress);
        list.push_back(newTask);
    } // while
    
    return true;
} // IMPORT CONTENT

//------------------------------------------------

/* the showContent method shows the content of TASKLIST note */
void taskList::showContent (int editor)
{
    cout << SINGLE_LINE                 << endl
         << "NAME: "      << name       << endl
         << "TAG: "       << tag        << endl
         << "CATEGORY: "  << category   << endl
         << "TYPE: "      << type       << endl
         << "FORMATTED: " << formatted  << endl
         << "CONTENT: "                 << endl
         << DOUBLE_LINE                 << endl;
    
    int i = 1;
    for (auto & taskIt : list)
    {
        cout << i++ << ") "  << taskIt->getTask()
             << " | State: " << taskIt->getProgress() << endl;
    } // for
    calculatePercent();
    
    cout << DOUBLE_LINE                                             << endl
         << "Done: " << fixed << setprecision(2) << percent << " %" << endl
         << SINGLE_LINE                                             << endl;
} // SHOW CONTENT

//------------------------------------------------

/* the doesItContain method tests whether it contains some string */
bool taskList::doesItContain (const string & str)
{
    for (auto & taskIt : list)
        if ((taskIt->getTask()).find(str) != string::npos)
            return true;

    return false;
} // DOES IT CONTAIN

//------------------------------------------------

/* the checkPercent method tests whether the percent meets the requirements */
bool taskList::checkPercent (const string & str,
                             int value)
{
    calculatePercent();
    if ((str == "Less" && percent < value) ||
        (str == "Equal" && percent == value) ||
        (str == "More" && percent > value))
        return true;
    
    return false;
} // CHECK PERCENT

//------------------------------------------------

/* the calculatePercent method calculates a percent according to all finished tasks in the task list */
void taskList::calculatePercent (void)
{
    int sum = 0;
    for (auto & taskIt : list)
    {
        if (taskIt->getProgress() == FINISHED)
            sum += 1;
    } // for
    if (list.size())
        percent = ((double)sum / list.size()) * 100;
    else
        percent = 0;
} // CALCULATE PERCENT

//------------------------------------------------

/* the isTaskInTaskList method tests whether the task is in the task list or not */
bool taskList::isTaskInTaskList (const string & task) const
{
    for (auto & taskIt : list)
        if (taskIt->getTask() == task)
        {
            cout << "PROGRAM: Task list already contains the task \"" << task << "\"." << endl;
            return true;
        } // if

    return false;
} // IS TASK IN TASK LIST

//------------------------------------------------

/* the isTaskListEmpty method tests whether the task list is empty or not */
bool taskList::isTaskListEmpty (void) const
{
    if (!list.size())
    {
        cout << "PROGRAM: Task list is empty." << endl;
        return true;
    } // if
    
    return false;
} // IS TASK LIST EMPTY
