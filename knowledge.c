/*
 * MIT License
 * 
 * Copyright (c) 2025 Jacob Adams
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "knowledge.h"
#include "constants.h"
#include "scripts.h"


/**
 * initStrings() - init all the strings in an array
 *
 * @funcName - array to init
 * @maxLen - max len of strings
*/
static void initStrings(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    for (int i = 0; i < NUM_SETS; i++)
    {
        for (int j = 0; j < FUNCTION_RESULT_SIZE*INT_LENGTH; j++)
        {
            funcName[i][j] = (char*) malloc(SIZE);
        }
    }
}

/**
 * initElementStrings() - init all the strings in an array
 *
 * @elementName - array to init
 * @maxLen - max len of strings
*/
static void initElementStrings(char *elementName[NUM_SETS][MAX_SET_ELEMENTS], int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    for (int i = 0; i < NUM_SETS; i++)
    {
        for (int j = 0; j < MAX_SET_ELEMENTS; j++)
        {
            elementName[i][j] = (char*) malloc(SIZE);
        }
    }
}

/**
 * writeFunc() - write the name of a function (and it's negation) into funcName
 *
 * @funcName - array to write name to
 * @set - the setID for the function
 * @index - the indicies for different sets to write into
 * @str the name of the function
 * @strNegation the name of the negation of the function
 * @maxLen - max len of strings
*/
static void writeFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], char *str, char *strNegation, int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    snprintf(funcName[set][index[set]], SIZE, "%s", str);
    index[set] += 1;
    snprintf(funcName[set][index[set]], SIZE, "%s", strNegation);
    index[set] += 1;
}

/**
 * writeFunc() - write the name of a function (and it's negation) into funcName
 * Also append a night identifier in the form _[NIGHT%d]
 *
 * @funcName - array to write name to
 * @set - the setID for the function
 * @index - the indicies for different sets to write into
 * @str the name of the function
 * @strNegation the name of the negation of the function
 * @night - the night of the function
 * @maxLen - max len of strings
*/
static void writeFuncNight(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], char *str, char *strNegation, int night, int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    snprintf(funcName[set][index[set]], SIZE, "%s_[NIGHT%d]", str, night);
    index[set] += 1;
    snprintf(funcName[set][index[set]], SIZE, "%s_[NIGHT%d]", strNegation, night);
    index[set] += 1;
}

/**
 * writeRoleFunc() - write the name of a role function (and it's negation) into funcName
 * Also append a night identifier in the form _[NIGHT%d]
 *
 * @funcName - array to write name to
 * @set - the setID for the function
 * @index - the indicies for different sets to write into
 * @name the role name
 * @night - the night of the function
 * @maxLen - max len of strings
*/
static void writeRoleFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], char *name, char* postfix, int night, int maxLen)
{
    char str[maxLen]; // Declare a character array to hold the string 
    char strNeg[maxLen]; // Declare a character array to hold the string 
    int SIZE = maxLen*sizeof(char);
    snprintf(str, SIZE, "is_%s%s", name, postfix);
    snprintf(strNeg, SIZE, "is_NOT_%s%s", name, postfix);
    writeFuncNight(funcName, set, index, str, strNeg, night, maxLen);
}

/**
 * writeDeathFunc() - write the name of a death function (and it's negation) into funcName
 * Also append a night identifier in the form _[NIGHT%d]
 *
 * @funcName - array to write name to
 * @set - the setID for the function
 * @index - the indicies for different sets to write into
 * @night - the night of the function
 * @maxLen - max len of strings
*/
static void writeDeathFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], int night, int maxLen)
{
    writeFuncNight(funcName, set, index, "is_DEAD", "is_ALIVE", night, maxLen);
}

/**
 * writePoisonFunc() - write the name of a poison function (and it's negation) into funcName
 * Also append a night identifier in the form _[NIGHT%d]
 *
 * @funcName - array to write name to
 * @set - the setID for the function
 * @index - the indicies for different sets to write into
 * @night - the night of the function
 * @maxLen - max len of strings
*/
static void writePoisonFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], int night, int maxLen)
{
    writeFuncNight(funcName, set, index, "is_POISONED", "is_NOT_POISONED", night, maxLen);
}

