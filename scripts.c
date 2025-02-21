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

#include "scripts.h"
#include "constants.h"
#include "rules.h"

char *ROLE_NAMES[NUM_BOTCT_ROLES];
char *ROLE_TEAMS[NUM_BOTCT_ROLES];
char *ROLE_CLASSES[NUM_BOTCT_ROLES];

int ROLE_IN_SCRIPT[NUM_BOTCT_ROLES];

int TOTAL_MINIONS = 12;
int TOTAL_OUTSIDERS = 12;
int FIRST_MINION_INDEX = 1;
int FIRST_OUTSIDER_INDEX = 18;

void initRoleStrings(int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        ROLE_NAMES[i] = (char*) malloc(SIZE);
        ROLE_TEAMS[i] = (char*) malloc(SIZE);
        ROLE_CLASSES[i] = (char*) malloc(SIZE);
    }
}

void addRole(int *index, char* name, char* team, char* class, int roleInScript, int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    snprintf(ROLE_NAMES[*index], SIZE, "%s", name);
    snprintf(ROLE_TEAMS[*index], SIZE, "%s", team);
    snprintf(ROLE_CLASSES[*index], SIZE, "%s", class);
    ROLE_IN_SCRIPT[*index] = roleInScript;
    
    *index = *index + 1;
}

