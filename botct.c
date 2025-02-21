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

//Multi-threading
#include <pthread.h>

#include "rules.h"
#include "knowledge.h"
#include "constants.h"
#include "scripts.h"
#include "ui.h"

#define NUM_SOLVE_STEPS 5

static int inferImplicitFacts(KnowledgeBase* kb, RuleSet* rs, int numRounds, int verbose)
{
        
    for (int i = 0; i < numRounds; i++)
    {
        int foundNovelSolution = inferknowledgeBaseFromRules(rs, kb, verbose);
        if (foundNovelSolution == 0)
        { //If nothing new was found
            break;
        }
    }
    //Check for contradictions
    return hasExplicitContradiction(kb);
}

struct inferFactsByContradictionArgs
{
    KnowledgeBase* kb;
    KnowledgeBase* contradiction_kb;
    RuleSet* rs;
    int min;
    int max;
};
static void* inferFactsByContradiction(void* void_arg)
{
    struct inferFactsByContradictionArgs *args = (struct inferFactsByContradictionArgs*) void_arg; //Get arguments by casting

    KnowledgeBase* kb = args->kb;
    KnowledgeBase* contradiction_kb = args->contradiction_kb;
    RuleSet* rs = args->rs;
    int min = args->min;
    int max = args->max;
    
    //see if it's possible to rule out any further worlds
    //proof by contradiction
    for (int player = min; player < max; player++)
    {
        for (int function = 0; function < INT_LENGTH*FUNCTION_RESULT_SIZE; function++)
        {
            int functionNeg;
            // Using the fact KB[2n] = A KB[2n+1] = NOT A
            if (function % 2 == 0)
            {
                functionNeg = function+1;
            }
            else
            {
                functionNeg = function-1;
            }

            if (isKnown(kb, 0, player, function) == 0 && isKnown(kb, 0, player, functionNeg) == 0)
            { //If this is not already assumed
                //Create copy of main space to look for contradiction
                copyTo(contradiction_kb, kb);

                //Assume true
                addKnowledge(contradiction_kb, 0, player, function);

                //Infer knowledge (to see if a contradiction arises)
                if (inferImplicitFacts(contradiction_kb, rs, NUM_SOLVE_STEPS, 0) == 1)
                { //If contradiction found by only adding "function" to assumptions we know NOT function is true 
                    //Update KB
                    printf("FOUND CONTRADICTION: %s(%d:%s) => %s(%d:%s)\n", kb->FUNCTION_NAME[0][function], player, kb->SET_NAMES[0], kb->FUNCTION_NAME[0][functionNeg], player, kb->SET_NAMES[0]);
                    addKnowledge(kb, 0, player, functionNeg);
                }
            }
        }
    }
    return NULL;
}

static void solve(KnowledgeBase* kb, RuleSet* rs, int NUM_PLAYERS, int NUM_MINIONS, int NUM_DEMONS,int BASE_OUTSIDERS, int NUM_DAYS)
{
    char buff[STRING_BUFF_SIZE];
    KnowledgeBase* revert_kb = initKB(NUM_PLAYERS, NUM_DAYS); //For backup incase of contradictions


    int NUM_THREADS = 16;
    KnowledgeBase* thread_kb[NUM_THREADS];
    KnowledgeBase* contradiction_kb[NUM_THREADS];
    //Thread object
    pthread_t threads[NUM_THREADS];
    struct inferFactsByContradictionArgs* threadArgs[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_kb[i] = initKB(NUM_PLAYERS, NUM_DAYS);
        contradiction_kb[i] = initKB(NUM_PLAYERS, NUM_DAYS);
    }

    printf("BEGIN GAME LOOP...\n");

    int contradiction;
    
    while (1)
    {
        //Create copy as backup incase of contradictions
        copyTo(revert_kb, kb);
        add_info(kb, rs, NUM_DAYS);

        printHeading("INFER FACTS"); //UI HEADING
        contradiction = inferImplicitFacts(kb, rs, NUM_SOLVE_STEPS, 1);
        

        /*
        Commented out for being too slow: further optimisation is needed
        if (contradiction == 0)
        { //If no immediate contradictions 
            printHeading("LOOK FOR PROOF BY CONTRADICTION"); //UI HEADING
            for (int i = 0; i < NUM_THREADS; i++)
            {
                copyTo(thread_kb[i], kb);
            }
            for (int i = 0; i < NUM_THREADS; i++)
            {
                
                //Create arguments in strctures to pass into new thread
                threadArgs[i] = (struct inferFactsByContradictionArgs*) malloc(sizeof(struct inferFactsByContradictionArgs));
                
                threadArgs[i]->kb = thread_kb[i];
                threadArgs[i]->contradiction_kb = contradiction_kb[i];
                threadArgs[i]->rs = rs;
                threadArgs[i]->min = (i*NUM_PLAYERS)/NUM_THREADS;
                threadArgs[i]->max = ((i+1)*NUM_PLAYERS)/NUM_THREADS;

                pthread_create(&threads[i], NULL, &inferFactsByContradiction, (void *) threadArgs[i]);
                
            }
            for (int i = 0; i < NUM_THREADS; i++)
            {
                void *aretreive;
                pthread_join(threads[i], &aretreive);
            }
            for (int i = 0; i < NUM_THREADS; i++)
            {
                free(threadArgs[i]);
                mergeKnowledge(kb, thread_kb[i]);
            }
        }
        */

        if (contradiction == 0)
        {
            printHeading("KNOWLEDGE BASE"); //UI HEADING
            printKnowledgeBase(kb);
            for (int night = 0; night < NUM_DAYS; night++)
            {
                snprintf(buff, STRING_BUFF_SIZE, "ROLE INFORMATION [NIGHT %d]", night);
                printHeading(buff); //UI HEADING
                printPlayerTable(kb, night);
                printRoleTable(kb, night);
            }
        }

        if (contradiction == 1)
        {
            printHeading("CONTRADICTION FOUND"); //UI HEADING
            printf("Rolling back Knowledge base\n");
            //Roll back knowledge base
            copyTo(kb, revert_kb);
        }

    }
}

int main()
{
    int NUM_PLAYERS;
    int NUM_MINIONS;
    int NUM_DEMONS;
    int BASE_OUTSIDERS;
    int SCRIPT;

    int NUM_DAYS = 5;
    setup(&NUM_PLAYERS, &NUM_MINIONS, &NUM_DEMONS, &BASE_OUTSIDERS, &SCRIPT);

    printHeading("CREATING GAME..."); //UI HEADING
    printf("There are %d players in the game\n", NUM_PLAYERS);
    printf("There are %d minions in the game\n", NUM_MINIONS);
    printf("There are %d demons in the game\n", NUM_DEMONS);
    printf("There are %d(+2) outsiders in the game\n", BASE_OUTSIDERS);

    printf("INITIALIZE KNOWLEDGE BASE...\n");
    KnowledgeBase* kb = NULL;
    RuleSet* rs = NULL;

    initScript(&rs, &kb, SCRIPT, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS, NUM_DAYS);

    getNames(kb->ELEMENT_NAMES, NUM_PLAYERS);

    printRules(rs, kb);

    solve(kb, rs, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS, NUM_DAYS);

    return 0;
}