/**
 * initKB() - allocate and initilise a knowledge base structure
 *
 * @NUM_PLAYERS - num players in game
 * @NUM_DAYS - num days in game
 * 
 * @return the KB
*/
KnowledgeBase* initKB(const int NUM_PLAYERS, const int NUM_DAYS)
{
    //Allocate memory
    KnowledgeBase* kb = (KnowledgeBase*) malloc(sizeof(KnowledgeBase));

    //Fill knowlegde base with zeroes
    resetKnowledgeBase(kb);

    //Set names
    kb->SET_NAMES[0] = "PLAYERS";
    kb->SET_NAMES[1] = "DAYS";
    kb->SET_NAMES[2] = "METADATA";
    //Set sizes
    kb->SET_SIZES[0] = NUM_PLAYERS; //NUM PLAYERS
    kb->SET_SIZES[1] = NUM_DAYS; //NUM DAYS
    kb->SET_SIZES[2] = 1; //Metadata

    //Store index of next unwritten element
    int index[3] = {0, 0, 0};

    //Allocate function name strings memory
    initStrings(kb->FUNCTION_NAME, 64);
    initElementStrings(kb->ELEMENT_NAMES, 255);

    // ===========================================
    //  PLAYER FUNCTIONS
    // ===========================================
    //Roles
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
        {
            writeRoleFunc(kb->FUNCTION_NAME, 0, index, ROLE_NAMES[roleID], "", night, 64);
        }

        //Teams
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "is_GOOD", "is_EVIL", night,64);
        //Classes
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "is_TOWNSFOLK", "is_NOT_TOWNSFOLK", night, 64);
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "is_OUTSIDER", "is_NOT_OUTSIDER", night, 64);
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "is_MINION", "is_NOT_MINION", night, 64);
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "is_DEMON", "is_NOT_DEMON", night, 64);

        //Poison
        writePoisonFunc(kb->FUNCTION_NAME, 0, index, night, 64);

        //Deaths
        writeDeathFunc(kb->FUNCTION_NAME, 0, index, night, 64);
        //Death Types
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "SLEEP_DEATH", "NOT_SLEEP_DEATH", night, 64);
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "HANGING_DEATH", "NOT_HANGING_DEATH", night, 64);
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "NOMINATION_DEATH", "NOT_NOMINATION_DEATH", night, 64);
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "RESURRECTED", "NOT_RESURRECTED", night, 64);
        //Role changed
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "is_ROLE_CHANGED", "is_NOT_ROLE_CHANGED", night, 64);
    }

    //REDHERRING
    writeFunc(kb->FUNCTION_NAME, 0, index, "is_REDHERRING", "is_NOT_REDHERRING", 64);
    //Evil Twin Pair
    writeFunc(kb->FUNCTION_NAME, 0, index, "is_GOOD_TWIN", "is_NOT_GOOD_TWIN", 64);

    // ===========================================
    //  DAY FUNCTIONS
    // ===========================================
    //FUNCTION_NAME[1][0] = "idk";
    //writeFunc(kb->FUNCTION_NAME, 1, index, "idk", "idk", 64);

    // ===========================================
    //  METADATA FUNCTIONS
    // ===========================================
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
        {
            writeRoleFunc(kb->FUNCTION_NAME, 2, index, ROLE_NAMES[roleID], "_in_PLAY", night, 64);
            writeRoleFunc(kb->FUNCTION_NAME, 2, index, ROLE_NAMES[roleID], "_ALIVE", night, 64);
        }
    }

    return kb; //Return initilized knowledge base
}

/**
 * initProbKB() - allocate and initilise a probabalistic knowledge base structure
 * 
 * @return the Probabilistic KB
*/
ProbKnowledgeBase* initProbKB()
{
    //Allocate memory
    ProbKnowledgeBase* tally = (ProbKnowledgeBase*) malloc(sizeof(ProbKnowledgeBase));

    resetProbKnowledgeBase(tally);

    return tally;
}

