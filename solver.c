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
#include "uitest.h"


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

#define MAX_FALIURES 1024

static int assignPoisonForWorld(
    KnowledgeBase* possibleWorldKB, KnowledgeBase**** possibleWorldRevertKB, 
    ProbKnowledgeBase* determinedInNWorlds, 
    RuleSet* rs, 
    int avaliable[5][MAX_SET_ELEMENTS][3], 
    int night, int playerIndex, 
    int *failures, 
    int permute[], 
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], 
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], 
    int isPoisonedIndexes[NUM_DAYS], int isNotPoisonedIndexes[NUM_DAYS],
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS]
);

static int assignKillForWorld(
    KnowledgeBase* possibleWorldKB, KnowledgeBase**** possibleWorldRevertKB, 
    ProbKnowledgeBase* determinedInNWorlds, 
    RuleSet* rs, 
    int avaliable[5][MAX_SET_ELEMENTS][3], 
    int night, int playerIndex, 
    int *failures, 
    int permute[], 
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], 
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS],
    int isPoisonedIndexes[NUM_DAYS], int isNotPoisonedIndexes[NUM_DAYS], 
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS]
);

static int assignRoleForWorld(
    KnowledgeBase* possibleWorldKB, KnowledgeBase**** possibleWorldRevertKB, 
    ProbKnowledgeBase* determinedInNWorlds, 
    RuleSet* rs, 
    int avaliable[5][MAX_SET_ELEMENTS][3], 
    int night, int playerIndex, 
    int *failures, 
    int permute[], 
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], 
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], 
    int isPoisonedIndexes[NUM_DAYS], int isNotPoisonedIndexes[NUM_DAYS],
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS]
);

static int assignPoisonForWorld(
    KnowledgeBase* possibleWorldKB, KnowledgeBase**** possibleWorldRevertKB, 
    ProbKnowledgeBase* determinedInNWorlds, 
    RuleSet* rs, 
    int avaliable[5][MAX_SET_ELEMENTS][3], 
    int night, int playerIndex, 
    int *failures, 
    int permute[], 
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], 
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS],
    int isPoisonedIndexes[NUM_DAYS], int isNotPoisonedIndexes[NUM_DAYS],
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS]
)
{
    //Choose player from random permutation to remove certain biases in allocation
    int player = permute[playerIndex];

    KnowledgeBase* myLayerRevertKB = possibleWorldRevertKB[night][player][2];

    int avaliableActions = 0;
    int actionsAvalaliable[NUM_BOTCT_ROLES];

    //You can always pick the "do nothing" action
    actionsAvalaliable[0] = 1;
    avaliableActions++;
    for (int playerToAction = 0; playerToAction < possibleWorldKB->SET_SIZES[0]; playerToAction++)
    {
        int notPoisonedPlayers = isKnown(possibleWorldKB, 0, player, notPoisonedIndexes[night][playerToAction]);

        int roleAvaliable = notPoisonedPlayers ? 0 : 1;
        actionsAvalaliable[playerToAction+1] = roleAvaliable;
        avaliableActions += roleAvaliable;
    }

    copyTo(myLayerRevertKB, possibleWorldKB); //copy to backup to revert later

    while (avaliableActions > 0)
    {
        int playerToActionID = getRandIntNotIn(actionsAvalaliable, avaliableActions);

        //Assume true
        if (playerToActionID != 0)
        {
            addKnowledge(possibleWorldKB, 0, player, poisonedIndexes[night][playerToActionID-1]);
        }

        if (playerIndex+1 >= possibleWorldKB->SET_SIZES[0])
        {
            for (int poisonedPlayerID = 0; poisonedPlayerID < possibleWorldKB->SET_SIZES[0]; poisonedPlayerID++)
            {
                int playerPoisoned = 0;
                for (int playerID = 0; playerID < possibleWorldKB->SET_SIZES[0]; playerID++)
                {
                    int poisonedPlayers = isKnown(possibleWorldKB, 0, playerID, poisonedIndexes[night][poisonedPlayerID]);
                    if (poisonedPlayers == 0)
                    {
                        addKnowledge(possibleWorldKB, 0, playerID, notPoisonedIndexes[night][poisonedPlayerID]);
                    }
                    else
                    {
                        playerPoisoned = 1;
                    }
                }
                if (playerPoisoned == 0)
                { //Add code to say the player isn't poisoned
                    addKnowledge(possibleWorldKB, 0, poisonedPlayerID, isNotPoisonedIndexes[night]);
                }
            }
        }
            

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
            avaliable[night][player][2] = 0;

            //Swapped out for faster code;
            for (int i = 0; i < possibleWorldKB->SET_SIZES[0]+1; i++) avaliable[night][player][2] += actionsAvalaliable[i];

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
            int result = assignRoleForWorld(
                possibleWorldKB, possibleWorldRevertKB, 
                determinedInNWorlds, 
                rs, 
                avaliable, 
                nextNight, nextPlayerIndex, 
                failures, 
                permute, 
                isroleIndexes, notroleIndexes, 
                poisonedIndexes, notPoisonedIndexes,
                isPoisonedIndexes, isNotPoisonedIndexes,
                killedIndexes, notKilledIndexes
            );
            if (result == 1) return 1;
            if (*failures > MAX_FALIURES) return -1; 
        }

        //If failed to find a world try a different role
        actionsAvalaliable[playerToActionID] = 0; //Mark this role as unavaliable
        avaliableActions--; //One less avaliable role now
        copyTo(possibleWorldKB, myLayerRevertKB); //Revert to before inference
    }
    //Failed to find anything
    return -1;
}

