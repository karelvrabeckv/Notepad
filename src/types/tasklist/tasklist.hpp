#pragma once

#include "../../constants.hpp"
#include "../../database/database.hpp"

//////////////////////////////////////////////////
// TASK
//////////////////////////////////////////////////

/** TASK together with its PROGRESS represents a MEMBER of TASK LIST */
struct task
{
        
	private:
		
		string text,
			   progress;

	public:
			   
		task (const string & typedTask,
			  const string & typedProgress) : text     (typedTask),
											  progress (typedProgress) {}

		/** setters of task */
		void setTask     (const string & typedTask)     {text     = typedTask;}
		void setProgress (const string & typedProgress) {progress = typedProgress;}

		/** getters of task */
		string getTask     (void) {return text;}
		string getProgress (void) {return progress;}
        
}; // TASK

//////////////////////////////////////////////////
// TASK LIST
//////////////////////////////////////////////////

/** TASK LIST is a NOTE TYPE and CLASS DESCENDANT of NOTE */
class taskList : public note
{
    private:

        vector <task*> list;
        double percent;
        
    public:
        
        taskList (const string & typedName,
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
        
        virtual ~taskList (void)
        {
            for (auto & taskIt : list)
                delete taskIt;
        } // DESTRUCTOR
        
		/** the createContent method creates the content specific for TASKLIST */
        virtual void createContent (void);
		/** the exportContent method exports content specific for TASKLIST */
        virtual void exportContent (ofstream & fileOut);
		/** the changeContent method changes the content of a TASKLIST note */
        virtual bool changeContent (void);
		/** the importContent method imports content specific for TASKLIST */
        virtual bool importContent (ifstream & fileIn);
		/** the showContent method shows the content of TASKLIST note */
        virtual void showContent   (int editor = EDITOR_OFF);
        
		/** the doesItContain method tests whether it contains some string */
        virtual bool doesItContain (const string & str);
		/** the checkPercent method tests whether the percent meets the requirements */
        virtual bool checkPercent  (const string & str,
                                    int value);
        
		/** the calculatePercent method calculates a percent according to all finished tasks in the task list */
        void calculatePercent (void);
		/** the isTaskInTaskList method tests whether the task is in the task list or not */
        bool isTaskInTaskList (const string & task) const;
		/** the isTaskListEmpty method tests whether the task list is empty or not */
        bool isTaskListEmpty  (void) const;
        
}; // TASK LIST