/**
 * copyTo() - deep copy (mostly) a knowledge base object
 * 
 * @dest - destination knowledge base
 * @src - source knowledge base
*/
void copyTo(KnowledgeBase* dest, KnowledgeBase* src)
{
    //Deep copy knowlegde base (these might change)
    memcpy(dest->KNOWLEDGE_BASE, src->KNOWLEDGE_BASE, sizeof(long)*NUM_SETS*MAX_SET_ELEMENTS*FUNCTION_RESULT_SIZE);
    memcpy(dest->SET_SIZES, src->SET_SIZES, sizeof(int)*NUM_SETS);
    for (int set = 0; set < NUM_SETS; set++)
    {
        //Shallow copy names (these will not change)
        dest->SET_NAMES[set] = src->SET_NAMES[set];
        //strcpy(dest->SET_NAMES[i], src->SET_NAMES[i]); //Deep copy version

        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            //Shallow copy names (these will not change)
            dest->ELEMENT_NAMES[set][element] = src->ELEMENT_NAMES[set][element];
        }

        for (int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            //Shallow copy names (these will not change)
            dest->FUNCTION_NAME[set][function] = src->FUNCTION_NAME[set][function];
           //strcpy(dest->FUNCTION_NAME[i][j], src->FUNCTION_NAME[i][j]); //Deep copy version
        }
    }
}

/**
 * getSetIDWithName() - finds the setID (index) given the string name identifer
 * 
 * @kb - knowledge base containing the set
 * @set - the name of the set
 * @validate - if TRUE if the set if not found crash the program throwing an error
 * 
 * @return the setID/index of the set -1 if not found
*/
int getSetIDWithName(KnowledgeBase* kb, char* set, int validate)
{
    for (int i = 0; i < NUM_SETS; i++)
    {
        if (strcmp(set,kb->SET_NAMES[i]) == 0)
        {
            return i;
        }
    }
    if (validate == 1)
    {
        printf("ERROR: WRONG SET NAME '%s'\n", set);
        exit(1);
    }
    return -1;
}

/**
 * getSetFunctionIDWithName() - finds the functionID (index) given the string name identifer
 * 
 * @kb - knowledge base containing the set
 * @setID - the ID of the set
 * @function - the name of the function
 * @validate - if TRUE if the function if not found crash the program throwing an error
 * 
 * @return the functionID/index of the function -1 if not found
*/
int getSetFunctionIDWithName(KnowledgeBase* kb, int setID, char* function, int validate)
{
    for (int i = 0; i < FUNCTION_RESULT_SIZE*INT_LENGTH; i++)
    {
        //printf("COMPARE: %s, %s\n",function,FUNCTION_NAME[setID][i]);
        if (strcmp(function,kb->FUNCTION_NAME[setID][i]) == 0)
        {
            return i;
        }
    }
    if (validate == 1)
    {
        printf("ERROR: WRONG FUNCTION (set='%d') NAME '%s' set contains=\n", setID, function);
        for (int i = 0; i < FUNCTION_RESULT_SIZE*INT_LENGTH; i++)
        {
            printf("%s,\n",kb->FUNCTION_NAME[setID][i]);
        }
        exit(1);
    }
    return -1;
}

/**
 * getSetElementIDWithName() - finds the elementID (index) given the string name identifer
 * 
 * @kb - knowledge base containing the set
 * @setID - the ID of the set
 * @element - the name of the element
 * @validate - if TRUE if the element if not found crash the program throwing an error
 * 
 * @return the elementID/index of the element -1 if not found
*/
int getSetElementIDWithName(KnowledgeBase* kb, int setID, char* element, int validate)
{
    for (int i = 0; i < MAX_SET_ELEMENTS; i++)
    {
        //printf("COMPARE: %s, %s\n",function,FUNCTION_NAME[setID][i]);
        if (strcmp(element,kb->ELEMENT_NAMES[setID][i]) == 0)
        {
            return i;
        }
    }
    if (validate == 1)
    {
        printf("ERROR: WRONG ELEMENT (set='%d') NAME '%s' set contains=\n", setID, element);
        for (int i = 0; i < MAX_SET_ELEMENTS; i++)
        {
            printf("%s,\n",kb->ELEMENT_NAMES[setID][i]);
        }
        exit(1);
    }
    return -1;
}

/**
 * getIndexAndBit() - write into index and bit the location of the functionID
 * 
 * @index - functionID location array index output destination
 * @bit - functionID location array[index] bit index output destination
 * @functionID - the functionID/index
*/
void getIndexAndBit(int* index, int* bit, int functionID)
{
    *index = functionID / INT_LENGTH;
    *bit = functionID - (*index * INT_LENGTH);
}

/**
 * resetKnowledgeBase() - reset the knowledge in the knowledge base
 * 
 * @kb - the knowledge base to reset
*/
void resetKnowledgeBase(KnowledgeBase* kb)
{
    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
            {
                kb->KNOWLEDGE_BASE[set][element][i] = 0;
            }
        }
    }
}

