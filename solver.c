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

void solve(KnowledgeBase* kb, RuleSet* rs, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS, const int NUM_DAYS)
{
    char buff[STRING_BUFF_SIZE];
    //Store tallies of possible worlds
    ProbKnowledgeBase* worldTally = initProbKB();

    KnowledgeBase* revertKB = initKB(NUM_PLAYERS, NUM_DAYS); //For backup incase of contradictions


    const int NUM_THREADS = 16;
    const int NUM_ITERATIONS = 16;

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