void initScript(RuleSet** rs, KnowledgeBase** kb, const int SCRIPT, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS, const int NUM_DAYS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];

    printf("NAME THINGS...\n");
    int count = 0;
    initRoleStrings(64);

    int TB = 0;
    int SV = 1;
    int BMR = 2;

    
    //Roles
    //Demons
    //TB
    addRole(&count, "IMP", "EVIL", "DEMON", SCRIPT==TB, 64);
    //S&V
    addRole(&count, "FANG_GU", "EVIL", "DEMON", SCRIPT==SV, 64);
    addRole(&count, "VIGORMORTIS", "EVIL", "DEMON", SCRIPT==SV, 64);
    addRole(&count, "NO_DASHII", "EVIL", "DEMON", SCRIPT==SV, 64);
    addRole(&count, "VORTOX", "EVIL", "DEMON", SCRIPT==SV, 64);
    //BMR
    addRole(&count, "ZOMBUUL", "EVIL", "DEMON", SCRIPT==BMR, 64);
    addRole(&count, "PUKKA", "EVIL", "DEMON", SCRIPT==BMR, 64);
    addRole(&count, "SHABALOTH", "EVIL", "DEMON", SCRIPT==BMR, 64);
    addRole(&count, "PO.", "EVIL", "DEMON", SCRIPT==BMR, 64);

    //Minions
    FIRST_MINION_INDEX = count;
    //TB
    addRole(&count, "BARON", "EVIL", "MINION", SCRIPT==TB, 64);
    addRole(&count, "SCARLET_WOMAN", "EVIL", "MINION", SCRIPT==TB, 64);
    addRole(&count, "SPY", "EVIL", "MINION", SCRIPT==TB, 64);
    addRole(&count, "POISONER", "EVIL", "MINION", SCRIPT==TB, 64);
    //S&V
    addRole(&count, "EVIL_TWIN", "EVIL", "MINION", SCRIPT==SV, 64);
    addRole(&count, "WITCH", "EVIL", "MINION", SCRIPT==SV, 64);
    addRole(&count, "CERENOVUS", "EVIL", "MINION", SCRIPT==SV, 64);
    addRole(&count, "PIT_HAG", "EVIL", "MINION", SCRIPT==SV, 64);
    //BMR
    addRole(&count, "GODFATHER", "EVIL", "MINION", SCRIPT==BMR, 64);
    addRole(&count, "DEVILS_ADVOCATE", "EVIL", "MINION", SCRIPT==BMR, 64);
    addRole(&count, "ASSASSIN", "EVIL", "MINION", SCRIPT==BMR, 64);
    addRole(&count, "MASTERMIND", "EVIL", "MINION", SCRIPT==BMR, 64);

    //Townsfolk
    //TB
    addRole(&count, "WASHERWOMAN", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "LIBRARIAN", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "INVESTIGATOR", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "CHEF", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "EMPATH", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "FORTUNE_TELLER", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "UNDERTAKER", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "MONK", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "RAVENKEEPER", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "VIRGIN", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "SLAYER", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "SOLDIER", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "MAYOR", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    //S&V
    addRole(&count, "CLOCKMAKER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "DREAMER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "SNAKE_CHARMER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "MATHEMATITICIAN", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "FLOWERGIRL", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "TOWN_CRIER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "ORACLE", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "SAVANT", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "SEAMSTRESS", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "PHILOSOPHER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "ARTIST", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "JUGGLER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "SAGE", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    //BMR
    addRole(&count, "GRANDMOTHER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "SAILOR", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "CHAMBERMAID", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "EXORCIST", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "INNKEEPER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "GAMBLER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "GOSSIP", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "COURTIER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "PROFESSOR", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "MINSTREL", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "TEA_LADY", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "PACIFIST", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "FOOL", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);

    //Outsiders
    FIRST_OUTSIDER_INDEX = count;
    //TB
    addRole(&count, "BUTLER", "GOOD", "OUTSIDER", SCRIPT==TB, 64);
    addRole(&count, "DRUNK", "GOOD", "OUTSIDER", SCRIPT==TB, 64);
    addRole(&count, "RECLUSE", "GOOD", "OUTSIDER", SCRIPT==TB, 64);
    addRole(&count, "SAINT", "GOOD", "OUTSIDER", SCRIPT==TB, 64);
    //S&V
    addRole(&count, "MUTANT", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    addRole(&count, "SWEETHEART", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    addRole(&count, "BARBER", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    addRole(&count, "KLUTZ", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    //BMR
    addRole(&count, "TINKER", "GOOD", "OUTSIDER", SCRIPT==BMR, 64);
    addRole(&count, "MOONCHILD", "GOOD", "OUTSIDER", SCRIPT==BMR, 64);
    addRole(&count, "GOON", "GOOD", "OUTSIDER", SCRIPT==BMR, 64);
    addRole(&count, "LUNATIC", "GOOD", "OUTSIDER", SCRIPT==BMR, 64);

    printf("INIT DATA STRUCTURES...\n");
    //Init data structures
    printf("-RULE SET...\n");
    *rs = initRS();
    printf("-DONE!..\n");
    printf("-KNOWLEDGE BASE...\n");
    *kb = initKB(NUM_PLAYERS, NUM_DAYS);
    printf("-DONE!..\n");

    printf("BUILD RULES...\n");
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (ROLE_IN_SCRIPT[i] == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[i], night);
                addKnowledgeName(*kb, "METADATA", 0, buff);
            }
        }
    }

    buildRules(*rs, *kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS, NUM_DAYS);
}

static void roleAssociation(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];

    // ===========================================
    //  Make role based Associatation rules
    // ===========================================
    /*
     * IDEA: Roles have associated data with them
     * For the most part!! a Player IS Demon => player IS Evil etc. etc.
    */
    for (int night = 0; night < numDays; night++)
    {
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            //A role implies a team
            //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE_TEAM>
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_TEAMS[i], night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            //A role implies a class
            //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE_CLASS>
            setTempRuleParams(rs, 1,0);
            
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_CLASSES[i], night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
        }

        //If a player is none of the roles within a class that player can't be that class (deduction)
        //<PLAYER>is_NOT_IMP => <PLAYER>is_NOT_DEMON
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "DEMON") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        pushTempRule(rs);

        //The converse is also true
        setTempRuleParams(rs, 1,0);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "DEMON") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //<PLAYER>is_NOT_BARON AND <PLAYER>is_NOT_SCARLET_WOMAN AND <PLAYER>is_NOT_SPY AND <PLAYER>is_NOT_POISONER=> <PLAYER>is_NOT_MINION
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "MINION") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        pushTempRule(rs);

        //The converse is also true
        setTempRuleParams(rs, 1,0);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "MINION") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //<PLAYER>is_NOT_BUTLER AND <PLAYER>is_NOT_DRUNK AND <PLAYER>is_NOT_RECLUSE AND <PLAYER>is_NOT_SAINT=> <PLAYER>is_NOT_OUTSIDER
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_OUTSIDER_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "OUTSIDER") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        pushTempRule(rs);

        //The converse is also true
        setTempRuleParams(rs, 1,0);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "OUTSIDER") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_OUTSIDER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);


        //<PLAYER>is_NOT_WASHERWOMAN AND <PLAYER>is_NOT_LIBRARIAN AND <PLAYER>is_NOT_INVESTIGATOR AND <PLAYER>is_NOT_CHEF
        //AND <PLAYER>is_NOT_EMPATH AND <PLAYER>is_NOT_FORTUNE_TELLER AND <PLAYER>is_NOT_UNDERTAKER AND <PLAYER>is_NOT_MONK
        //AND <PLAYER>is_NOT_RAVENKEEPER AND <PLAYER>is_NOT_VIRGIN AND <PLAYER>is_NOT_SLAYER AND <PLAYER>is_NOT_SOLDIER
        //AND <PLAYER>is_NOT_MAYOR => <PLAYER>is_NOT_TOWNSFOLK
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TOWNSFOLK_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "TOWNSFOLK") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        pushTempRule(rs);

        //The converse is also true
        setTempRuleParams(rs, 1,0);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_CLASSES[i], "TOWNSFOLK") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TOWNSFOLK_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);
    }
}
static void roleMutuallyExclusive(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Role Mutally Exclusive Rules
    // ===========================================
    /*
     * IDEA: Every player has exactly one role and only one player can have that role
     * For the most part
    */
    for (int night = 0; night < numDays; night++)
    {
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            //<PLAYER_A>is_<ROLE> => <PLAYER_B>is_NOT<ROLE>
            // IDEA: Only one player can have a role
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            //<PLAYER>is_NOT_{<ROLE_A>, <ROLE_B>...} => <PLAYER>is_<ROLE_Z>
            // IDEA: A player MUST have one role so by deduction if only one role is remaining they are that role
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff); //A player is role ROLE_NAME[i] if
            for (int j = 0; j < NUM_BOTCT_ROLES; j++)
            {
                if (j!=i)
                { //A player is NOT role ROLE_NAME[j] for all i!=j
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[j], night);
                    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            pushTempRule(rs);

            //<PLAYER>is_<ROLE_A> => <PLAYER>is_NOT<ROLE_B>
            // IDEA: Each player only has one role
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            for (int j = 0; j < NUM_BOTCT_ROLES; j++)
            {
                if (i!=j)
                { //I and j are mutually exclusive
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[j], night);
                    setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                }
            }
            pushTempRule(rs);
        }
    }
}
static void roleMetaData(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Role Metadata implications
    // ===========================================
    /*
     * IDEA: The game has metadata associated with it Roles can imply metadata and metadata can imply roles
     * 
    */
    for (int night = 0; night < numDays; night++)
    {
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            //<PLAYER_0>is_NOT_<ROLE> AND <PLAYER_1>is_NOT_<ROLE> AND .. => <METADATA>is_NOT_ROLE_in_PLAY
            // IDEA: If no one is a role, that role is not in play
            setTempRuleParams(rs, numPlayers,1);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[i], night);
            setTempRuleResultName(rs, kb, -1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            for (int j = 0; j < numPlayers; j++)
            {
                addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
            }
            pushTempRule(rs);

            
            //<PLAYER>is_<ROLE> => <METADATA>is_<ROLE>_in_PLAY
            // IDEA: If a player is a role that rule is in play (obviously)
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[i], night);
            setTempRuleResultName(rs, kb, -1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            //<METADATA>is_NOT_<ROLE>_in_PLAY => <PLAYER>is_NOT_<ROLE>
            // IDEA: [take the contrapositive] If a role is not in play... no one is that role
            setTempRuleParams(rs, 1,0);
            
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[i], night);
            addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            pushTempRule(rs);
        }
    }
}