/**
 * resetProbKnowledgeBase() - reset the knowledge in the probablistic knowledge base
 * 
 * @tally - the probabalistic knowledge base to reset
*/
void resetProbKnowledgeBase(ProbKnowledgeBase* tally)
{
    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for (int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
            {
                tally->KNOWLEDGE_BASE[set][element][function] = 0.0;
            }
        }
    }
    tally->tally = 0.0;
}

/**
 * resetElement() - resets an element within the knowledge base
 * 
 * @kb - the knowledge base to reset
 * @set - the setID/index the element belongs to
 * @element - the elementID/index the element belongs to
*/
void resetElement(KnowledgeBase* kb, int set, int element)
{
    for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
    {
        kb->KNOWLEDGE_BASE[set][element][i] = 0;
    }
}

/**
 * mergeKnowledge() - merge the knowledge from 2 different bases into 1 (inline)
 * 
 * @kb - the knowledge base to merge into
 * @x - the extra knowledge to add into kb (inline)
*/
void mergeKnowledge(KnowledgeBase* kb, KnowledgeBase* x)
{
    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < kb->SET_SIZES[set]; element++)
        {
            for (int i = 0 ; i < FUNCTION_RESULT_SIZE; i++)
            {
                kb->KNOWLEDGE_BASE[set][element][i] |= x->KNOWLEDGE_BASE[set][element][i];
            }
        }
    }
}

/**
 * mergeProbKnowledge() - merge the probabalistic knowledge from 2 different bases into 1 (inline)
 * 
 * @probkb - the probabalistic knowledge base to merge into
 * @x - the extra probabalistic knowledge to add into kb (inline)
*/
void mergeProbKnowledge(ProbKnowledgeBase* probkb, ProbKnowledgeBase* x)
{
    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for (int function = 0 ; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
            {
                probkb->KNOWLEDGE_BASE[set][element][function] += x->KNOWLEDGE_BASE[set][element][function];
            }
        }
    }
    probkb->tally += x->tally;
}

/**
 * addKnowledge() - add some knowlegde to the KB (set a function to true)
 * 
 * @kb - the knowledge base to add into (inline)
 * @set - the setID/index of the element to add knowledge to
 * @element - the elementID/index to add knowledge to
 * @function - the functionID/index to set to true
*/
void addKnowledge(KnowledgeBase* kb, int set, int element, int function)
{
    int index, bit;
    getIndexAndBit(&index, &bit, function);

    long mask = 1L << bit;

    kb->KNOWLEDGE_BASE[set][element][index] |= mask;
}

/**
 * addKnowledgeName() - add some knowlegde to the KB (set a function to true)
 * 
 * @kb - the knowledge base to add into (inline)
 * @set - the NAME of the set of the element to add knowledge to
 * @element - the elementID/index to add knowledge to
 * @function - the NAME of the function to set to true
*/
void addKnowledgeName(KnowledgeBase* kb, char* set, int element, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    addKnowledge(kb, setID, element, functionID);
}

/**
 * isKnown() - returns if something is known to be true
 * 
 * @kb - the knowledge base to check
 * @set - the the setID/index of the element to check
 * @element - the elementID/index to check
 * @function - the functionID/index to check if true
 * 
 * @return the value of the function in the knowledge base
*/
int isKnown(KnowledgeBase* kb, int set, int element, int function)
{
    int index, bit;
    getIndexAndBit(&index, &bit, function);

    return (kb->KNOWLEDGE_BASE[set][element][index] >> bit) & 1;
}

/**
 * isKnownName() - returns if something is known to be true
 * 
 * @kb - the knowledge base to check
 * @set - the NAME of the set of the element to check
 * @element - the elementID/index to check
 * @function - the NAME of the function to check if true
 * 
 * @return the value of the function in the knowledge base
*/
int isKnownName(KnowledgeBase* kb, char* set, int element, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    return isKnown(kb, setID, element, functionID);
}

