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
#include "util.h"
#include "solver.h"

/**
 * inferImplicitFacts() - helper function to iterate inferknowledgeBaseFromRules()
 * with some basic optimisations
 * 
 * @rs the ruleset object
 * @kb the knoweledge base
 * @numRounds the maximium number of infer steps (less steps may be taken if a contradiction is found early or if no new information is found)
 * @verbose if 1 print results
 * 
 * @return TRUE if a contradiction was found
*/
int inferImplicitFacts(KnowledgeBase* kb, RuleSet* rs, int numRounds, int verbose)
{
    //Loop for a maximum number of rounds
    for (int i = 0; i < numRounds; i++)
    {
        int result = inferknowledgeBaseFromRules(rs, kb, verbose);

        if (result == -1) return 1; //Check for contradictions
        if (result == 0) break; //If nothing new was found
    }
    //No contradictions were found
    return 0;
}

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

#define MAX_FALIURES 256

static int assignRoleForWorld(KnowledgeBase* possibleWorldKB, KnowledgeBase*** possibleWorldRevertKB, ProbKnowledgeBase* determinedInNWorlds, RuleSet* rs, int avaliable[5][MAX_SET_ELEMENTS], int night, int playerIndex, int *failures, int permute[], int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES]);

static int assignRoleForWorld(KnowledgeBase* possibleWorldKB, KnowledgeBase*** possibleWorldRevertKB, ProbKnowledgeBase* determinedInNWorlds, RuleSet* rs, int avaliable[5][MAX_SET_ELEMENTS], int night, int playerIndex, int *failures, int permute[], int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES])
{
    //Choose player from random permutation to remove certain biases in allocation
    int player = permute[playerIndex];

    KnowledgeBase* myLayerRevertKB = possibleWorldRevertKB[night][player];

    

    int avaliableRoles = 0;
    int roleAvalaliable[NUM_BOTCT_ROLES];
    for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
    {
        //NOTE: swapped out for faster code
        //snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[roleID], night);
        //int isNotRole = isKnownName(possibleWorldKB, "PLAYERS", player, buff);
        int isNotRole = isKnown(possibleWorldKB, 0, player, notroleIndexes[night][roleID]);

        //NOTE: swapped out for branchless version
        /*
        if (isNotRole == 0)
        {
            roleAvalaliable[roleID] = 1;
            avaliableRoles++;
        } 
        else
        {
            roleAvalaliable[roleID] = 0;
        }
        */
        int roleAvaliable = isNotRole ? 0 : 1;
        roleAvalaliable[roleID] = roleAvaliable;
        avaliableRoles += roleAvaliable;
    }

    copyTo(myLayerRevertKB, possibleWorldKB); //copy to backup to revert later

    while (avaliableRoles > 0)
    {
        int selectedRoleID = getRandIntNotIn(roleAvalaliable, avaliableRoles);

        //Assume true
        //NOTE: swapped out for faster code
        //snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[selectedRoleID], night);
        //addKnowledgeName(possibleWorldKB, "PLAYERS", player, buff);
        addKnowledge(possibleWorldKB, 0, player, isroleIndexes[night][selectedRoleID]);
        //printf("ASSIGNING %s\n", buff);
        //Infer knowledge (to see if a contradiction arises)
        
        if (inferImplicitFacts(possibleWorldKB, rs, NUM_SOLVE_STEPS, 0))
        { //If contradiction found by only adding "function" to assumptions we know NOT function is true 
            *failures = *failures+1;
            if (*failures > MAX_FALIURES) return -1;
        }
        else
        { //If I found a role to assign, try deeper layers
            //Find out how many roles are avaliable 
            //used for computing weights basic on non uniform probabilities later
            avaliable[night][player] = 0;
            //Swapped out for faster code;
            /*
            for (int i = 0; i < NUM_BOTCT_ROLES; i++)
            {    
                if (roleAvalaliable[i]) avaliable[night][player]++;
                
            }
             */
            for (int i = 0; i < NUM_BOTCT_ROLES; i++) avaliable[night][player] += roleAvalaliable[i];

            //Find next player and next night
            int nextPlayerIndex = playerIndex+1;
            int nextNight = night;
            if (nextPlayerIndex >= possibleWorldKB->SET_SIZES[0])
            {
                nextPlayerIndex = 0;
                nextNight++;
            }
            if (nextNight >= NUM_DAYS) return 1; //If no more elements to assign, as this layer was a success return SUCCESS 
            //If there are deeper levels to infer look for them

            //See if deeper level inference leads to a good world
            int result = assignRoleForWorld(possibleWorldKB, possibleWorldRevertKB, determinedInNWorlds, rs, avaliable, nextNight, nextPlayerIndex, failures, permute, isroleIndexes, notroleIndexes);
            if (result == 1) return 1;
            if (*failures > MAX_FALIURES) return -1; 
        }

        //If failed to find a world try a different role
        roleAvalaliable[selectedRoleID] = 0; //Mark this role as unavaliable
        avaliableRoles--; //One less avaliable role now
        copyTo(possibleWorldKB, myLayerRevertKB); //Revert to before inference
    }
    //Failed to find anything
    return -1;
}

