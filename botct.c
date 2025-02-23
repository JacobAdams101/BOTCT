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

        if (hasExplicitContradiction(kb)) {return 1;} //Check for contradictions
        if (foundNovelSolution == 0) {break;} //If nothing new was found
    }
    //No contradictions were found
    return 0;
}

struct getProbApproxArgs
{
    KnowledgeBase* kb;
    KnowledgeBase* possibleWorldKB;
    KnowledgeBase* possibleWorldTempKB;
    ProbKnowledgeBase* determinedInNWorlds;
    RuleSet* rs;
    int numIterations;
    //int min;
    //int max;
};
static void buildWorld(KnowledgeBase* possibleWorldKB, KnowledgeBase* possibleWorldRevertKB, ProbKnowledgeBase* determinedInNWorlds, RuleSet* rs)
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
        for (int player = 0; player < possibleWorldKB->SET_SIZES[0]; player++)
        {
            //printf("NIGHT %d PLAYER %d!\n", night, player);
            //printf("NIGHT %d PLAYER %d\n", night, player);
            int avaliableRoles = 0;
            int roleAvalaliable[NUM_BOTCT_ROLES];
            for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
            {
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[roleID], night);
                int isNotRole = isKnownName(possibleWorldKB, "PLAYERS", player, buff);
                //printf("%s IS AVALIABLE %d!\n", buff, isNotRole);
                if (isNotRole == 0)
                {
                    roleAvalaliable[roleID] = 1;
                    avaliableRoles++;
                } 
                else
                {
                    roleAvalaliable[roleID] = 0;
                }
            }
            int foundRoleToAssign = 0;
            while (foundRoleToAssign == 0)
            {
                if (avaliableRoles > 0)
                {
                    int rand = getRandInt(0, avaliableRoles);
                    //printf("RANDOM %d!\n", rand);
                    int selectedRoleID = 0;
                    //Find avaliable role 
                    while(roleAvalaliable[selectedRoleID] == 0 || rand > 0)
                    {
                        if (roleAvalaliable[selectedRoleID] == 1)
                        {
                            rand--;
                        } 
                        selectedRoleID++;
                    }

                    copyTo(possibleWorldRevertKB, possibleWorldKB); //copy to backup to revert later
                    //Assume true
                    snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[selectedRoleID], night);
                    addKnowledgeName(possibleWorldKB, "PLAYERS", player, buff);
                    //printf("ASSIGNING %s\n", buff);
                    //Infer knowledge (to see if a contradiction arises)
                    
                    if (inferImplicitFacts(possibleWorldKB, rs, NUM_SOLVE_STEPS, 0) == 1)
                    { //If contradiction found by only adding "function" to assumptions we know NOT function is true 
                        roleAvalaliable[selectedRoleID] = 0; //Mark this role as unavaliable
                        avaliableRoles--; //One less avaliable role now
                        copyTo(possibleWorldKB, possibleWorldRevertKB); //Revert to before inference
                        //printf("WORLD HAD CONRADICTION!\n");
                    }
                    else
                    { //If I found a role to assign
                        foundRoleToAssign = 1;
                        //printf("FOUND MATCH!\n");
                    }

                }
                else
                {
                    printf("WORLD HAD TOO MANY CONRADICTIONS!\n");
                    return;
                }
            }
        }
    }
    printf("FOUND WORLD!\n");
    addKBtoProbTally(possibleWorldKB, determinedInNWorlds);
}
static void* getProbApprox(void* void_arg)
{
    //Arguments
    struct getProbApproxArgs *args = (struct getProbApproxArgs*) void_arg; //Get arguments by casting
    //Upack arguments
    KnowledgeBase* kb = args->kb;
    KnowledgeBase* possibleWorldKB = args->possibleWorldKB;
    KnowledgeBase* possibleWorldTempKB = args->possibleWorldTempKB;
    ProbKnowledgeBase* determinedInNWorlds = args->determinedInNWorlds;
    RuleSet* rs = args->rs;
    int numIterations = args->numIterations;

    
    for (int i = 0; i < numIterations; i++)
    {
        copyTo(possibleWorldKB, kb);
        buildWorld(possibleWorldKB, possibleWorldTempKB, determinedInNWorlds, rs);
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

    KnowledgeBase* revertKB = initKB(NUM_PLAYERS, NUM_DAYS); //For backup incase of contradictions


    int NUM_THREADS = 16;
    int NUM_ITERATIONS = 16;

    ProbKnowledgeBase* threadTallies[NUM_THREADS];
    KnowledgeBase* possibleWorldKB[NUM_THREADS];
    KnowledgeBase* possibleWorldTempKB[NUM_THREADS];
    struct getProbApproxArgs* threadArgs[NUM_THREADS];

    //Thread object
    pthread_t threads[NUM_THREADS-1];
    
    for (int i = 0; i < NUM_THREADS; i++)
    {
        possibleWorldKB[i] = initKB(NUM_PLAYERS, NUM_DAYS);
        possibleWorldTempKB[i] = initKB(NUM_PLAYERS, NUM_DAYS);
        threadTallies[i] = initProbKB();
    }

    printf("BEGIN GAME LOOP...\n");

    int contradiction;
    
    while (1)
    {
        //Create copy as backup incase of contradictions
        copyTo(revertKB, kb);
        int runProb = add_info(kb, rs, NUM_DAYS);

        printHeading("INFER FACTS"); //UI HEADING
        contradiction = inferImplicitFacts(kb, rs, NUM_SOLVE_STEPS, 1);
        

        
        //Commented out for being too slow: further optimisation is needed
        //NOTE: Re-added after optimisation
        //Probability analysis
        /*
         * IDEA: try setting random elements to true see if the world produces any contradictions
         * tally up all TRUE functions in worlds without contradictions compute prob estimate off that
        */
        if (runProb == 1 && contradiction == 0)
        { //If no immediate contradictions 
            printHeading("PROBABILITY ANALYSIS"); //UI HEADING
            for (int i = 0; i < NUM_THREADS; i++)
            {
                resetProbKnowledgeBase(threadTallies[i]);
            }
            //Generate NUM_THREADS thread arguments
            for (int i = 0; i < NUM_THREADS; i++)
            {
                
                //Create arguments in strctures to pass into new thread
                threadArgs[i] = (struct getProbApproxArgs*) malloc(sizeof(struct getProbApproxArgs));
                
                threadArgs[i]->kb = kb; //We MUST promise to never touch this in the thread
                threadArgs[i]->possibleWorldKB = possibleWorldKB[i]; //Working block of memory
                threadArgs[i]->possibleWorldTempKB = possibleWorldTempKB[i]; //Working block of memory
                threadArgs[i]->determinedInNWorlds = threadTallies[i]; //The output tallies
                threadArgs[i]->rs = rs;
                threadArgs[i]->numIterations = NUM_ITERATIONS;

                
                
            }
            //Set off NUM_THREADS-1 threads
            for (int i = 0; i < NUM_THREADS-1; i++)
            {
                pthread_create(&threads[i], NULL, &getProbApprox, (void *) threadArgs[i]);
            }

            //Run one on this thread
            getProbApprox((void *) threadArgs[NUM_THREADS-1]);

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
                mergeProbKnowledge(worldTally, threadTallies[i]);
            }
        }
        

        if (contradiction == 0)
        {
            printHeading("KNOWLEDGE BASE"); //UI HEADING
            printKnowledgeBase(kb);
            
            if (runProb == 1)
            {
                for (int night = 0; night < NUM_DAYS; night++)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "ROLE INFORMATION [NIGHT %d]", night);
                    printHeading(buff); //UI HEADING
                    printProbPlayerTable(kb, worldTally, night);
                    printProbRoleTable(kb, worldTally, night);
                }
            }
            else
            {
                for (int night = 0; night < NUM_DAYS; night++)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "ROLE INFORMATION [NIGHT %d]", night);
                    printHeading(buff); //UI HEADING
                    printPlayerTable(kb, night);
                    printRoleTable(kb, night);
                }
            }
        }

        if (contradiction == 1)
        {
            printHeading("CONTRADICTION FOUND"); //UI HEADING
            printf("Rolling back Knowledge base\n");
            //Roll back knowledge base
            copyTo(kb, revertKB);
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