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



void initStrings(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int maxLen)
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

void initElementStrings(char *elementName[NUM_SETS][MAX_SET_ELEMENTS], int maxLen)
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
void writeFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], char *str, char *strNegation, int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    snprintf(funcName[set][index[set]], SIZE, "%s", str);
    index[set] += 1;
    snprintf(funcName[set][index[set]], SIZE, "%s", strNegation);
    index[set] += 1;
}

void writeFuncNight(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], char *str, char *strNegation, int night, int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    snprintf(funcName[set][index[set]], SIZE, "%s_[NIGHT%d]", str, night);
    index[set] += 1;
    snprintf(funcName[set][index[set]], SIZE, "%s_[NIGHT%d]", strNegation, night);
    index[set] += 1;
}

void writeRoleFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], char *name, int night, int maxLen)
{
    char str[maxLen]; // Declare a character array to hold the string 
    char strNeg[maxLen]; // Declare a character array to hold the string 
    int SIZE = maxLen*sizeof(char);
    snprintf(str, SIZE, "is_%s", name);
    snprintf(strNeg, SIZE, "is_NOT_%s", name);
    writeFuncNight(funcName, set, index, str, strNeg, night, maxLen);
}

void writeRoleInGameFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], char *name, int night, int maxLen)
{
    char str[maxLen]; // Declare a character array to hold the string 
    char strNeg[maxLen]; // Declare a character array to hold the string 
    int SIZE = maxLen*sizeof(char);
    snprintf(str, SIZE, "is_%s_in_PLAY", name);
    snprintf(strNeg, SIZE, "is_NOT_%s_in_PLAY", name);
    writeFuncNight(funcName, set, index, str, strNeg, night, maxLen);
}

void writeDeathFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], int night, int maxLen)
{
    writeFuncNight(funcName, set, index, "is_DEAD", "is_ALIVE", night, maxLen);
}

void writePoisonFunc(char *funcName[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH], int set, int index[], int night, int maxLen)
{
    writeFuncNight(funcName, set, index, "is_POISONED", "is_NOT_POISONED", night, maxLen);
}



KnowledgeBase* initKB(int NUM_PLAYERS, int NUM_DAYS)
{
    //Allocate memory
    KnowledgeBase* kb = (KnowledgeBase*) malloc(sizeof(KnowledgeBase));

    

    resetKnowledgeBase(kb->KNOWLEDGE_BASE);

    //Sets
    kb->SET_NAMES[0] = "PLAYERS";
    kb->SET_NAMES[1] = "DAYS";
    kb->SET_NAMES[2] = "METADATA";

    kb->SET_SIZES[0] = NUM_PLAYERS; //NUM PLAYERS
    kb->SET_SIZES[1] = NUM_DAYS; //NUM DAYS
    kb->SET_SIZES[2] = 1; //Metadata

    int index[3] = {0, 0, 0};


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
            writeRoleFunc(kb->FUNCTION_NAME, 0, index, ROLE_NAMES[roleID], night, 64);
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
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "died_in_NIGHT", "died_NOT_in_NIGHT", night, 64);
        writeFuncNight(kb->FUNCTION_NAME, 0, index, "died_by_HANGING", "died_NOT_by_HANGING", night, 64);
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
            writeRoleInGameFunc(kb->FUNCTION_NAME, 2, index, ROLE_NAMES[roleID], night, 64);
        }
    }

    return kb;
}

ProbKnowledgeBase* initProbKB()
{
    //Allocate memory
    ProbKnowledgeBase* tally = (ProbKnowledgeBase*) malloc(sizeof(ProbKnowledgeBase));

    resetProbKnowledgeBase(tally);

    return tally;
}