/**
 * hasExplicitContradiction() - returns if an explict contradiction is found
 * An explicit contradiction is described is if X AND NOT(X) evaluates to true
 * Can be modified to working implicitly by first running infer steps
 * 
 * @kb - the knowledge base to check
 * 
 * @return TRUE (1) if there is a contradiction in the knowledge base
*/
int hasExplicitContradiction(KnowledgeBase* kb)
{
    //[NUM_SETS][MAX_SET_ELEMENTS][FUNCTION_RESULT_SIZE];
    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for (int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function += 2)
            {
                //Using the fact that for any statement A stored at an even index i NOT(A) is stored at the odd index i+1 
                if ((isKnown(kb, set, element, function) == 1) && (isKnown(kb, set, element, function+1) == 1))
                { //If A AND NOT(A) -> we have a contradiction
                    return 1; 
                }
            }
        }
    }
    return 0;
}

/**
 * addKBtoProbTally() - add a tally of [+weight] to each function which evaluates to true in the knowledge base
 * 
 * @kb - the knowledge base to add to the tally
 * @tally - the tally to add to
 * @weight - the weight to add
*/
void addKBtoProbTally(KnowledgeBase* kb, ProbKnowledgeBase* tally, double weight)
{

    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
            {
                if (isKnown(kb, set, element, function))
                {
                    tally->KNOWLEDGE_BASE[set][element][function] += weight;
                }
            }
        }
    }
    tally->tally += weight;
}
//Used to accomodate for floating point rounding
#define EPSILON 1.0
/**
 * getProbIntPercentage() - get an estimation for the probability of somethign being true in the tally
 * 
 * @tally - the probablistic knowledge base to extra the percentage from
 * @set - the setID/index of the element to get the percentage from
 * @element - the elementID/index to get the percentage from
 * @function - the functionID/index to get the percentage from
 * 
 * @return as a percentage P(ELEMENT in SET F(ELEMENT) = TRUE) * 100
*/
int getProbIntPercentage(ProbKnowledgeBase* tally, int set, int element, int function)
{
    double percentage = (tally->KNOWLEDGE_BASE[set][element][function]*100.0) / tally->tally;

    //Floating point rounding correction
    if (percentage > 100.0 - EPSILON) return 100;
    if (percentage < EPSILON) return 0;

    return (int) percentage; //Cast to int
}

/**
 * getProbIntPercentage() - get an estimation for the probability of somethign being true in the tally
 * 
 * @tally - the probablistic knowledge base to extra the percentage from
 * @set - the NAME of the set of the element to get the percentage from
 * @element - the elementID/index to get the percentage from
 * @function - the NAME of the function to get the percentage from
 * 
 * @return as a percentage P(ELEMENT in SET F(ELEMENT) = TRUE) * 100
*/
int getProbIntPercentageName(ProbKnowledgeBase* tally, KnowledgeBase* kb, char* set, int element, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    return getProbIntPercentage(tally, setID, element, functionID);
}