/**
 * buildWorld() - true to build some random world,
 * if the world is without any detectable contradictin add to determinedInNWorlds
 * 
 * @possibleWorldKB the knowledge base to build the world in
 * @possibleWorldRevertKB a working memory space to store backups for backtracking
 * @determinedInNWorlds the tally to add the score to if the world works
 * @rs the ruleset
*/
static void buildWorld(KnowledgeBase* possibleWorldKB, KnowledgeBase*** possibleWorldRevertKB, ProbKnowledgeBase* determinedInNWorlds, RuleSet* rs, int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES])
{
    //Build permutations to remove symptoms from always assigning orders in one way
    int permute[possibleWorldKB->SET_SIZES[0]];
    int temp[possibleWorldKB->SET_SIZES[0]];
    for (int i = 0; i < possibleWorldKB->SET_SIZES[0]; i++)
    {
        temp[i] = 1;
    }
    for (int i = 0; i < possibleWorldKB->SET_SIZES[0]; i++)
    {
        int rand = getRandIntNotIn(temp, possibleWorldKB->SET_SIZES[0]-i);
        permute[i] = rand;
        temp[rand] = 0;
    }
    

    int avaliable[5][MAX_SET_ELEMENTS];
    /*
     * IDEA: "Loop" through all important information to try 
     * and build some worlds where every play is assigned a role
     * After each step infer as much information as possible checking to see if there are contradictions
     * IF: there are contradiction BACKTRACK
     * ELSE: continue until all player are assigned roles
     * once/if every player is assigned roles add this to the tally
    */
    int faliures = 0;
    int result = assignRoleForWorld(possibleWorldKB, possibleWorldRevertKB, determinedInNWorlds, rs, avaliable, 0, 0, &faliures, permute, isroleIndexes, notroleIndexes);
    if (result == -1) 
    {
        printf("WORLD HAD TOO MANY CONRADICTIONS\n");
        return; //If no valid world was found
    }


    //Compute weight to account for changes in probability due to backtracking
    double weight = 1000000000.0;
    for (int night = 0; night < 1; night++) //ONLY LOOK AT FIRST NIGHT and only consider roles in script
    {
        for (int player = 0; player < possibleWorldKB->SET_SIZES[0]; player++)
        {
            double term = (float)(avaliable[night][player]) / (float)(NUM_ROLES_IN_SCRIPT);
            weight *= term;
        }
    }
    printf("FOUND WORLD (Scaling Weight=%f)!\n", weight);
    //Add weighted tally
    addKBtoProbTally(possibleWorldKB, determinedInNWorlds, weight);
}