static int assignKillForWorld(
    KnowledgeBase* possibleWorldKB, KnowledgeBase**** possibleWorldRevertKB, 
    ProbKnowledgeBase* determinedInNWorlds, 
    RuleSet* rs, 
    int avaliable[5][MAX_SET_ELEMENTS][3], 
    int night, int playerIndex, 
    int *failures, 
    int permute[], 
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], 
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS],
    int isPoisonedIndexes[NUM_DAYS], int isNotPoisonedIndexes[NUM_DAYS], 
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS]
)
{
    //Choose player from random permutation to remove certain biases in allocation
    int player = permute[playerIndex];

    KnowledgeBase* myLayerRevertKB = possibleWorldRevertKB[night][player][1];

    int avaliableActions = 0;
    int actionsAvalaliable[NUM_BOTCT_ROLES];

    //You can always pick the "do nothing" action
    actionsAvalaliable[0] = 1;
    avaliableActions++;
    for (int playerToAction = 0; playerToAction < possibleWorldKB->SET_SIZES[0]; playerToAction++)
    {
        int notKilledPlayers = isKnown(possibleWorldKB, 0, player, notKilledIndexes[night][playerToAction]);

        int roleAvaliable = notKilledPlayers ? 0 : 1;
        actionsAvalaliable[playerToAction+1] = roleAvaliable;
        avaliableActions += roleAvaliable;
    }

    copyTo(myLayerRevertKB, possibleWorldKB); //copy to backup to revert later

    while (avaliableActions > 0)
    {
        int playerToActionID = getRandIntNotIn(actionsAvalaliable, avaliableActions);

        //Assume true
        if (playerToActionID != 0)
        {
            addKnowledge(possibleWorldKB, 0, player, killedIndexes[night][playerToActionID-1]);
        }
        if (playerIndex+1 >= possibleWorldKB->SET_SIZES[0])
        {
            for (int killedPlayerID = 0; killedPlayerID < possibleWorldKB->SET_SIZES[0]; killedPlayerID++)
            {
                int playerKilled = 0;
                for (int playerID = 0; playerID < possibleWorldKB->SET_SIZES[0]; playerID++)
                {
                    int killedPlayers = isKnown(possibleWorldKB, 0, playerID, killedIndexes[night][killedPlayerID]);
                    if (killedPlayers == 0)
                    {
                        addKnowledge(possibleWorldKB, 0, playerID, notKilledIndexes[night][killedPlayerID]);
                    }
                    else
                    {
                        playerKilled = 1;
                    }
                }
                if (playerKilled == 0)
                { //Add code to say the player isn't killed
                    //NOTE: killing is difficult as the player may die in other ways
                    //addKnowledge(possibleWorldKB, 0, killedPlayerID, isNotPoisonedIndexes[night]);
                }
            }
        }
            

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
            avaliable[night][player][1] = 0;

            //Swapped out for faster code;
            for (int i = 0; i < possibleWorldKB->SET_SIZES[0]+1; i++) avaliable[night][player][1] += actionsAvalaliable[i];

            //Find next player and next night
            /*
            int nextPlayerIndex = playerIndex+1;
            int nextNight = night;
            if (nextPlayerIndex >= possibleWorldKB->SET_SIZES[0])
            {
                nextPlayerIndex = 0;
                nextNight++;
            }
            if (nextNight >= NUM_DAYS) return 1; //If no more elements to assign, as this layer was a success return SUCCESS
            */ 
            //If there are deeper levels to infer look for them

            //See if deeper level inference leads to a good world
            int result = assignPoisonForWorld(
                possibleWorldKB, possibleWorldRevertKB, 
                determinedInNWorlds, 
                rs, 
                avaliable, 
                night, playerIndex, 
                failures, 
                permute, 
                isroleIndexes, notroleIndexes, 
                poisonedIndexes, notPoisonedIndexes,
                isPoisonedIndexes, isNotPoisonedIndexes,
                killedIndexes, notKilledIndexes
            );
            if (result == 1) return 1;
            if (*failures > MAX_FALIURES) return -1; 
        }

        //If failed to find a world try a different role
        actionsAvalaliable[playerToActionID] = 0; //Mark this role as unavaliable
        avaliableActions--; //One less avaliable role now
        copyTo(possibleWorldKB, myLayerRevertKB); //Revert to before inference
    }
    //Failed to find anything
    return -1;
}

