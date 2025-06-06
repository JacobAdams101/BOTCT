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

#include "rules.h"
#include "knowledge.h"
#include "rules.h"
#include <stdbool.h>

#define NUM_SOLVE_STEPS 5

int inferImplicitFacts(KnowledgeBase* kb, RuleSet* rs, int numRounds, int verbose);

void* getProbApproxContinuous(void* void_arg);
/*
 * struct to store function args for getProbApprox()
*/
struct getProbApproxArgs
{
    KnowledgeBase* kb;
    KnowledgeBase* possibleWorldKB;
    KnowledgeBase**** possibleWorldRevertKB;
    ProbKnowledgeBase* determinedInNWorlds;
    ProbKnowledgeBase* worldTally;
    CachedKnowledgeBases* POSSIBLE_WORLDS_FOR_PROB;
    int (*POSSIBLE_WORLD_GENERATED)[MAX_SET_ELEMENTS][NUM_BOTCT_ROLES][NUM_DAYS];
    RuleSet* rs;
    int* worldGeneration;
    bool* reRenderCall;
    int numIterations;
};

/**
 * updateCacheWithNewKB()
 * 
 * 
 */
void updateCacheWithNewKB(CachedKnowledgeBases* cache, KnowledgeBase* kb, RuleSet* rs);