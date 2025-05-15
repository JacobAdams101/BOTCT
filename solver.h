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

#define NUM_SOLVE_STEPS 5

/**
 * solve() - a function to ask the user for informaiton and tries to solve the game
 * 
 * @kb the knowledge base to use
 * @rs the ruleset
 * @NUM_PLAYERS the number of players in the game
 * @NUM_MINIONS the number of base starting minions in the game
 * @NUM_DEMONS the number of base starting demons in the game
 * @BASE_OUTSIDERS the number of abse starting outsiders in the game
 * @NUM_DAYS the maxium number of days the game can go on for
*/
void solve(KnowledgeBase* kb, RuleSet* rs, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS);

int inferImplicitFacts(KnowledgeBase* kb, RuleSet* rs, int numRounds, int verbose);

void* getProbApprox(void* void_arg);
/*
 * struct to store function args for getProbApprox()
*/
struct getProbApproxArgs
{
    KnowledgeBase* kb;
    KnowledgeBase* possibleWorldKB;
    KnowledgeBase*** possibleWorldRevertKB;
    ProbKnowledgeBase* determinedInNWorlds;
    RuleSet* rs;
    int numIterations;
};