/**
 * printKnowledgeBase() - print the knowledge base to the terminal
 * 
 * @kb - the knowledge base to print to the termial
*/
void printKnowledgeBase(KnowledgeBase* kb)
{

    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
            {
                if (isKnown(kb, set, element, function))
                {
                    printf("%s(%d:%s), ", kb->FUNCTION_NAME[set][function], element, kb->SET_NAMES[set]);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

/**
 * printTrucatedStr() - print a trucated string
 * 
 * @str - the string to print
 * @maxLen - max length to print
*/
static void printTrucatedStr(char* str, int maxLen)
{
    for (int c = 0; c < maxLen; c++)
    {
        if (c < strlen(str))
        {
            printf("%c", str[c]);
        }
        else
        {
            printf(" ");
        }
    }
}

/**
 * printPlayerTable() - print the knowledge base to the terminal
 * Print in the form of a player table
 * 
 * @kb - the knowledge base to print to the termial
 * @night - night to print
*/
void printPlayerTable(KnowledgeBase* kb, int night)
{
    char buff[64];

    printf("         |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            printTrucatedStr(ROLE_NAMES[role], 3);
            PRINT_END
            printf("|");
        }
    }
    printf("   CLASS   | TEAM | POISONED | ALIVE |\n");
    printf("---------|");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            printf("---|");
            
        }
    }
    printf("-----------|------|----------|-------|\n");
    int set = 0;
    for (int element = 0; element < kb->SET_SIZES[set]; element++)
    {
        snprintf(buff, 64, "is_TOWNSFOLK_[NIGHT%d]", night);
        int isTownsfolk = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_OUTSIDER_[NIGHT%d]", night);
        int isOutsider = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_MINION_[NIGHT%d]", night);
        int isMinion = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_DEMON_[NIGHT%d]", night);
        int isDemon = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_GOOD_[NIGHT%d]", night);
        int isGood = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_EVIL_[NIGHT%d]", night);
        int isEvil = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_POISONED_[NIGHT%d]", night);
        int isPoisoned = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_NOT_POISONED_[NIGHT%d]", night);
        int isNotPoisoned = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_ALIVE_[NIGHT%d]", night);
        int isAlive = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_DEAD_[NIGHT%d]", night);
        int isDead = isKnownName(kb, "PLAYERS", element, buff); 

        printTrucatedStr(kb->ELEMENT_NAMES[0][element], 9);
        printf("|");
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            //Only print roles in the script
            if (ROLE_IN_SCRIPT[role] == 1)
            {
                if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
                else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
                else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
                else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
                snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isRole = isKnownName(kb, "PLAYERS", element, buff); 
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isNotRole = isKnownName(kb, "PLAYERS", element, buff);
                if (isRole == 1)
                {
                    PRINT_TITLE
                    printf(" * ");
                }
                else if (isNotRole == 1)
                {
                    printf("   ");
                }
                else
                {
                    printf(" ? ");
                }
                PRINT_END
                printf("|");
            }
        }
        if (isTownsfolk == 1)
        {
            PRINT_GREEN
            printf(" TOWNSFOLK ");
            PRINT_END
        }
        else if (isOutsider == 1)
        {
            PRINT_PURPLE
            printf("  OUTSIDER ");
            PRINT_END
        }
        else if (isMinion == 1)
        {
            PRINT_YELLOW
            printf("   MINION  ");
            PRINT_END
        }
        else if (isDemon == 1)
        {
            PRINT_RED
            printf("   DEMON   ");
            PRINT_END
        }
        else
        {
            printf("     ?     ");
        }
        printf("|");
        if (isGood == 1)
        {
            PRINT_GREEN
            printf(" GOOD ");
            PRINT_END
        }
        else if (isEvil == 1)
        {
            PRINT_RED
            printf(" EVIL ");
            PRINT_END
        }
        else
        {
            printf("  ?   ");
        }
        printf("|");
        if (isPoisoned == 1)
        {
            PRINT_RED
            printf(" POISONED ");
            PRINT_END
        }
        else if (isNotPoisoned == 1)
        {
            PRINT_GREEN
            printf("  HEALTHY ");
            PRINT_END
        }
        else
        {
            printf("     ?    ");
        }
        printf("|");
        if (isAlive == 1)
        {
            PRINT_GREEN
            printf(" ALIVE ");
            PRINT_END
        }
        else if (isDead == 1)
        {
            PRINT_RED
            printf(" DEAD  ");
            PRINT_END
        }
        else
        {
            printf("  ?    ");
        }
        printf("|\n");
    }
}

/**
 * printRoleTable() - print the knowledge base to the terminal
 * Print in the form of a role table
 * 
 * @kb - the knowledge base to print to the termial
 * @night - night to print
*/
void printRoleTable(KnowledgeBase* kb, int night)
{
    char buff[64];

    printf("         |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            printTrucatedStr(ROLE_NAMES[role], 3);
            printf("|");
            PRINT_END
        }
    }
    printf("\n");
    printf("---------|");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            printf("---|");
        }
    }
    //printf("-----------|------|");
    printf("\n");
    int element = 0;
    printf("IN GAME  |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            snprintf(buff, 64, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isRoleInPlay = isKnownName(kb, "METADATA", element, buff); 
            snprintf(buff, 64, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isNotRoleInPlay = isKnownName(kb, "METADATA", element, buff);

            if (isRoleInPlay == 1)
            {
                PRINT_TITLE
                printf(" * ");
            }
            else if (isNotRoleInPlay == 1)
            {
                printf("   ");
            }
            else
            {
                printf(" ? ");
            }
            PRINT_END
            printf("|");
        }
    }
    printf("\n");
    printf("IS ALIVE |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            snprintf(buff, 64, "is_%s_ALIVE_[NIGHT%d]", ROLE_NAMES[role], night);
            int isRoleInPlay = isKnownName(kb, "METADATA", element, buff); 
            snprintf(buff, 64, "is_NOT_%s_ALIVE_[NIGHT%d]", ROLE_NAMES[role], night);
            int isNotRoleInPlay = isKnownName(kb, "METADATA", element, buff);

            if (isRoleInPlay == 1)
            {
                PRINT_TITLE
                printf(" * ");
            }
            else if (isNotRoleInPlay == 1)
            {
                printf("   ");
            }
            else
            {
                printf(" ? ");
            }
            PRINT_END
            printf("|");
        }
    }
    printf("\n");
    
}