void copyTo(KnowledgeBase* dest, KnowledgeBase* src)
{
    //Deep copy knowlegde base (these might change)
    memcpy(dest->KNOWLEDGE_BASE, src->KNOWLEDGE_BASE, sizeof(long)*NUM_SETS*MAX_SET_ELEMENTS*FUNCTION_RESULT_SIZE);
    memcpy(dest->SET_SIZES, src->SET_SIZES, sizeof(int)*NUM_SETS);
    for (int i = 0; i < NUM_SETS; i++)
    {
        //Shallow copy names (these will not change)
        dest->SET_NAMES[i] = src->SET_NAMES[i];
        //strcpy(dest->SET_NAMES[i], src->SET_NAMES[i]); //Deep copy version

        for (int j = 0; j < MAX_SET_ELEMENTS; j++)
        {
            //Shallow copy names (these will not change)
            dest->ELEMENT_NAMES[i][j] = src->ELEMENT_NAMES[i][j];
        }

        for (int j = 0; j < FUNCTION_RESULT_SIZE*INT_LENGTH; j++)
        {
            //Shallow copy names (these will not change)
            dest->FUNCTION_NAME[i][j] = src->FUNCTION_NAME[i][j];
           //strcpy(dest->FUNCTION_NAME[i][j], src->FUNCTION_NAME[i][j]); //Deep copy version
        }
    }
}

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

void getIndexAndBit(int* index, int* bit, int functionID)
{
    *index = functionID / INT_LENGTH;
    *bit = functionID - (*index * INT_LENGTH);
}



void resetKnowledgeBase(long knowledgeBase[NUM_SETS][MAX_SET_ELEMENTS][FUNCTION_RESULT_SIZE])
{
    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
            {
                knowledgeBase[set][element][i] = 0;
            }
        }
    }
}

void resetProbKnowledgeBase(ProbKnowledgeBase* tally)
{
    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for (int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
            {
                tally->KNOWLEDGE_BASE[set][element][function] = 0;
            }
        }
    }
    tally->tally = 0;
}

void resetElement(KnowledgeBase* kb, int set, int element)
{
    for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
    {
        kb->KNOWLEDGE_BASE[set][element][i] = 0;
    }
}

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

void addKnowledge(KnowledgeBase* kb, int set, int element, int function)
{
    int index, bit;
    getIndexAndBit(&index, &bit, function);

    long mask = 1L << bit;

    kb->KNOWLEDGE_BASE[set][element][index] |= mask;
}

int isKnown(KnowledgeBase* kb, int set, int element, int function)
{
    int index, bit;
    getIndexAndBit(&index, &bit, function);

    return (kb->KNOWLEDGE_BASE[set][element][index] >> bit) & 1;
}

int isKnownName(KnowledgeBase* kb, char* set, int element, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    return isKnown(kb, setID, element, functionID);
}

void addKnowledgeName(KnowledgeBase* kb, char* set, int element, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    addKnowledge(kb, setID, element, functionID);
}


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

void addKBtoProbTally(KnowledgeBase* kb, ProbKnowledgeBase* tally)
{

    for (int set = 0; set < NUM_SETS; set++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
            {
                if (isKnown(kb, set, element, function))
                {
                    tally->KNOWLEDGE_BASE[set][element][function]++;
                }
            }
        }
    }
    tally->tally++;
}

int getProbIntPercentage(ProbKnowledgeBase* tally, int set, int element, int function)
{
    return (tally->KNOWLEDGE_BASE[set][element][function]*100) / tally->tally;
}

int getProbIntPercentageName(ProbKnowledgeBase* tally, KnowledgeBase* kb, char* set, int element, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    return getProbIntPercentage(tally, setID, element, functionID);
}

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


void printPlayerName(char* name, int maxLen)
{
    for (int c = 0; c < maxLen; c++)
    {
        if (c < strlen(name))
        {
            printf("%c", name[c]);
        }
        else
        {
            printf(" ");
        }
    }
}

