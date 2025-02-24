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

#pragma once

#include "constants.h"

typedef struct {
    long KNOWLEDGE_BASE[NUM_SETS][MAX_SET_ELEMENTS][FUNCTION_RESULT_SIZE];
    int SET_SIZES[NUM_SETS];

    char *FUNCTION_NAME[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH];
    char *SET_NAMES[NUM_SETS];
    char *ELEMENT_NAMES[NUM_SETS][MAX_SET_ELEMENTS];
} KnowledgeBase;

typedef struct {
    int KNOWLEDGE_BASE[NUM_SETS][MAX_SET_ELEMENTS][FUNCTION_RESULT_SIZE*INT_LENGTH];
    int tally;
} ProbKnowledgeBase;
/**
 * initKB() - allocate and initilise a knowledge base structure
 *
 * @NUM_PLAYERS - num players in game
 * @NUM_DAYS - num days in game
 * 
 * @return the KB
*/
KnowledgeBase* initKB(const int NUM_PLAYERS, const int NUM_DAYS);

/**
 * initProbKB() - allocate and initilise a probabalistic knowledge base structure
 * 
 * @return the Probabilistic KB
*/
ProbKnowledgeBase* initProbKB();

/**
 * copyTo() - deep copy (mostly) a knowledge base object
 * 
 * @dest - destination knowledge base
 * @src - source knowledge base
*/
void copyTo(KnowledgeBase* dest, KnowledgeBase* src);

/**
 * resetKnowledgeBase() - reset the knowledge in the knowledge base
 * 
 * @kb - the knowledge base to reset
*/
void resetKnowledgeBase(KnowledgeBase* kb);

/**
 * resetProbKnowledgeBase() - reset the knowledge in the probablistic knowledge base
 * 
 * @tally - the probabalistic knowledge base to reset
*/
void resetProbKnowledgeBase(ProbKnowledgeBase* tally);

/**
 * getSetIDWithName() - finds the setID (index) given the string name identifer
 * 
 * @kb - knowledge base containing the set
 * @set - the name of the set
 * @validate - if TRUE if the set if not found crash the program throwing an error
 * 
 * @return the setID/index of the set -1 if not found
*/
int getSetIDWithName(KnowledgeBase* kb, char* set, int validate);

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
int getSetFunctionIDWithName(KnowledgeBase* kb, int setID, char* function, int validate);

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
int getSetElementIDWithName(KnowledgeBase* kb, int setID, char* element, int validate);

/**
 * getIndexAndBit() - write into index and bit the location of the functionID
 * 
 * @index - functionID location array index output destination
 * @bit - functionID location array[index] bit index output destination
 * @functionID - the functionID/index
*/
void getIndexAndBit(int* index, int* bit, int functionID);

/**
 * resetElement() - resets an element within the knowledge base
 * 
 * @kb - the knowledge base to reset
 * @set - the setID/index the element belongs to
 * @element - the elementID/index the element belongs to
*/
void resetElement(KnowledgeBase* kb, int set, int element);

/**
 * mergeKnowledge() - merge the knowledge from 2 different bases into 1 (inline)
 * 
 * @kb - the knowledge base to merge into
 * @x - the extra knowledge to add into kb (inline)
*/
void mergeKnowledge(KnowledgeBase* kb, KnowledgeBase* x);

/**
 * mergeProbKnowledge() - merge the probabalistic knowledge from 2 different bases into 1 (inline)
 * 
 * @probkb - the probabalistic knowledge base to merge into
 * @x - the extra probabalistic knowledge to add into kb (inline)
*/
void mergeProbKnowledge(ProbKnowledgeBase* probkb, ProbKnowledgeBase* x);

/**
 * addKnowledge() - add some knowlegde to the KB (set a function to true)
 * 
 * @kb - the knowledge base to add into (inline)
 * @set - the set of the element to add knowledge to
 * @element - the element to add knowledge to
 * @function - the functionID/index to set to true
*/
void addKnowledge(KnowledgeBase* kb,  int set, int element, int function);

/**
 * addKnowledgeName() - add some knowlegde to the KB (set a function to true)
 * 
 * @kb - the knowledge base to add into (inline)
 * @set - the NAME of the set of the element to add knowledge to
 * @element - the elementID/index to add knowledge to
 * @function - the NAME of the function to set to true
*/
void addKnowledgeName(KnowledgeBase* kb,  char* set, int element, char* function);

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
int isKnown(KnowledgeBase* kb,  int set, int element, int function);

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
int isKnownName(KnowledgeBase* kb,  char* set, int element, char* function);

/**
 * hasExplicitContradiction() - returns if an explict contradiction is found
 * An explicit contradiction is described is if X AND NOT(X) evaluates to true
 * Can be modified to working implicitly by first running infer steps
 * 
 * @kb - the knowledge base to check
 * 
 * @return TRUE (1) if there is a contradiction in the knowledge base
*/
int hasExplicitContradiction(KnowledgeBase* kb);

/**
 * addKBtoProbTally() - add a tally of 1 to each function which evaluates to true in the knowledge base
 * 
 * @kb - the knowledge base to add to the tally
 * @tally - the tally to add to
*/
void addKBtoProbTally(KnowledgeBase* kb, ProbKnowledgeBase* tally);

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
int getProbIntPercentage(ProbKnowledgeBase* tally, int set, int element, int function);

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
int getProbIntPercentageName(ProbKnowledgeBase* tally, KnowledgeBase* kb, char* set, int element, char* function);

/**
 * printKnowledgeBase() - print the knowledge base to the terminal
 * 
 * @kb - the knowledge base to print to the termial
*/
void printKnowledgeBase(KnowledgeBase* kb);

/**
 * printPlayerTable() - print the knowledge base to the terminal
 * Print in the form of a player table
 * 
 * @kb - the knowledge base to print to the termial
 * @night - night to print
*/
void printPlayerTable(KnowledgeBase* kb, int night);

/**
 * printProbPlayerTable() - print the probablistic knowledge base to the terminal
 * Print in the form of a prob player table
 * 
 * @kb - the knowledge base that the prob tally is based off of to print to the termial
 * @kb - the probabalistic knowledge base to use for percentages (%) to print to the termial
 * @night - night to print
*/
void printProbPlayerTable(KnowledgeBase* kb, ProbKnowledgeBase* probkb, int night);

/**
 * printRoleTable() - print the knowledge base to the terminal
 * Print in the form of a role table
 * 
 * @kb - the knowledge base to print to the termial
 * @night - night to print
*/
void printRoleTable(KnowledgeBase* kb, int night);

/**
 * printProbRoleTable() - print the probablistic knowledge base to the terminal
 * Print in the form of a prob role table
 * 
 * @kb - the knowledge base that the prob tally is based off of to print to the termial
 * @kb - the probabalistic knowledge base to use for percentages (%) to print to the termial
 * @night - night to print
*/
void printProbRoleTable(KnowledgeBase* kb, ProbKnowledgeBase* probkb, int night);