static void roleMetaDataCounting(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Role Metadata Counting Arguments
    // ===========================================
    /*
     * IDEA: If we know about what roles are/are not in the game 
     * and we know about how many of a certain class we're expecting
     * we can deduce information about other roles that might be in the game
    */
    for (int night = 0; night < numDays; night++)
    {
        //If found all minions in play set all unused minions to is_NOT_<MINION>_in_PLAY
        int numUnusedMinions = TOTAL_MINIONS-numMinions;
        if (0 < numMinions && numMinions < TOTAL_MINIONS)
        {
            for (int i = 0; i < 4096; i++)
            {
                //Permute all subsets
                int count = 0;
                for (int j = 0; j < TOTAL_MINIONS; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        count++;
                    }
                }
                if (count == numMinions)
                {
                    //<METADATA>is_NOT_<MINION>_in_PLAY => <METADATA>is_<MINION>_in_PLAY
                    setTempRuleParams(rs, 1,0);
                    

                    for (int j = 0; j < TOTAL_MINIONS; j++)
                    {
                        if (((i >> j) & 1) == 1)
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_MINION_INDEX], night);
                            addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                        }
                        else
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_MINION_INDEX], night);
                            setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                        }
                    }
                    pushTempRule(rs);
                }
                if (count == numUnusedMinions)
                {
                    //<PLAYER>is_<MINION>_in_PLAY => <METADATA>is_NOT_<MINION>_in_PLAY
                    setTempRuleParams(rs, 1,0);
                    

                    for (int j = 0; j < TOTAL_MINIONS; j++)
                    {
                        if (((i >> j) & 1) == 1)
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_MINION_INDEX], night);
                            addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                        }
                        else
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_MINION_INDEX], night);
                            setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                        }
                    }
                    pushTempRule(rs);
                }
            }
        }
        
        for (int k = 0; k < 8; k++)
        {
            int isBaronInPlay = k&1;
            int isFangGuInPlay = (k>>1)&1;
            int isVigormortisInPlay = (k>>2)&1;
            int isGodFatherInPlay = 0;

            int numOutsiders = baseOutsiders;
            if (isBaronInPlay)
            {
                numOutsiders += 2;
            }
            if (isFangGuInPlay)
            {
                numOutsiders += 1;
            }
            if (isVigormortisInPlay)
            {
                numOutsiders -= 1;
            }
            int numUnusedOutsiders = TOTAL_OUTSIDERS-numOutsiders;

            for (int i = 0; i < 4096; i++)
            {
                //Permute all subsets
                int count = 0;
                for (int j = 0; j < TOTAL_OUTSIDERS; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        count++;
                    }
                }
                if (count == numOutsiders)
                {
                    //<METADATA>is_<OUTSIDER>_in_PLAY (xA) AND <METADATA>is_NOT_BARON_in_PLAY => <METADATA>is_NOT_<OUTSIDER>_in_PLAY (x4-A)
                    setTempRuleParams(rs, 1,0);
                    
                    //Check for outsider count modifiers
                    if (isBaronInPlay == 1)
                    {
                        //The BARON must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_BARON_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The BARON can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARON_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    if (isFangGuInPlay == 1)
                    {
                        //The FANG_GU must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_FANG_GU_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The FANG_GU can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FANG_GU_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    if (isVigormortisInPlay == 1)
                    {
                        //The VIGORMORTIS must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_VIGORMORTIS_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The VIGORMORTIS can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIGORMORTIS_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    if (isGodFatherInPlay == 1)
                    {
                        //The GODFATHER must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_GODFATHER_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The GODFATHER can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GODFATHER_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }

                    for (int j = 0; j < TOTAL_OUTSIDERS; j++)
                    {
                        if (((i >> j) & 1) == 1)
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_OUTSIDER_INDEX], night);
                            addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                        }
                        else
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_OUTSIDER_INDEX], night);
                            setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                        }
                    }
                    pushTempRule(rs);
                }
                if (count == numUnusedOutsiders)
                {
                    //<METADATA>is_NOT_<OUTSIDER>_in_PLAY (xA) AND <METADATA>is_NOT_BARON_in_PLAY => <METADATA>is_<OUTSIDER>_in_PLAY (x4-A)
                    setTempRuleParams(rs, 1,0);
                    
                    //Check for outsider count modifiers
                    if (isBaronInPlay == 1)
                    {
                        //The BARON must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_BARON_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The BARON can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARON_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    if (isFangGuInPlay == 1)
                    {
                        //The FANG_GU must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_FANG_GU_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The FANG_GU can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FANG_GU_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    if (isVigormortisInPlay == 1)
                    {
                        //The VIGORMORTIS must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_VIGORMORTIS_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The VIGORMORTIS can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIGORMORTIS_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    if (isGodFatherInPlay == 1)
                    {
                        //The GODFATHER must be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_GODFATHER_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        //The GODFATHER can NOT be in play for this
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GODFATHER_in_PLAY_[NIGHT%d]", night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }

                    for (int j = 0; j < TOTAL_OUTSIDERS; j++)
                    {
                        if (((i >> j) & 1) == 1)
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_OUTSIDER_INDEX], night);
                            addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                        }
                        else
                        {
                            snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[j+FIRST_OUTSIDER_INDEX], night);
                            setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                        }
                    }
                    pushTempRule(rs);
                }
            }

        }
    }
}
static void teamAndClassCountingArguments(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];

    int numEvil = numDemons + numMinions;
    int numGood = numPlayers - numEvil;
    // ===========================================
    //  Team Counting Arguments
    // ===========================================
    /*
     * IDEA: By counting the classes and teams of people,
     * we can deduce what teams other people must be
    */
    for (int night = 0; night < numDays; night++)
    {
        //RULE DISABLED DUE TO BAD PERFORMANCE
        //EDIT: new optimisation reduces this from O(n!) down to O(n) ish ish runing so re enabled

        //If all good players have been found the rest are evil
        //<PLAYER_1>is_GOOD AND ... <PLAYER_[num good players]>is_GOOD => <PLAYER_A>is_EVIL
        setTempRuleParams(rs, numGood,1);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff); 
        for (int j = 0; j < numGood; j++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
        }
        pushTempRule(rs);
        


        //If all evil players have been found the rest are good
        //<PLAYER_1>is_EVIL AND ... <PLAYER_[num evil players]>is_EVIL => <PLAYER_A>is_GOOD
        setTempRuleParams(rs, numEvil,1);
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff); 
        for (int j = 0; j < numEvil; j++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
        }
        pushTempRule(rs);

        //If all minion players have been found the rest are not minions
        //<PLAYER_1>is_MINION AND ... <PLAYER_[num minion players]>is_MINION => <PLAYER_A>is_NOT_MINION
        setTempRuleParams(rs, numMinions,1);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff); 
        for (int j = 0; j < numMinions; j++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
        }
        pushTempRule(rs);

        //If all demon players have been found the rest are not demons
        //<PLAYER_1>is_DEMON AND ... <PLAYER_[num minion players]>is_DEMON => <PLAYER_A>is_NOT_DEMON
        setTempRuleParams(rs, numDemons,1);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff); 
        for (int j = 0; j < numDemons; j++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
        }
        pushTempRule(rs);

        /*
        EDIT: DISABLED DUE TO MORE COMPLEX SCRIPTS IN GAME NOW

        //Outsider counting
        //If all outsider players have been found the rest are not outsider
        //<PLAYER_1>is_OUTSIDER AND ... <PLAYER_[num outsider players]>is_OUTSIDER => <PLAYER_A>is_NOT_OUTSIDER
        setTempRuleParams(rs, maxOutsiders,1);
        
        setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_NOT_OUTSIDER"); 
        for (int j = 0; j < maxOutsiders; j++)
        {
            addConditionToTempRuleName(rs,kb, j, "PLAYERS", "is_OUTSIDER");
        }
        pushTempRule(rs);

        //If all outsider players have been found the rest are not outsider
        //<PLAYER_1>is_OUTSIDER AND ... <PLAYER_[num outsider players]>is_OUTSIDER => <PLAYER_A>is_NOT_OUTSIDER
        setTempRuleParams(rs, baseOutsiders+1,1);
        
        setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_NOT_OUTSIDER"); 
        for (int j = 0; j < baseOutsiders; j++)
        {
            addConditionToTempRuleName(rs,kb, j, "PLAYERS", "is_OUTSIDER");
        }
        addConditionToTempRuleName(rs,kb, baseOutsiders, "METADATA", "is_NOT_BARON_in_PLAY");
        pushTempRule(rs);

        //Counting too many outsiders implies the existence of a baron
        //<PLAYER_1>is_OUTSIDER AND ... <PLAYER_[num outsider players]>is_OUTSIDER => <METADATA>is_BARON_in_PLAY
        setTempRuleParams(rs, baseOutsiders+1,1);
        
        setTempRuleResultName(rs, kb, -1, "METADATA", "is_BARON_in_PLAY"); 
        for (int j = 0; j < baseOutsiders+1; j++)
        {
            addConditionToTempRuleName(rs,kb, j, "PLAYERS", "is_OUTSIDER");
        }
        pushTempRule(rs);
        */
    }
}