static int assignRoleForWorld(
    KnowledgeBase* possibleWorldKB, KnowledgeBase**** possibleWorldRevertKB, 
    ProbKnowledgeBase* determinedInNWorlds, 
    RuleSet* rs, 
    int avaliable[5][MAX_SET_ELEMENTS][3], 
    int night, int playerIndex, 
    int *failures, 
    int permute[], 
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], 
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS],
    int isPoisonedIndexes[NUM_DAYS], int isNotPoisonedIndexes[NUM_DAYS], 
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS]
)
{
    //printf("TEST player %d, night %d\n", playerIndex, night);
    //Choose player from random permutation to remove certain biases in allocation
    int player = permute[playerIndex];

    KnowledgeBase* myLayerRevertKB = possibleWorldRevertKB[night][player][0];

    int avaliableRoles = 0;
    int roleAvalaliable[NUM_BOTCT_ROLES];
    for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
    {
        int isNotRole = isKnown(possibleWorldKB, 0, player, notroleIndexes[night][roleID]);

        int roleAvaliable = isNotRole ? 0 : 1;
        roleAvalaliable[roleID] = roleAvaliable;
        avaliableRoles += roleAvaliable;
    }

    copyTo(myLayerRevertKB, possibleWorldKB); //copy to backup to revert later

    while (avaliableRoles > 0)
    {
        int selectedRoleID = getRandIntNotIn(roleAvalaliable, avaliableRoles);

        //Assume true
        addKnowledge(possibleWorldKB, 0, player, isroleIndexes[night][selectedRoleID]);

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
            avaliable[night][player][0] = 0;

            //Swapped out for faster code;
            for (int i = 0; i < NUM_BOTCT_ROLES; i++) avaliable[night][player][0] += roleAvalaliable[i];

            //Find next player and next night
            /*
            int nextPlayerIndex = playerIndex+1;
            int nextNight = night;
            if (nextPlayerIndex >= possibleWorldKB->SET_SIZES[0])
            {
                nextPlayerIndex = 0;
                nextNight++;
            }
            if (nextNight >= NUM_DAYS) return 1; //If no more elements to assign, as this layer was a success return SUCCESS
            */ 
            //If there are deeper levels to infer look for them

            //See if deeper level inference leads to a good world
            int result = assignKillForWorld(
                possibleWorldKB, possibleWorldRevertKB, 
                determinedInNWorlds, 
                rs, 
                avaliable, 
                night, playerIndex, 
                failures, 
                permute, 
                isroleIndexes, notroleIndexes, 
                poisonedIndexes, notPoisonedIndexes,
                isPoisonedIndexes, isNotPoisonedIndexes,
                killedIndexes, notKilledIndexes
            );
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
static void buildWorld(
    KnowledgeBase* possibleWorldKB, KnowledgeBase**** possibleWorldRevertKB, 
    ProbKnowledgeBase* determinedInNWorlds, 
    CachedKnowledgeBases* POSSIBLE_WORLDS_FOR_PROB, int (*POSSIBLE_WORLD_GENERATED)[MAX_SET_ELEMENTS][NUM_BOTCT_ROLES][NUM_DAYS],
    RuleSet* rs, 
    int myGeneration, int *worldGeneration,
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES], 
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS],
    int isPoisonedIndexes[NUM_DAYS], int isNotPoisonedIndexes[NUM_DAYS],
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS], int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS]

)
{

    //printf("GEN WORLD\n");
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
    

    int avaliable[5][MAX_SET_ELEMENTS][3];
    /*
     * IDEA: "Loop" through all important information to try 
     * and build some worlds where every play is assigned a role
     * After each step infer as much information as possible checking to see if there are contradictions
     * IF: there are contradiction BACKTRACK
     * ELSE: continue until all player are assigned roles
     * once/if every player is assigned roles add this to the tally
    */
    int faliures = 0;
    int result = assignRoleForWorld(
        possibleWorldKB, possibleWorldRevertKB, 
        determinedInNWorlds, 
        rs, 
        avaliable, 
        0, 0, //night, player index 
        &faliures, 
        permute, 
        isroleIndexes, notroleIndexes, 
        poisonedIndexes, notPoisonedIndexes,
        isPoisonedIndexes, isNotPoisonedIndexes,
        killedIndexes, notKilledIndexes
    );
    if (result == -1) 
    {
        printf("WORLD HAD TOO MANY CONRADICTIONS\n");
        return; //If no valid world was found
    }
    //printf("ADD WEIGHT\n");

    //Compute weight to account for changes in probability due to backtracking
    double weight = 1000000000.0;
    for (int night = 0; night < 1; night++) //ONLY LOOK AT FIRST NIGHT and only consider roles in script
    {
        for (int player = 0; player < possibleWorldKB->SET_SIZES[0]; player++)
        {
            double roleTerm = (float)(avaliable[night][player][0]) / (float)(NUM_ROLES_IN_SCRIPT);
            double killTerm = (float)(avaliable[night][player][1]) / (float)(possibleWorldKB->SET_SIZES[0]+1);
            double poisonTerm = (float)(avaliable[night][player][2]) / (float)(possibleWorldKB->SET_SIZES[0]+1);
            //printf("(%f, %f)", roleTerm, killTerm);
            weight *= roleTerm * killTerm * poisonTerm;
        }
    }
    //printf("\n");
    printf("FOUND WORLD (Scaling Weight=%f)!\n", weight);
    //Add weighted tally
    addKBtoProbTally(possibleWorldKB, determinedInNWorlds, weight);

    pthread_mutex_lock(&cacheworldlock);   // Lock before accessing shared data
        // Critical section 
        if (myGeneration == *worldGeneration)
        {
            int location = addKBToCache(POSSIBLE_WORLDS_FOR_PROB, possibleWorldKB, weight);
        
            for (int night = 0; night < NUM_DAYS; night++)
            {
                for (int player = 0; player < possibleWorldKB->SET_SIZES[0]; player++)
                {
                    int role = 0;
                    while (role < NUM_BOTCT_ROLES)
                    {
                        int isRole = isKnown(possibleWorldKB, 0, player, isroleIndexes[night][role]);
                        if (isRole) break;
                        role++;
                    }
                    if ((*POSSIBLE_WORLD_GENERATED)[player][role][night] == -1) (*POSSIBLE_WORLD_GENERATED)[player][role][night] = location;
                }
            }
        }
    pthread_mutex_unlock(&cacheworldlock); // Unlock after done
    

    
    
}