void printPlayerTable(KnowledgeBase* kb, int night)
{
    char buff[64];

    printf("         |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            for (int c = 0; c < 3; c++)
            {
                printf("%c", ROLE_NAMES[role][c]);
            }
            printf("|");
        }
    }
    printf("   CLASS   | TEAM | POISONED |\n");
    printf("---------|");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            printf("---|");
        }
    }
    printf("-----------|------|----------|\n");
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

        //printf("PLAYER %d |", element);
        //printf("%s |", kb->ELEMENT_NAMES[0][element]);
        printPlayerName(kb->ELEMENT_NAMES[0][element], 9);
        printf("|");
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            //Only print roles in the script
            if (ROLE_IN_SCRIPT[role] == 1)
            {
                snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isRole = isKnownName(kb, "PLAYERS", element, buff); 
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isNotRole = isKnownName(kb, "PLAYERS", element, buff);
                if (isRole == 1)
                {
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
                printf("|");
            }
        }
        if (isTownsfolk == 1)
        {
            printf(" TOWNSFOLK ");
        }
        else if (isOutsider == 1)
        {
            printf("  OUTSIDER ");
        }
        else if (isMinion == 1)
        {
            printf("   MINION  ");
        }
        else if (isDemon == 1)
        {
            printf("   DEMON   ");
        }
        else
        {
            printf("     ?     ");
        }
        printf("|");
        if (isGood == 1)
        {
            printf(" GOOD ");
        }
        else if (isEvil == 1)
        {
            printf(" EVIL ");
        }
        else
        {
            printf("  ?   ");
        }
        printf("|");
        if (isPoisoned == 1)
        {
            printf(" POISONED ");
        }
        else if (isNotPoisoned == 1)
        {
            printf("  HEALTHY ");
        }
        else
        {
            printf("     ?    ");
        }
        printf("|\n");
    }
}

void printRoleTable(KnowledgeBase* kb, int night)
{
    char buff[64];

    printf("         |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            for (int c = 0; c < 3; c++)
            {
                printf("%c", ROLE_NAMES[role][c]);
            }
            printf("|");
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
            snprintf(buff, 64, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isRoleInPlay = isKnownName(kb, "METADATA", element, buff); 
            snprintf(buff, 64, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isNotRoleInPlay = isKnownName(kb, "METADATA", element, buff);

            if (isRoleInPlay == 1)
            {
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
            printf("|");
        }
    }
    printf("\n");
    
}

void printProbPlayerTable(KnowledgeBase* kb, ProbKnowledgeBase* probkb, int night)
{
    char buff[64];

    printf("         |  ");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            for (int c = 0; c < 3; c++)
            {
                printf("%c", ROLE_NAMES[role][c]);
            }
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


        printPlayerName(kb->ELEMENT_NAMES[0][element], 9);
        printf("|");
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            //Only print roles in the script
            if (ROLE_IN_SCRIPT[role] == 1)
            {
                snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isRole = getProbIntPercentageName(probkb, kb, "PLAYERS", element, buff); 
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isNotRole = getProbIntPercentageName(probkb, kb, "PLAYERS", element, buff);
                
                if (isRole == 100)
                {
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
                printf("|");
                }
        }
        printf("\n");
    }
}

void printProbRoleTable(KnowledgeBase* kb, ProbKnowledgeBase* probkb, int night)
{
    char buff[64];

    printf("         |  ");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            for (int c = 0; c < 3; c++)
            {
                printf("%c", ROLE_NAMES[role][c]);
            }
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
    //printf("-----------|------|");
    printf("\n");
    int element = 0;
    printf("IN GAME  |");
    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            snprintf(buff, 64, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isRoleInPlay = getProbIntPercentageName(probkb, kb, "METADATA", element, buff); 
            snprintf(buff, 64, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
            int isNotRoleInPlay = getProbIntPercentageName(probkb, kb, "METADATA", element, buff);

            if (isRoleInPlay == 100)
            {
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
            printf("|");
        }
    }
    printf("\n");
    
}