static void classDeductionArguments(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Class Deduction argument rules
    // ===========================================
    /*
     * IDEA: If we know what class a player isn't,
     * then we know what class the player is
    */
    for (int night = 0; night < numDays; night++)
    {
        //A player can only have one class
        //<PLAYER>is_TOWNSFOLK => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_OUTSIDER
        //<PLAYER>is_OUTSIDER => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_TOWNSFOLK
        //<PLAYER>is_MINION => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK
        //<PLAYER>is_DEMON => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK
        for (int i = 0; i < 4; i++)
        {
            setTempRuleParams(rs, 1,0);
            if (i == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_TOWNSFOLK_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TOWNSFOLK_[NIGHT%d]", night);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);  
            }
            if (i == 1)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_OUTSIDER_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_OUTSIDER_[NIGHT%d]", night);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);  
            }
            if (i == 2)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", night);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);  
            }
            if (i == 3)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);  
            }
            pushTempRule(rs);
        }


        //A player must have a class
        //<PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_OUTSIDER => <PLAYER>is_TOWNSFOLK
        //<PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_TOWNSFOLK => <PLAYER>is_OUTSIDER
        //<PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK => <PLAYER>is_MINION
        //<PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK => <PLAYER>is_DEMON

        for (int i = 0; i < 4; i++)
        {
            setTempRuleParams(rs, 1,0);
            if (i == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_TOWNSFOLK_[NIGHT%d]", night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TOWNSFOLK_[NIGHT%d]", night);
                addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);  
            }
            if (i == 1)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_OUTSIDER_[NIGHT%d]", night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_OUTSIDER_[NIGHT%d]", night);
                addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);  
            }
            if (i == 2)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", night);
                addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);  
            }
            if (i == 3)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
                addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);  
            }
            pushTempRule(rs);
        }
    }
}