/**
 * getProbApprox() - tries to randomly build worlds to approximate the probability
 * of different events
 * 
 * @void_arg the arguments
 * 
 * @return NULL
*/
static void* getProbApprox(void* void_arg)
{
    //Arguments
    struct getProbApproxArgs *args = (struct getProbApproxArgs*) void_arg; //Get arguments by casting
    //Upack arguments
    KnowledgeBase* kb = args->kb;
    KnowledgeBase* possibleWorldKB = args->possibleWorldKB;
    KnowledgeBase*** possibleWorldRevertKB = args->possibleWorldRevertKB;
    ProbKnowledgeBase* determinedInNWorlds = args->determinedInNWorlds;
    RuleSet* rs = args->rs;
    int numIterations = args->numIterations;

    //Cache role data locations for fast lookup
    char buff[STRING_BUFF_SIZE];
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES];
    int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES];
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
            isroleIndexes[night][role] = getSetFunctionIDWithName(kb, 0, buff, 1);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
            notroleIndexes[night][role] = getSetFunctionIDWithName(kb, 0, buff, 1);
        }
    }
    
    for (int i = 0; i < numIterations; i++)
    {
        copyTo(possibleWorldKB, kb);
        buildWorld(possibleWorldKB, possibleWorldRevertKB, determinedInNWorlds, rs, isroleIndexes, notroleIndexes);
    }
    return NULL;
}

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
void solve(KnowledgeBase* kb, RuleSet* rs, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    char buff[STRING_BUFF_SIZE];
    //Store tallies of possible worlds
    ProbKnowledgeBase* worldTally = initProbKB();

    KnowledgeBase* revertKB = initKB(NUM_PLAYERS); //For backup incase of contradictions


    const int NUM_THREADS = 16;
    const int NUM_ITERATIONS = 256;

    ProbKnowledgeBase* threadTallies[NUM_THREADS];
    KnowledgeBase* possibleWorldKB[NUM_THREADS];
    KnowledgeBase*** possibleWorldTempKB[NUM_THREADS];
    struct getProbApproxArgs* threadArgs[NUM_THREADS];

    //Thread object
    pthread_t threads[NUM_THREADS-1];
    
    for (int i = 0; i < NUM_THREADS; i++)
    {
        possibleWorldKB[i] = initKB(NUM_PLAYERS);
        threadTallies[i] = initProbKB();
        possibleWorldTempKB[i] = (KnowledgeBase***)malloc(NUM_DAYS * sizeof(KnowledgeBase***));
        if (possibleWorldTempKB[i] == NULL)
        {
            printf("MALLOC FAILED!\n");
            return;
        }
        for (int j = 0; j < NUM_DAYS; j++)
        {
            possibleWorldTempKB[i][j] = (KnowledgeBase**)malloc(MAX_SET_ELEMENTS * sizeof(KnowledgeBase**));
            if (possibleWorldTempKB[i][j] == NULL)
            {
                printf("MALLOC FAILED!\n");
                return;
            }
            for (int k = 0; k < MAX_SET_ELEMENTS; k++)
            {
                //possibleWorldTempKB[i][j][k] = (KnowledgeBase*)malloc(NUM_DAYS * sizeof(KnowledgeBase*));
                
                possibleWorldTempKB[i][j][k] = initKB(NUM_PLAYERS);
                if (possibleWorldTempKB[i][j][k] == NULL)
                {
                    printf("MALLOC FAILED!\n");
                    return;
                }
            }
        }
    }


    printf("BEGIN GAME LOOP...\n");

    int contradiction;
    
    while (1)
    {
        //Create copy as backup incase of contradictions
        copyTo(revertKB, kb);
        int runProb = add_info(kb, rs);

        printHeading("INFER FACTS"); //UI HEADING
        contradiction = inferImplicitFacts(kb, rs, NUM_SOLVE_STEPS, 1);
        
        if (contradiction == 0)
        { //Optimise ruleset only if no contradictions were produced
            printHeading("OPTIMISE RULESET");
            optimiseRuleset(rs, kb);
        }
        
        
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
                threadArgs[i]->possibleWorldRevertKB = possibleWorldTempKB[i]; //Working block of memory
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
            printRedHeading("CONTRADICTION FOUND!"); //UI HEADING
            printf("Rolling back Knowledge base\n");
            //Roll back knowledge base
            copyTo(kb, revertKB);
        }

    }
}