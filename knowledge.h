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

KnowledgeBase* initKB(int NUM_PLAYERS, int NUM_DAYS);

void copyTo(KnowledgeBase* dest, KnowledgeBase* src);

void resetKnowledgeBase(long knowledgeBase[NUM_SETS][MAX_SET_ELEMENTS][FUNCTION_RESULT_SIZE]);

int getSetIDWithName(KnowledgeBase* kb, char* set, int validate);
int getSetFunctionIDWithName(KnowledgeBase* kb, int setID, char* function, int validate);
int getSetElementIDWithName(KnowledgeBase* kb, int setID, char* element, int validate);

void getIndexAndBit(int* index, int* bit, int functionID);

void resetElement(KnowledgeBase* kb, int set, int element);
void mergeKnowledge(KnowledgeBase* kb, KnowledgeBase* x);
void addKnowledge(KnowledgeBase* kb,  int set, int element, int function);
void addKnowledgeName(KnowledgeBase* kb,  char* set, int element, char* function);

int isKnown(KnowledgeBase* kb,  int set, int element, int function);
int isKnownName(KnowledgeBase* kb,  char* set, int element, char* function);

int hasExplicitContradiction(KnowledgeBase* kb);

void printKnowledgeBase(KnowledgeBase* kb);
void printPlayerTable(KnowledgeBase* kb, int night);
void printRoleTable(KnowledgeBase* kb, int night);