static void redHerringRules(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Red Herring rules
    // ===========================================
    /*
     * IDEA: There can be one red herring for the fortune teller
     * red herrings do not change with night
    */
    //<PLAYER>is_REDHERRING => <PLAYER_Y>is_NOT_REDHERRING
    setTempRuleParams(rs, 2,0);
    
    setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_NOT_REDHERRING");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_REDHERRING");
    pushTempRule(rs);

    //<PLAYER>is_REDHERRING => <PLAYER>is_GOOD
    setTempRuleParams(rs, 2,0);
    for (int night = 0; night < numDays; night++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
    }
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_REDHERRING");
    pushTempRule(rs);

    //Contrapositive logic
    //<PLAYER>is_EVIL => <PLAYER>is_NOT_REDHERRING
    setTempRuleParams(rs, 2,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_REDHERRING");
    for (int night = 0; night < numDays; night++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
    }
    pushTempRule(rs);
}

static void roleContinuityArguments(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Role Continuity Arguments
    // ===========================================
    /*
     * IDEA: For most games peoples roles don't change throughout the game
     * 
    */
    for (int startNight = 0; startNight < numDays; startNight++)
    {
        for (int nextNight = startNight+1; nextNight < numDays; nextNight++)
        {
            for (int i = 0; i < NUM_BOTCT_ROLES; i++)
            {
                if (strcmp(ROLE_NAMES[i], "IMP") == 0)
                { //Imps can star pass

                }
                else if (strcmp(ROLE_NAMES[i], "SCARLET_WOMAN") == 0)
                { //Scarlet womans can become the imp

                }
                else if (strcmp(ROLE_NAMES[i], "SNAKE_CHARMER") == 0)
                { //Snake charmers swap roles with demons

                }
                else if (strcmp(ROLE_CLASSES[i], "DEMON") == 0)
                {
                    //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE>[Night x]
                    setTempRuleParams(rs, 2,0);
                    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], nextNight);
                    setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], startNight);
                    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                    addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                    addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                    addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                    pushTempRule(rs);
                }
                else
                { //Everyone else is affected by barbers and pit hags
                    //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE>[Night x]
                    setTempRuleParams(rs, 2,0);
                    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], nextNight);
                    setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[i], startNight);
                    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                    addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                    addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                    pushTempRule(rs);
                }
                


            }
            //<PLAYER>is_GOOD => <PLAYER>is_GOOD[Night x]
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", nextNight);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            pushTempRule(rs);

            //<PLAYER>is_GOOD => <PLAYER>is_GOOD[Night x]
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", nextNight);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            pushTempRule(rs);

            //<PLAYER>is_GOOD => <PLAYER>is_GOOD[Night x]
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_TOWNSFOLK_[NIGHT%d]", nextNight);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_TOWNSFOLK_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            pushTempRule(rs);

            //<PLAYER>is_GOOD => <PLAYER>is_GOOD[Night x]
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_OUTSIDER_[NIGHT%d]", nextNight);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_OUTSIDER_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            pushTempRule(rs);

            //<PLAYER>is_GOOD => <PLAYER>is_GOOD[Night x]
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", nextNight);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            pushTempRule(rs);

            //<PLAYER>is_GOOD => <PLAYER>is_GOOD[Night x]
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", nextNight);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_IMP_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SCARLET_WOMAN_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            pushTempRule(rs);
        }
    }
}

