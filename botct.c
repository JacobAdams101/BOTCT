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

static int getRandInt(int min, int max)
{
    return (rand() % (max - min)) + min;
}

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

struct getProbApproxArgs
{
    KnowledgeBase* kb;
    KnowledgeBase* contradiction_kb;
    ProbKnowledgeBase* determinedInNWorlds;
    RuleSet* rs;
    int numIterations;
    //int min;
    //int max;
};
static void buildWorld(KnowledgeBase* contradiction_kb, ProbKnowledgeBase* determinedInNWorlds, RuleSet* rs)
{
    char buff[64];
    /*
     * IDEA: Loop through all important information to try 
     * and build some worlds where every play is assigned a role
     * After each step infer as much information as possible checking to see if there are contradictions
     * IF: there are contradiction STOP
     * ELSE: continue until all player are assigned roles
     * once/if every player is assigned roles add this to the tally
    */
    for (int night = 0; night < 5; night++)
    {
        
        for (int player = 0; player < contradiction_kb->SET_SIZES[0]; player++)
        {
            //printf("NIGHT %d PLAYER %d\n", night, player);
            int avaliableRoles = 0;
            for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
            {
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[roleID], night);
                int isNotRole = isKnownName(contradiction_kb, "PLAYERS", player, buff);
                if (isNotRole == 0)
                {
                    avaliableRoles++;
                } 
            }
            if (avaliableRoles > 1)
            {
                int rand = getRandInt(0, avaliableRoles);
                int selectedRoleID = -1;
                int isNotRole;
                do
                {
                    selectedRoleID++;
                    snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[selectedRoleID], night);
                    isNotRole = isKnownName(contradiction_kb, "PLAYERS", player, buff);
                    if (isNotRole == 0)
                    {
                        rand--;
                    } 
                    
                } while (rand >= 0);

                //Assume true
                snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[selectedRoleID], night);
                addKnowledgeName(contradiction_kb, "PLAYERS", player, buff);
                //Infer knowledge (to see if a contradiction arises)
                
                if (inferImplicitFacts(contradiction_kb, rs, NUM_SOLVE_STEPS/2, 0) == 1)
                { //If contradiction found by only adding "function" to assumptions we know NOT function is true 
                    printf("WORLD HAD CONRADICTION!\n");
                    return;
                }
                    
                
            }
        }
    }
    printf("FOUND WORLD!\n");
    addKBtoProbTally(contradiction_kb, determinedInNWorlds);
}
static void* getProbApprox(void* void_arg)
{
    //Arguments
    struct getProbApproxArgs *args = (struct getProbApproxArgs*) void_arg; //Get arguments by casting
    //Upack arguments
    KnowledgeBase* kb = args->kb;
    KnowledgeBase* contradiction_kb = args->contradiction_kb;
    ProbKnowledgeBase* determinedInNWorlds = args->determinedInNWorlds;
    RuleSet* rs = args->rs;
    int numIterations = args->numIterations;

    
    for (int i = 0; i < numIterations; i++)
    {
        copyTo(contradiction_kb, kb);
        buildWorld(contradiction_kb, determinedInNWorlds, rs);
    }
    return NULL;
}
/*
static int min(int a, int b)
{
    return a < b ? a : b;
}
*/

static void solve(KnowledgeBase* kb, RuleSet* rs, int NUM_PLAYERS, int NUM_MINIONS, int NUM_DEMONS,int BASE_OUTSIDERS, int NUM_DAYS)
{
    char buff[STRING_BUFF_SIZE];
    //Store tallies of possible worlds
    ProbKnowledgeBase* worldTally = initProbKB();

    KnowledgeBase* revert_kb = initKB(NUM_PLAYERS, NUM_DAYS); //For backup incase of contradictions


    int NUM_THREADS = 16;
    int NUM_ITERATIONS = 32;

    ProbKnowledgeBase* thread_tallies[NUM_THREADS];
    KnowledgeBase* contradiction_kb[NUM_THREADS];
    //Thread object
    pthread_t threads[NUM_THREADS];
    struct getProbApproxArgs* threadArgs[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        contradiction_kb[i] = initKB(NUM_PLAYERS, NUM_DAYS);
        thread_tallies[i] = initProbKB();
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
        

        
        //Commented out for being too slow: further optimisation is needed
        //NOTE: Re-added after optimisation


        //Probability analysis
        /*
         * IDEA: try setting random elements to true see if the world produces any contradictions
         * tally up all TRUE functions in worlds without contradictions compute prob estimate off that
        */
        if (contradiction == 0)
        { //If no immediate contradictions 
            printHeading("PROBABILITY ANALYSIS"); //UI HEADING
            for (int i = 0; i < NUM_THREADS; i++)
            {
                resetProbKnowledgeBase(thread_tallies[i]);
            }
            for (int i = 0; i < NUM_THREADS; i++)
            {
                
                //Create arguments in strctures to pass into new thread
                threadArgs[i] = (struct getProbApproxArgs*) malloc(sizeof(struct getProbApproxArgs));
                
                threadArgs[i]->kb = kb; //We MUST promise to never touch this in the thread
                threadArgs[i]->contradiction_kb = contradiction_kb[i]; //Working block of memory
                threadArgs[i]->determinedInNWorlds = thread_tallies[i]; //The output tallies
                threadArgs[i]->rs = rs;
                threadArgs[i]->numIterations = NUM_ITERATIONS;

                pthread_create(&threads[i], NULL, &getProbApprox, (void *) threadArgs[i]);
                
            }
            //Wait for all threads to finish
            for (int i = 0; i < NUM_THREADS; i++)
            {
                void *aretreive;
                pthread_join(threads[i], &aretreive);
            }
            //Compute total tally
            resetProbKnowledgeBase(worldTally);
            for (int i = 0; i < NUM_THREADS; i++)
            {
                free(threadArgs[i]);
                mergeProbKnowledge(worldTally, thread_tallies[i]);
            }
        }
        

        if (contradiction == 0)
        {
            printHeading("KNOWLEDGE BASE"); //UI HEADING
            printKnowledgeBase(kb);
            for (int night = 0; night < NUM_DAYS; night++)
            {
                snprintf(buff, STRING_BUFF_SIZE, "ROLE INFORMATION [NIGHT %d]", night);
                printHeading(buff); //UI HEADING
                //No longer using deterministic values
                printPlayerTable(kb, night);
                printRoleTable(kb, night);
            }
            for (int night = 0; night < NUM_DAYS; night++)
            {
                snprintf(buff, STRING_BUFF_SIZE, "ROLE INFORMATION [NIGHT %d]", night);
                printHeading(buff); //UI HEADING
                //No longer using deterministic values
                //printPlayerTable(kb, night);
                //printRoleTable(kb, night);
                printProbPlayerTable(kb, worldTally, night);
                printProbRoleTable(kb, worldTally, night);
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
    srand(time(NULL));

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