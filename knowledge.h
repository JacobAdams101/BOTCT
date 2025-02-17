#pragma once

#include "constants.h"

#define NUM_SETS 3
#define MAX_SET_ELEMENTS 16
#define FUNCTION_RESULT_SIZE 2

typedef struct {
    long KNOWLEDGE_BASE[NUM_SETS][MAX_SET_ELEMENTS][FUNCTION_RESULT_SIZE];
    int SET_SIZES[NUM_SETS];

    char *FUNCTION_NAME[NUM_SETS][FUNCTION_RESULT_SIZE*INT_LENGTH];
    char *SET_NAMES[NUM_SETS];
    char *ELEMENT_NAMES[NUM_SETS][MAX_SET_ELEMENTS];
} KnowledgeBase;

KnowledgeBase* initKB(int NUM_PLAYERS, int NUM_DAYS);

void copyTo(KnowledgeBase* dest, KnowledgeBase* src);

void resetKnowledgeBase(long knowledgeBase[NUM_SETS][MAX_SET_ELEMENTS][FUNCTION_RESULT_SIZE]);

int getSetIDWithName(KnowledgeBase* kb, char* set);
int getSetFunctionIDWithName(KnowledgeBase* kb, int setID, char* function);

void getIndexAndBit(int* index, int* bit, int functionID);

void addKnowledge(KnowledgeBase* kb,  int set, int element, int function);
void addKnowledgeName(KnowledgeBase* kb,  char* set, int element, char* function);

int isKnown(KnowledgeBase* kb,  int set, int element, int function);

int hasExplicitContradiction(KnowledgeBase* kb);

void printKnowledgeBase(KnowledgeBase* kb);
void printPlayerTable(KnowledgeBase* kb);
void printRoleTable(KnowledgeBase* kb);