static void poisonRules(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Poison rules
    // ===========================================
    /*
     * IDEA: There can be one red herring for the fortune teller
     * red herrings do not change with night
    */
    for (int night = 0; night < numDays; night++)
    {
        //<METADATA>is_NOT_POISONER_in_PLAY => <PLAYER>is_NOT_poisoned_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_COURTIER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINSTREL_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PUKKA_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VORTOX_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_NO_DASHII_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIGORMORTIS_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        pushTempRule(rs);
        /*
        //Contrapositive logic
        // <PLAYER>is_poisoned_NIGHT<i> => <METADATA>is_POISONER_in_PLAY
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_POISONER_in_PLAY_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);
        */
        /*
        //<PLAYER>is_EVIL => <PLAYER>is_NOT_poisoned_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_poisoned_NIGHT%d", i);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_EVIL");
        pushTempRule(rs);

        //Contrapositive logic
        //<PLAYER>is_poisoned_NIGHT<i> => <PLAYER>is_GOOD
        setTempRuleParams(rs, 1,0);
        
        setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_GOOD");
        snprintf(buff, STRING_BUFF_SIZE, "is_poisoned_NIGHT%d", i);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //Only one player can be poisoned at once
        //<PLAYER_A>is_poisoned_NIGHT<i> => <PLAYER_B>is_NOT_poisoned_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_poisoned_NIGHT%d", i);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_poisoned_NIGHT%d", i);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);
        */
    }

}