/**
 * printProbPlayerTable() - print the probablistic knowledge base to the terminal
 * Print in the form of a prob player table
 * 
 * @kb - the knowledge base that the prob tally is based off of to print to the termial
 * @kb - the probabalistic knowledge base to use for percentages (%) to print to the termial
 * @night - night to print
*/
void printProbPlayerTable(KnowledgeBase* kb, ProbKnowledgeBase* probkb, int night)
{
    char buff[64];

    printf("         |  ");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            printTrucatedStr(ROLE_NAMES[role], 3);
            PRINT_END
            printf("  |  ");
        }
    }
    printf("\n");
    printf("---------|");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            printf("-------|");
        }
    }
    printf("\n");
    int set = 0;
    for (int element = 0; element < kb->SET_SIZES[set]; element++)
    {


        printTrucatedStr(kb->ELEMENT_NAMES[0][element], 9);
        printf("|");
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            //Only print roles in the script
            if (ROLE_IN_SCRIPT[role] == 1)
            {
                if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
                else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
                else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
                else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
                snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isRole = getProbIntPercentageName(probkb, kb, "PLAYERS", element, buff); 
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isNotRole = getProbIntPercentageName(probkb, kb, "PLAYERS", element, buff);
                
                if (isRole == 100)
                {
                    PRINT_TITLE
                    printf("   *   ");
                }
                else if (isNotRole == 100)
                {
                    printf("       ");
                }
                else
                {
                    printf("%03d-%03d", isRole, isNotRole);
                }
                PRINT_END
                printf("|");
            }
        }
        printf("\n");
    }
}

/**
 * printProbRoleTable() - print the probablistic knowledge base to the terminal
 * Print in the form of a prob role table
 * 
 * @kb - the knowledge base that the prob tally is based off of to print to the termial
 * @kb - the probabalistic knowledge base to use for percentages (%) to print to the termial
 * @night - night to print
*/
void printProbRoleTable(KnowledgeBase* kb, ProbKnowledgeBase* probkb, int night)
{
    char buff[64];

    printf("         |  ");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            printTrucatedStr(ROLE_NAMES[role], 3);
            PRINT_END
            printf("  |  ");
        }
    }
    printf("\n");
    printf("---------|");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            printf("-------|");
        }
    }
    printf("\n");
    int element = 0;
    printf("IN GAME  |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            snprintf(buff, 64, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isRoleInPlay = getProbIntPercentageName(probkb, kb, "METADATA", element, buff); 
            snprintf(buff, 64, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isNotRoleInPlay = getProbIntPercentageName(probkb, kb, "METADATA", element, buff);

            if (isRoleInPlay == 100)
            {
                PRINT_TITLE
                printf("   *   ");
            }
            else if (isNotRoleInPlay == 100)
            {
                printf("       ");
            }
            else
            {
                printf("%03d-%03d", isRoleInPlay, isNotRoleInPlay);
            }
            PRINT_END
            printf("|");
        }
    }
    printf("\n");
    printf("IS ALIVE |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0) PRINT_RED
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0) PRINT_YELLOW
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0) PRINT_PURPLE
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0) PRINT_GREEN
            snprintf(buff, 64, "is_%s_ALIVE_[NIGHT%d]", ROLE_NAMES[role], night);
            int isRoleInPlay = getProbIntPercentageName(probkb, kb, "METADATA", element, buff); 
            snprintf(buff, 64, "is_NOT_%s_ALIVE_[NIGHT%d]", ROLE_NAMES[role], night);
            int isNotRoleInPlay = getProbIntPercentageName(probkb, kb, "METADATA", element, buff);

            if (isRoleInPlay == 100)
            {
                PRINT_TITLE
                printf("   *   ");
            }
            else if (isNotRoleInPlay == 100)
            {
                printf("       ");
            }
            else
            {
                printf("%03d-%03d", isRoleInPlay, isNotRoleInPlay);
            }
            PRINT_END
            printf("|");
        }
    }
    printf("\n");
    
}