void* getProbApproxContinuous(void* void_arg)
{
    //Arguments
    struct getProbApproxArgs *args = (struct getProbApproxArgs*) void_arg; //Get arguments by casting
    //Upack arguments
    KnowledgeBase* kb = args->kb;
    KnowledgeBase* possibleWorldKB = args->possibleWorldKB;
    KnowledgeBase**** possibleWorldRevertKB = args->possibleWorldRevertKB;
    ProbKnowledgeBase* determinedInNWorlds = args->determinedInNWorlds;
    ProbKnowledgeBase* worldTally = args->worldTally;
    CachedKnowledgeBases* POSSIBLE_WORLDS_FOR_PROB = args->POSSIBLE_WORLDS_FOR_PROB;
    int (*POSSIBLE_WORLD_GENERATED)[MAX_SET_ELEMENTS][NUM_BOTCT_ROLES][NUM_DAYS] = args->POSSIBLE_WORLD_GENERATED;
    RuleSet* rs = args->rs;
    int* worldGeneration = args->worldGeneration;
    bool* reRenderCall = args->reRenderCall;
    int numIterations = args->numIterations;

    //Cache role data locations for fast lookup
    char buff[STRING_BUFF_SIZE];
    int isroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES];
    int notroleIndexes[NUM_DAYS][NUM_BOTCT_ROLES];
    int poisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS];
    int notPoisonedIndexes[NUM_DAYS][MAX_SET_ELEMENTS];
    int isPoisonedIndexes[NUM_DAYS];
    int isNotPoisonedIndexes[NUM_DAYS];
    int killedIndexes[NUM_DAYS][MAX_SET_ELEMENTS];
    int notKilledIndexes[NUM_DAYS][MAX_SET_ELEMENTS];
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
            
            isroleIndexes[night][role] = getSetFunctionIDWithName(kb, 0, buff, 1);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
            
            notroleIndexes[night][role] = getSetFunctionIDWithName(kb, 0, buff, 1);
        }

        snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
        
        isPoisonedIndexes[night] = getSetFunctionIDWithName(kb, 0, buff, 1);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        
        isNotPoisonedIndexes[night] = getSetFunctionIDWithName(kb, 0, buff, 1);
        
        for (int playerID = 0; playerID < kb->SET_SIZES[0]; playerID++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", playerID, night);
            
            killedIndexes[night][playerID] = getSetFunctionIDWithName(kb, 0, buff, 1);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", playerID, night);
            
            notKilledIndexes[night][playerID] = getSetFunctionIDWithName(kb, 0, buff, 1);
            snprintf(buff, STRING_BUFF_SIZE, "POISONED_%d_[NIGHT%d]", playerID, night);
            
            poisonedIndexes[night][playerID] = getSetFunctionIDWithName(kb, 0, buff, 1);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_POISONED_%d_[NIGHT%d]", playerID, night);
            
            notPoisonedIndexes[night][playerID] = getSetFunctionIDWithName(kb, 0, buff, 1);
        }
    }

    int myGeneration = *worldGeneration;
    
    //Loop forever adding 
    while (1)
    {
        resetProbKnowledgeBase(determinedInNWorlds);
        for (int i = 0; i < numIterations; i++)
        {
            if (myGeneration != *worldGeneration) break;
            copyTo(possibleWorldKB, kb);
            buildWorld(
                possibleWorldKB, possibleWorldRevertKB, 
                determinedInNWorlds, 
                POSSIBLE_WORLDS_FOR_PROB, POSSIBLE_WORLD_GENERATED, 
                rs, 
                myGeneration, worldGeneration,
                isroleIndexes, notroleIndexes, 
                poisonedIndexes, notPoisonedIndexes, 
                isPoisonedIndexes, isNotPoisonedIndexes,
                killedIndexes, notKilledIndexes
            );
        }

        if (myGeneration == *worldGeneration) 
        {
            pthread_mutex_lock(&problock);   // Lock before accessing shared data 
                // Critical section
                if (myGeneration == *worldGeneration) 
                {                 
                    mergeProbKnowledge(worldTally, determinedInNWorlds);
                    *reRenderCall = true;
                }
            pthread_mutex_unlock(&problock); // Unlock after done

            
        }
        else 
        {
            myGeneration = *worldGeneration;
        }
    }

    return NULL;
}