void buildRules(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders, int numDays)
{
    //ROLE RULES
    roleAssociation(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    roleMutuallyExclusive(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    //META DATA RULES
    roleMetaData(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    roleMetaDataCounting(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    //CLASS RULES
    teamAndClassCountingArguments(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    classDeductionArguments(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    
    //RED HERRING RULES
    redHerringRules(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    //POISON RULES
    poisonRules(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);

    //ROLE CONTINUITY ARGUMENTS
    roleContinuityArguments(rs, kb, numPlayers, numMinions, numDemons, baseOutsiders, numDays);
    
}

void buildRulesOLD(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders)
{

     
    // ===========================================
    //  Setup memory
    // ===========================================
    printf("-SETUP MEMORY...\n");
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    
    

    
    

    


    

    // ===========================================
    //  Dead rules
    // ===========================================
    //<PLAYER>died_by_HANGING AND <METADATA>is_NOT_SCARLET_WOMAN_in_PLAY=> <PLAYER>is_NOT_DEMON
    setTempRuleParams(rs, 2,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "died_by_HANGING");
    addConditionToTempRuleName(rs,kb, 1, "METADATA", "is_NOT_SCARLET_WOMAN_in_PLAY");
    pushTempRule(rs);

    //If a player died: the player is dead
    for (int i = 0; i < 10; i++)
    {
        //<METADATA>died_NIGHT<i> => <PLAYER>is_DEAD
        setTempRuleParams(rs, 1,0);
        
        setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_DEAD");
        snprintf(buff, STRING_BUFF_SIZE, "died_NIGHT%d", i);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

    }
    
    // ===========================================
    //  Poison rules
    // ===========================================
    for (int i = 0; i < 10; i++)
    {
        

    }
    
}