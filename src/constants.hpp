#pragma once

/** find function is used by DEFAULT */
const int FIND_DEFAULT = 0;
/** find function is used to EXPORT */
const int FIND_EXPORT = 1;
/** find function is used to IMPORT */
const int FIND_IMPORT = 2;

/** the editor is OFF */
const int EDITOR_OFF = 0;
/** the editor is ON */
const int EDITOR_ON = 1;

/** option for e. g. ignore() function */
const char NEW_LINE = '\n';

/** lines used for FORMATTING the text */
const char* const DOUBLE_LINE = "=============================";
const char* const SINGLE_LINE = "-----------------------------";
const char* const ARROW_LINE  = "<<<<<<<<<<<<<<<<<<<<<<<<<<<<|";

/** commands in CONTENT EDITOR */
const char* const ADD    = "/add";
const char* const END    = "/end";
const char* const DELETE = "/delete";
const char* const CHANGE = "/change";
const char* const FINISH = "/finish";

/** options for FORMATTED */
const char* const YES = "yes";
const char* const NO  = "no";

/** the DIRECTORY and SUFFIX names */
const char* const DIRECTORY = "./notes";
const char* const SUFFIX    = ".pb";

/** options for PROGRESS in TASKLIST */
const char* const FINISHED     = "finished";
const char* const NOT_FINISHED = "not_finished";

/** options for SEARCHING notes */
const char* const NOTE_NOT_FOUND   = "PROGRAM: Note not found.";
const char* const NO_MATCHES_FOUND = "PROGRAM: No matches found.";

/** options for BAD INPUTS */
const char* const INVALID_AMOUNT    = "PROGRAM: Invalid amount.";
const char* const INVALID_ANSWER    = "PROGRAM: Invalid answer.";
const char* const INVALID_ATTRIBUTE = "PROGRAM: Invalid attribute.";
const char* const INVALID_FILTER    = "PROGRAM: Invalid filter.";
const char* const INVALID_CHOICE    = "PROGRAM: Invalid choice.";
const char* const INVALID_POSITION  = "PROGRAM: Invalid position.";
const char* const INVALID_PRICE     = "PROGRAM: Invalid price.";
const char* const INVALID_VALUE     = "PROGRAM: Invalid value.";
