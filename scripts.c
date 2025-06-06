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

int NUM_ROLES_IN_SCRIPT;

int TOTAL_MINIONS = 0;
int TOTAL_OUTSIDERS = 0;
int MINION_INDICIES[NUM_BOTCT_ROLES];
int OUTSIDER_INDICIES[NUM_BOTCT_ROLES];

/**
 * initRoleStrings() - allocate memory to the strings
 * 
 * @maxLen max length of strings
*/
static void initRoleStrings(int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        ROLE_NAMES[i] = (char*) malloc(SIZE);
        ROLE_TEAMS[i] = (char*) malloc(SIZE);
        ROLE_CLASSES[i] = (char*) malloc(SIZE);
    }
}

/**
 * addRole() - Add a role name, team and class to the role descriptors
 * 
 * @index the index of the next empty space will be updated after calling this function
 * @name the name of the role
 * @team the team of the role
 * @class the class of the role
 * @roleInScript is the role in the script
 * @maxLen the maximum length a sctring can be
*/
static void addRole(int *index, char* name, char* team, char* class, int roleInScript, int maxLen)
{
    int SIZE = maxLen*sizeof(char);
    snprintf(ROLE_NAMES[*index], SIZE, "%s", name);
    snprintf(ROLE_TEAMS[*index], SIZE, "%s", team);
    snprintf(ROLE_CLASSES[*index], SIZE, "%s", class);
    ROLE_IN_SCRIPT[*index] = roleInScript;

    if (roleInScript == 1 && strcmp(class, "MINION") == 0) 
    {
        MINION_INDICIES[TOTAL_MINIONS] = *index;
        TOTAL_MINIONS++;
    }
    if (roleInScript == 1 && strcmp(class, "OUTSIDER") == 0) 
    {
        OUTSIDER_INDICIES[TOTAL_OUTSIDERS] = *index;
        TOTAL_OUTSIDERS++;
    }

    *index = *index + 1;
}

/**
 * initScript() - initialise a script for blood on the clocktower
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @SCRIPT the script to play
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
*/
void initScript(RuleSet** rs, KnowledgeBase** kb, const int SCRIPT, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];

    printf("NAME THINGS...\n");
    int count = 0;
    initRoleStrings(64);

    int TB = 0;
    int SV = 1;
    int BMR = 2;
    int ALFIE = 3; //alfies script :)

    
    //Roles
    //Demons
    //TB
    addRole(&count, "IMP", "EVIL", "DEMON", SCRIPT==TB || SCRIPT==ALFIE, 64);
    //S&V
    addRole(&count, "FANG_GU", "EVIL", "DEMON", SCRIPT==SV, 64);
    addRole(&count, "VIGORMORTIS", "EVIL", "DEMON", SCRIPT==SV, 64);
    addRole(&count, "NO_DASHII", "EVIL", "DEMON", SCRIPT==SV || SCRIPT==ALFIE, 64);
    addRole(&count, "VORTOX", "EVIL", "DEMON", SCRIPT==SV, 64);
    //BMR
    addRole(&count, "ZOMBUUL", "EVIL", "DEMON", SCRIPT==BMR, 64);
    addRole(&count, "PUKKA", "EVIL", "DEMON", SCRIPT==BMR, 64);
    addRole(&count, "SHABALOTH", "EVIL", "DEMON", SCRIPT==BMR, 64);
    addRole(&count, "PO.", "EVIL", "DEMON", SCRIPT==BMR, 64);
    //Misc
    addRole(&count, "OJO", "EVIL", "DEMON", SCRIPT==ALFIE, 64);
    addRole(&count, "LLEECH", "EVIL", "DEMON", SCRIPT==ALFIE, 64);

    //Minions
    //FIRST_MINION_INDEX = count;
    //TB
    addRole(&count, "BARON", "EVIL", "MINION", SCRIPT==TB || SCRIPT==ALFIE, 64);
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
    addRole(&count, "DEVILS_ADVOCATE", "EVIL", "MINION", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "ASSASSIN", "EVIL", "MINION", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "MASTERMIND", "EVIL", "MINION", SCRIPT==BMR, 64);
    //Misc
    addRole(&count, "SUMMONER", "EVIL", "MINION", SCRIPT==ALFIE, 64);

    //Townsfolk
    //TB
    addRole(&count, "WASHERWOMAN", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "LIBRARIAN", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "INVESTIGATOR", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "CHEF", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "EMPATH", "GOOD", "TOWNSFOLK", SCRIPT==TB, 64);
    addRole(&count, "FORTUNE_TELLER", "GOOD", "TOWNSFOLK", SCRIPT==TB || SCRIPT==ALFIE, 64);
    addRole(&count, "UNDERTAKER", "GOOD", "TOWNSFOLK", SCRIPT==TB || SCRIPT==ALFIE, 64);
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
    addRole(&count, "MATHEMATICIAN", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "FLOWERGIRL", "GOOD", "TOWNSFOLK", SCRIPT==SV || SCRIPT==ALFIE, 64);
    addRole(&count, "TOWN_CRIER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "ORACLE", "GOOD", "TOWNSFOLK", SCRIPT==SV || SCRIPT==ALFIE, 64);
    addRole(&count, "SAVANT", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "SEAMSTRESS", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "PHILOSOPHER", "GOOD", "TOWNSFOLK", SCRIPT==SV || SCRIPT==ALFIE, 64);
    addRole(&count, "ARTIST", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "JUGGLER", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    addRole(&count, "SAGE", "GOOD", "TOWNSFOLK", SCRIPT==SV, 64);
    //BMR
    addRole(&count, "GRANDMOTHER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "SAILOR", "GOOD", "TOWNSFOLK", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "CHAMBERMAID", "GOOD", "TOWNSFOLK", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "EXORCIST", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "INNKEEPER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "GAMBLER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "GOSSIP", "GOOD", "TOWNSFOLK", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "COURTIER", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "PROFESSOR", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "MINSTREL", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "TEA_LADY", "GOOD", "TOWNSFOLK", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "PACIFIST", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    addRole(&count, "FOOL", "GOOD", "TOWNSFOLK", SCRIPT==BMR, 64);
    //MISC
    addRole(&count, "BOUNTY_HUNTER", "GOOD", "TOWNSFOLK", SCRIPT==ALFIE, 64);
    addRole(&count, "ACROBAT", "GOOD", "TOWNSFOLK", SCRIPT==ALFIE, 64);
    addRole(&count, "POPPY_GROWER", "GOOD", "TOWNSFOLK", SCRIPT==ALFIE, 64);
    addRole(&count, "VILLAGE_IDIOT", "GOOD", "TOWNSFOLK", SCRIPT==ALFIE, 64);

    //Outsiders
    //FIRST_OUTSIDER_INDEX = count;
    //TB
    addRole(&count, "BUTLER", "GOOD", "OUTSIDER", SCRIPT==TB, 64);
    addRole(&count, "DRUNK", "GOOD", "OUTSIDER", SCRIPT==TB || SCRIPT==ALFIE, 64);
    addRole(&count, "RECLUSE", "GOOD", "OUTSIDER", SCRIPT==TB, 64);
    addRole(&count, "SAINT", "GOOD", "OUTSIDER", SCRIPT==TB, 64);
    //S&V
    addRole(&count, "MUTANT", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    addRole(&count, "SWEETHEART", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    addRole(&count, "BARBER", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    addRole(&count, "KLUTZ", "GOOD", "OUTSIDER", SCRIPT==SV, 64);
    //BMR
    addRole(&count, "TINKER", "GOOD", "OUTSIDER", SCRIPT==BMR, 64);
    addRole(&count, "MOONCHILD", "GOOD", "OUTSIDER", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "GOON", "GOOD", "OUTSIDER", SCRIPT==BMR || SCRIPT==ALFIE, 64);
    addRole(&count, "LUNATIC", "GOOD", "OUTSIDER", SCRIPT==BMR, 64);
    //MISC
    addRole(&count, "POLITICIAN", "GOOD", "OUTSIDER", SCRIPT==ALFIE, 64);
    

    printf("INIT DATA STRUCTURES...\n");
    //Init data structures
    printf("-RULE SET...\n");
    *rs = initRS();
    printf("-DONE!..\n");
    printf("-KNOWLEDGE BASE...\n");
    *kb = initKB(NUM_PLAYERS);
    printf("-DONE!..\n");

    printf("BUILD RULES...\n");
    NUM_ROLES_IN_SCRIPT = 0;
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (ROLE_IN_SCRIPT[i])
            {
                NUM_ROLES_IN_SCRIPT++;
            }
            else
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[i], night);
                addKnowledgeName(*kb, "METADATA", 0, buff);

                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                for (int player = 0; player < NUM_PLAYERS; player++) addKnowledgeName(*kb, "PLAYERS", player, buff);
            }
        }
    }
    //No ones role can have changed on the first night
    for (int player = 0; player < NUM_PLAYERS; player++)
    {
        addKnowledgeName(*kb, "PLAYERS", player, "is_NOT_ROLE_CHANGED_[NIGHT0]");
        addKnowledgeName(*kb, "PLAYERS", player, "NOT_SLEEP_DEATH_[NIGHT0]");
        addKnowledgeName(*kb, "PLAYERS", player, "NOT_HANGING_DEATH_[NIGHT0]");
        addKnowledgeName(*kb, "PLAYERS", player, "NOT_NOMINATION_DEATH_[NIGHT0]");
        addKnowledgeName(*kb, "PLAYERS", player, "NOT_RESURRECTED_[NIGHT0]");
        addKnowledgeName(*kb, "PLAYERS", player, "is_ALIVE_[NIGHT0]");
    }

    buildRules(*rs, *kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
}

/**
 * roleAssociation() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void roleAssociation(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];

    // ===========================================
    //  Make role based Associatation rules
    // ===========================================
    printf("Make role based Associatation rules...\n");
    /*
     * IDEA: Roles have associated data with them
     * For the most part!! a Player IS Demon => player IS Evil etc. etc.
    */
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            if (ROLE_IN_SCRIPT[role])
            {
                //A role implies a team (assuming the role was made the normal way)
                //<PLAYER>is_<ROLE> AND <PLAYER>is_NOT_ROLE_CHANGED => <PLAYER>is_<ROLE_TEAM>
                setTempRuleParams(rs, 2,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_TEAMS[role], night);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ROLE_CHANGED_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BOUNTY_HUNTER_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                pushTempRule(rs);

                //A role implies a class
                //Unlike alingment the class is always implied by a role
                //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE_CLASS>
                setTempRuleParams(rs, 1,0);
                
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_CLASSES[role], night);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                pushTempRule(rs);
            }
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

        //If the role hasn't been change you are definitely the correct alignment for your role
        //Assuming a bounty hunter isn't in play
        setTempRuleParams(rs, 2,0);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_TEAMS[i], "GOOD") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ROLE_CHANGED_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BOUNTY_HUNTER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
        pushTempRule(rs);

        setTempRuleParams(rs, 1,0);
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(ROLE_TEAMS[i], "EVIL") == 0)
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ROLE_CHANGED_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);
    }
}

/**
 * roleMutuallyExclusive() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void roleMutuallyExclusive(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Roles Are Mutally Exclusive And Players have exactly 1 Role
    // ===========================================
    printf("Make Roles Are Mutally Exclusive And Players have exactly 1 Role...\n");
    /*
     * IDEA: Every player has exactly one role and only one player can have that role
     * For the most part (excluding dead players)
    */
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            if (ROLE_IN_SCRIPT[role])
            {
                //<PLAYER_A>is_<ROLE> AND <PLAYER_A>is_ALIVE AND <PLAYER_B>is_ALIVE => <PLAYER_B>is_NOT<ROLE>
                // IDEA: Only one alive player can have a role
                if (strcmp(ROLE_NAMES[role], "VILLAGE_IDIOT") != 0)
                { //Village idiots are not mutually exclusive
                    setTempRuleParams(rs, 2,1);
                    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                    setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
                    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                    snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
                    addConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff);
                    pushTempRule(rs);
                }

                //<PLAYER>is_NOT_{<ROLE_A>, <ROLE_B>...} => <PLAYER>is_<ROLE_Z>
                // IDEA: A player MUST have one role so by deduction if only one role is remaining they are that role
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff); //A player is role ROLE_NAME[i] if
                for (int j = 0; j < NUM_BOTCT_ROLES; j++)
                {
                    if (role!=j)
                    { //A player is NOT role ROLE_NAME[j] for all i!=j
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[j], night);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                    }
                }
                pushTempRule(rs);

                //<PLAYER>is_<ROLE_A> => <PLAYER>is_NOT<ROLE_B>
                // IDEA: Each player only has one role
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                for (int j = 0; j < NUM_BOTCT_ROLES; j++)
                {
                    if (role!=j)
                    { //I and j are mutually exclusive
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[j], night);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                    }
                }
                pushTempRule(rs);
            }
        }
    }
}

/**
 * roleMetaData() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void roleMetaData(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Role Metadata implications
    // ===========================================
    printf("Make: Role Metadata implications...\n");
    /*
     * IDEA: The game has metadata associated with it Roles can imply metadata and metadata can imply roles
     * 
    */
    for (int night = 0; night < NUM_DAYS; night++)
    {
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            if (ROLE_IN_SCRIPT[role])
            {
                //<PLAYER_0>is_NOT_<ROLE> AND <PLAYER_1>is_NOT_<ROLE> AND .. => <METADATA>is_NOT_ROLE_in_PLAY
                // IDEA: If no one is a role, that role is not in play
                setTempRuleParams(rs, NUM_PLAYERS,1);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, -1, "METADATA", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                for (int j = 0; j < NUM_PLAYERS; j++)
                {
                    addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
                }
                pushTempRule(rs);

                
                //<PLAYER>is_<ROLE> => <METADATA>is_<ROLE>_in_PLAY
                // IDEA: If a player is a role that role is in play (obviously)
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, -1, "METADATA", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                pushTempRule(rs);

                //<PLAYER>is_<ROLE> AND <PLAYER>is_ALIVE => <METADATA>is_<ROLE>_ALIVE
                // IDEA: If a player is a role and is alive that role is in play and alive (obviously)
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_ALIVE_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, -1, "METADATA", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                pushTempRule(rs);

                //<PLAYER>is_<ROLE> => <METADATA>is_<ROLE>_in_PLAY
                // IDEA: If a player is a role and is alive that role is in play and alive (obviously)
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, -1, "METADATA", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                pushTempRule(rs);

                //<METADATA>is_NOT_<ROLE>_in_PLAY => <PLAYER>is_NOT_<ROLE>
                // IDEA: [take the contrapositive] If a role is not in play... no one is that role
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                pushTempRule(rs);

                //<METADATA>is_NOT_<ROLE>_in_PLAY => <METADATA>is_NOT_<ROLE>_ALIVE
                // If A role is NOT in play no one can be that alive role
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_ALIVE_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                pushTempRule(rs);

                //<METADATA>is_<ROLE>_ALIVE => <METADATA>is_<ROLE>_in_PLAY
                // Contrapositive of rule above
                setTempRuleParams(rs, 1,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[role], night);
                setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                snprintf(buff, STRING_BUFF_SIZE, "is_%s_ALIVE_[NIGHT%d]", ROLE_NAMES[role], night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                pushTempRule(rs);
            }
        }
    }
}

static int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

/*
NOTE: I think this function my need to be updated
*/
/**
 * roleMetaDataCounting() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void roleMetaDataCounting(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Role Metadata Counting Arguments
    // ===========================================
    printf("Make: Role Metadata Counting Arguments...\n");
    /*
     * IDEA: If we know about what roles are/are not in the game 
     * and we know about how many of a certain class we're expecting
     * we can deduce information about other roles that might be in the game
     * 
     * These rules ONLY works on night 0 as roles can change which changes the distrubution of roles
    */
    int night = 0; //Set night to 0
    //If found all minions in play set all unused minions to is_NOT_<MINION>_in_PLAY
    int numUnusedMinions = TOTAL_MINIONS-NUM_MINIONS;

    int NUM_MINION_PERMS = ipow(2, TOTAL_MINIONS);
    if (0 < NUM_MINIONS && NUM_MINIONS < TOTAL_MINIONS)
    {
        for (int i = 0; i < NUM_MINION_PERMS; i++)
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
            if (count == NUM_MINIONS)
            {
                //<METADATA>is_NOT_<MINION>_in_PLAY => <METADATA>is_<MINION>_in_PLAY
                setTempRuleParams(rs, 1,0);
                

                for (int j = 0; j < TOTAL_MINIONS; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[MINION_INDICIES[j]], night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[MINION_INDICIES[j]], night);
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
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[MINION_INDICIES[j]], night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[MINION_INDICIES[j]], night);
                        setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                    }
                }
                pushTempRule(rs);
            }
        }
    }
    
    int NUM_OUTSIDER_PERMS = ipow(2, TOTAL_OUTSIDERS);
    int NUM_MOD_PERMS = ipow(2, 3);
    for (int k = 0; k < NUM_MOD_PERMS; k++)
    {
        int isBaronInPlay = k&1;
        int isFangGuInPlay = (k>>1)&1;
        int isVigormortisInPlay = (k>>2)&1;
        int isGodFatherInPlay = 0;

        int numOutsiders = BASE_OUTSIDERS;
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

        for (int i = 0; i < NUM_OUTSIDER_PERMS; i++)
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
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[OUTSIDER_INDICIES[j]], night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[OUTSIDER_INDICIES[j]], night);
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
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[OUTSIDER_INDICIES[j]], night);
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
                    }
                    else
                    {
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[OUTSIDER_INDICIES[j]], night);
                        setTempRuleResultName(rs, kb, 0, "METADATA", buff);
                    }
                }
                pushTempRule(rs);
            }
        }

    }
    
}

/*
NOTE: I think this function my need to be updated
*/
/**
 * teamAndClassCountingArguments() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void teamAndClassCountingArguments(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];

    int numEvil = NUM_DEMONS + NUM_MINIONS;
    int numGood = NUM_PLAYERS - numEvil;
    // ===========================================
    //  Team Counting Arguments
    // ===========================================
    printf("Make: Team Counting Arguments...\n");
    /*
     * IDEA: By counting the classes and teams of people,
     * we can deduce what teams other people must be
     * 
     * theses rules are ONLY true on night 0 when roles are assigned
     * this is becaue roles can change on later nights
    */
    int night = 0;

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
    setTempRuleParams(rs, NUM_MINIONS,1);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, -1, "PLAYERS", buff); 
    for (int j = 0; j < NUM_MINIONS; j++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
    }
    pushTempRule(rs);

    //If all demon players have been found the rest are not demons
    //<PLAYER_1>is_DEMON AND ... <PLAYER_[num minion players]>is_DEMON => <PLAYER_A>is_NOT_DEMON
    setTempRuleParams(rs, NUM_DEMONS,1);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, -1, "PLAYERS", buff); 
    for (int j = 0; j < NUM_DEMONS; j++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, j, "PLAYERS", buff);
    }
    pushTempRule(rs);
    
}

/**
 * classDeductionArguments() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void classDeductionArguments(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Class Deduction argument rules
    // ===========================================
    printf("Make: Class Deduction argument rules...\n");
    /*
     * IDEA: If we know what class a player isn't,
     * then we know what class the player is
    */
    for (int night = 0; night < NUM_DAYS; night++)
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

/**
 * redHerringRules() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void redHerringRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Red Herring rules
    // ===========================================
    printf("Make: Red Herring rules...\n");
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
    for (int night = 0; night < NUM_DAYS; night++)
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
    for (int night = 0; night < NUM_DAYS; night++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
    }
    pushTempRule(rs);
}

/**
 * roleContinuityArguments() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void roleContinuityArguments(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Role Continuity Arguments
    // ===========================================
    printf("Make: Role Continuity Arguments...\n");
    /*
     * IDEA: For most games peoples roles don't change throughout the game
     * 
    */
    for (int startNight = 0; startNight < NUM_DAYS; startNight++)
    {
        int nextNight = startNight+1;
        if (nextNight < NUM_DAYS)
        {
            for (int role = 0; role < NUM_BOTCT_ROLES; role++)
            {
                if (ROLE_IN_SCRIPT[role])
                {
                    if (strcmp(ROLE_NAMES[role], "IMP") == 0)
                    { 
                        //Imps can star pass, any minion can become the imp, 
                        //imps will remain imps unless affected snake charmers barbers or pithags

                        //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE>[Night x]
                        setTempRuleParams(rs, 2,0);
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        pushTempRule(rs);
                        
                        //<PLAYER>is_NOT_<ROLE> => <PLAYER>is_NOT_<ROLE>[Night x]
                        setTempRuleParams(rs, 2,0);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SUMMONER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        pushTempRule(rs);
                        
                        //<PLAYER>is_IMP AND <PLAYER>_NOT_KILLED_<PLAYER> AND <PLAYER2>=> <PLAYER2>is_NOT_IMP[Night x]
                        for (int playerID = 0; playerID < NUM_PLAYERS; playerID++)
                        {
                            setTempRuleParams(rs, 3,1);
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                            setTempRuleResultName(rs, kb, 2, "PLAYERS", buff);
                            snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerID);
                            snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", playerID, startNight);
                            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerID);
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SCARLET_WOMAN_[NIGHT%d]", startNight);
                            addConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff);
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SUMMONER_in_PLAY_[NIGHT%d]", startNight);
                            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                            pushTempRule(rs);
                        }
                        
                        
                    }
                    else if (strcmp(ROLE_NAMES[role], "SNAKE_CHARMER") == 0)
                    { //Snake charmers swap roles with demons

                    }
                    else if (strcmp(ROLE_CLASSES[role], "MINION") == 0)
                    { 
                        //Scarlet womans can become the demon, 
                        //Minions can become the imp

                        //No one will become a minion unless pithags, snake charmers or barbers are involved


                        //<PLAYER>is_NOT_<ROLE> => <PLAYER>is_NOT_<ROLE>[Night x]
                        setTempRuleParams(rs, 2,0);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        pushTempRule(rs);
                        
                    }
                    else if (strcmp(ROLE_CLASSES[role], "DEMON") == 0)
                    { //Non imp demons
                        //Scarlet womans can become the demon, 
                        //Minions can become the imp (not needed to consider here as imps considered seperately)

                        //Demons remain demons unless affected by snake charmers, barbers or pithags

                        //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE>[Night x]
                        setTempRuleParams(rs, 2,0);
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        pushTempRule(rs);
                        
                        //<PLAYER>is_NOT_<ROLE> => <PLAYER>is_NOT_<ROLE>[Night x]
                        setTempRuleParams(rs, 2,0);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SCARLET_WOMAN_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SUMMONER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        pushTempRule(rs);
                        
                    }
                    else
                    { //Everyone else is affected by barbers and pit hags
                        //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE>[Night x]
                        setTempRuleParams(rs, 2,0);
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        pushTempRule(rs);
                        
                        //<PLAYER>is_NOT_<ROLE> => <PLAYER>is_NOT_<ROLE>[Night x]
                        setTempRuleParams(rs, 2,0);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], nextNight);
                        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], startNight);
                        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
                        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
                        pushTempRule(rs);
                        
                    }
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

            //<PLAYER>is_EVIL_ => <PLAYER>is_EVIL_[Night x]
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

            //<PLAYER>is_TOWNSFOLK_ => <PLAYER>is_TOWNSFOLK_[Night x]
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

            //<PLAYER>is_OUTSIDER_ => <PLAYER>is_OUTSIDER_[Night x]
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

            //<PLAYER>is_DEMON_ => <PLAYER>is_DEMON_[Night x]
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

            //<PLAYER>is_MINION_ => <PLAYER>is_MINION_[Night x]
            //NOTE this is not true if the demon star passes
            
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", nextNight);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_MINION_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SCARLET_WOMAN_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_IMP_in_PLAY_[NIGHT%d]", startNight);
            addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
            
            pushTempRule(rs);
            
        }
    }
}

/**
 * poisonRules() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void poisonRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Poison rules
    // ===========================================
    printf("Make: Poison rules...\n");
    /*
     * IDEA: There can be one red herring for the fortune teller
     * red herrings do not change with night
     * 
     * ROLES THAT CAUSE POISONING
     * ===TB
     * Poisoner (Anyone)
     * 
     * ===BMR
     * Sailor (Either someone or themselves)
     * Innkeeper
     * COURTIER
     * MINSTREL
     * GOON
     * PUKKA
     * 
     * ===S&V
     * SNAKE_CHARMER
     * PHILOSOPHER
     * SWEETHEART
     * VORTOX [WRONG INFO so not really poisoned but assumed wrong]
     * NO_DASHII
     * VIGORMORTIS
     * 
    */
    for (int night = 0; night < NUM_DAYS; night++)
    {
        //<METADATA>is_NOT_POISONER_in_PLAY AND ...=> <PLAYER>is_NOT_poisoned_NIGHT<i>
        //IDEA: If no roles that cause poisoning are in play... no one can be poisoned
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
        //TB
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        //BMR
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAILOR_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_INNKEEPER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_COURTIER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINSTREL_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PUKKA_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        //S&V
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PHILOSOPHER_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SWEETHEART_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VORTOX_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_NO_DASHII_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIGORMORTIS_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        //Misc
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LLEECH_in_PLAY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        pushTempRule(rs);

        //Specific poisoning
        //An Alive VORTOX poisons everyone
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_VORTOX_ALIVE_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        pushTempRule(rs);
        //An Alive NO_DASHII Poisons to the left and right of himself
        for (int player = 0; player < NUM_PLAYERS; player++)
        {
            int LHS = player - 1;
            int RHS = player + 1;
            if (LHS < 0)
            {
                LHS += NUM_PLAYERS;
            }
            if (RHS >= NUM_PLAYERS)
            {
                RHS -= NUM_PLAYERS;
            }
            //LHS
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, -LHS-1000, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NO_DASHII_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, player);
            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, player);
            pushTempRule(rs);
            //RHS
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, -RHS-1000, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NO_DASHII_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, player);
            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, player);
            pushTempRule(rs);
        }


        //Specific poisoning
        //A solider who dies [[by a demon]] must be poisoned
        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_SOLDIER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "SLEEP_DEATH_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ASSASSIN_ALIVE_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 1, "METADATA", buff);
        pushTempRule(rs);
        
        for (int poisonRole = 0; poisonRole < 14; poisonRole++)
        {
            //Contrapositive logic
            //Disabled due to a large amount of roles in play
            // <PLAYER>is_poisoned_NIGHT<i> => <METADATA>is_POISONER_in_PLAY
            setTempRuleParams(rs, 2,0);

            //TB
            if (poisonRole == 0) snprintf(buff, STRING_BUFF_SIZE, "is_POISONER_ALIVE_[NIGHT%d]", night);
            //BMR
            else if (poisonRole == 1) snprintf(buff, STRING_BUFF_SIZE, "is_SAILOR_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 2) snprintf(buff, STRING_BUFF_SIZE, "is_INNKEEPER_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 3) snprintf(buff, STRING_BUFF_SIZE, "is_COURTIER_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 4) snprintf(buff, STRING_BUFF_SIZE, "is_MINSTREL_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 5) snprintf(buff, STRING_BUFF_SIZE, "is_GOON_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 6) snprintf(buff, STRING_BUFF_SIZE, "is_PUKKA_ALIVE_[NIGHT%d]", night);
            //S&V
            else if (poisonRole == 7) snprintf(buff, STRING_BUFF_SIZE, "is_SNAKE_CHARMER_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 8) snprintf(buff, STRING_BUFF_SIZE, "is_PHILOSOPHER_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 9) snprintf(buff, STRING_BUFF_SIZE, "is_SWEETHEART_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 10) snprintf(buff, STRING_BUFF_SIZE, "is_VORTOX_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 11) snprintf(buff, STRING_BUFF_SIZE, "is_NO_DASHII_ALIVE_[NIGHT%d]", night);
            else if (poisonRole == 12) snprintf(buff, STRING_BUFF_SIZE, "is_VIGORMORTIS_ALIVE_[NIGHT%d]", night);
            //Misc
            else if (poisonRole == 13) snprintf(buff, STRING_BUFF_SIZE, "is_LLEECH_ALIVE_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "METADATA", buff);

            snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff);

            //TB
            if (poisonRole != 0) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONER_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            //BMR
            if (poisonRole != 1) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAILOR_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 2) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_INNKEEPER_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 3) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_COURTIER_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 4) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINSTREL_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 5) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 6) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PUKKA_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            //S&V
            if (poisonRole != 7) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 8) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PHILOSOPHER_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 9) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SWEETHEART_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 10) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VORTOX_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 11) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_NO_DASHII_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 12) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIGORMORTIS_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }
            if (poisonRole != 13) 
            {
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LLEECH_in_PLAY_[NIGHT%d]", night);
                addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
            }

            pushTempRule(rs);
        }
        /*
        //This is not true in scripts other than tb and technically 
        //the poisoner could poison they're own team
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
        //Also not true
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

/**
 * summonerRules() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void summonerRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Summoner rules
    // ===========================================
    printf("Make: Summoner rules...\n");
    /*
     * IDEA: 
     * 
    */
   setTempRuleParams(rs, 1,0);
   setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_NOT_DEMON_[NIGHT0]");
   addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_SUMMONER_in_PLAY_[NIGHT0]");
   pushTempRule(rs);

}

/**
 * deathRules() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void deathRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Death rules
    // ===========================================
    printf("Make: Death rules...\n");
    /*
     * IDEA: 
     * 
    */

    //If a player died: the player is dead
    for (int night = 0; night < NUM_DAYS; night++)
    {
        //<METADATA>SLEEP_DEATH_NIGHT<i> => <PLAYER>is_DEAD_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "SLEEP_DEATH_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //<METADATA>HANGING_DEATH<i> => <PLAYER>is_DEAD_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "HANGING_DEATH_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //<METADATA>NOMINATION_DEATH<i> => <PLAYER>is_DEAD_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "NOMINATION_DEATH_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //Contrapositive
        //<METADATA>is_ALIVE_NIGHT<i> => <PLAYER>NOT_SLEEP_DEATH_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //<METADATA>is_ALIVE_NIGHT<i> => <PLAYER>NOT_HANGING_DEATH_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "NOT_HANGING_DEATH_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //<METADATA>is_ALIVE_NIGHT<i> => <PLAYER>NOT_NOMINATION_DEATH_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "NOT_NOMINATION_DEATH_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);
        int previousNight = night-1;
        if (previousNight >= 0)
        {
            //Case Analysis for remaining alive
            //<PLAYER>NOT_SLEEP_DEATH_NIGHT<i> AND <PLAYER>NOT_HANGING_DEATH_NIGHT<i> AND <PLAYER>NOT_NOMINATION_DEATH_NIGHT<i> AND <PLAYER>is_ALIVE_NIGHT<"j<i">=> <METADATA>is_ALIVE_NIGHT
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_NOMINATION_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_HANGING_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", previousNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            //Case Analysis for remaining dead
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", previousNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_RESURRECTED_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            //Case Analysis for coming back to life
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "RESURRECTED_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_NOMINATION_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_HANGING_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", previousNight);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            //Demon Dying -> Scarlett Woman becomes Demon (If atleast 5 players are alive)
            setTempRuleParams(rs, 5,1);
            snprintf(buff, STRING_BUFF_SIZE, "is_IMP_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 1, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_ROLE_CHANGED_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 1, "PLAYERS", buff);
            //EVIL Demon died
            snprintf(buff, STRING_BUFF_SIZE, "HANGING_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            //EVIL Scarlet Woman Is Alive
            snprintf(buff, STRING_BUFF_SIZE, "is_SCARLET_WOMAN_[NIGHT%d]", previousNight);
            addConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff);

            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff);

            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 3, "PLAYERS", buff);

            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            addConditionToTempRuleName(rs,kb, 4, "PLAYERS", buff);
            pushTempRule(rs);

            //Need to add star passing rules
        }
        else
        {
            //Case Analysis for remaining alive
            //<PLAYER>NOT_SLEEP_DEATH_NIGHT<i> AND <PLAYER>NOT_HANGING_DEATH_NIGHT<i> AND <PLAYER>NOT_NOMINATION_DEATH_NIGHT<i> AND <PLAYER>is_ALIVE_NIGHT<"j<i">=> <METADATA>is_ALIVE_NIGHT
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_ALIVE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_NOMINATION_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_HANGING_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
        }
    }
}
/**
 * poisoningRules() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void poisoningRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Poisoning rules
    // ===========================================
    printf("Make: Poisoning rules...\n");
    /*
     * IDEA: 
     * 
    */

    //If a player died: the player is dead
    for (int night = 0; night < NUM_DAYS; night++)
    {

        setTempRuleParams(rs, 1,0);
        for (int notPoisonedPlayerID = 0; notPoisonedPlayerID < NUM_PLAYERS; notPoisonedPlayerID++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "NOT_POISONED_%d_[NIGHT%d]", notPoisonedPlayerID, night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
        }

        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAILOR_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_INNKEEPER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_COURTIER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINSTREL_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LUNATIC_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PUKKA_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);

        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PHILOSOPHER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SWEETHEART_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_NO_DASHII_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VORTOX_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //Rules to enforce maximum killings per role
        for (int killedPlayerID = 0; killedPlayerID < NUM_PLAYERS; killedPlayerID++)
        {
            /*
            IMP
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_POISONED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "POISONED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_POISONER_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            
            /*
            NOTE: need to add more complex demons to this list
            */

        }
    
        for (int playerID = 0; playerID < NUM_PLAYERS; playerID++)
        {
            
            // <PLAYER>POISONED_<PLAYER_X>_NIGHT<i> => <PLAYER_X>is_POISONED__NIGHT<i> 
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, -1000-playerID, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "POISONED_%d_[NIGHT%d]", playerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            // <PLAYER_X>NOT_SLEEP_DEATH_NIGHT<i> => <PLAYER>NOT_KILLED_<PLAYER_X>_NIGHT_<i>
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_POISONED_%d_[NIGHT%d]", playerID, night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs, kb, 1, "PLAYERS", buff, playerID);
            pushTempRule(rs);

            // <PLAYER>POISONED_<PLAYER_X>_NIGHT<i> => <PLAYER>NOT_<ROLE THAT DOESN'T POISON>
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_IMP_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FANG_GU_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIGORMORTIS_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_NO_DASHII_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ZOMBUUL_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_OJO_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LLEECH_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ASSASSIN_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SLAYER_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARON_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SCARLET_WOMAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_EVIL_TWIN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_WITCH_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CERENOVUS_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEVILS_ADVOCATE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MASTERMIND_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_WASHERWOMAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LIBRARIAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_INVESTIGATOR_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CHEF_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_EMPATH_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FORTUNE_TELLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_UNDERTAKER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MONK_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RAVENKEEPER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIRGIN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SOLDIER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MAYOR_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CLOCKMAKER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DREAMER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MATHEMATICIAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FLOWERGIRL_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TOWN_CRIER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ORACLE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAVANT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SEAMSTRESS_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ARTIST_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_JUGGLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAGE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GRANDMOTHER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CHAMBERMAID_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_EXORCIST_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GAMBLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOSSIP_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PROFESSOR_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TEA_LADY_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PACIFIST_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FOOL_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BOUNTY_HUNTER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ACROBAT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POPPY_GROWER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VILLAGE_IDIOT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BUTLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DRUNK_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAINT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MUTANT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SWEETHEART_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_KLUTZ_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TINKER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MOONCHILD_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LUNATIC_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POLITICIAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);

            snprintf(buff, STRING_BUFF_SIZE, "POISONED_%d_[NIGHT%d]", playerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
        }
    }
}
/**
 * killingRules() - add rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
static void killingRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    // ===========================================
    //  Death rules
    // ===========================================
    printf("Make: Killing rules...\n");
    /*
     * IDEA: 
     * 
    */

    //If a player died: the player is dead
    for (int night = 0; night < NUM_DAYS; night++)
    {

        setTempRuleParams(rs, 1,0);
        for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
        }

        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_IMP_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FANG_GU_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIGORMORTIS_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_NO_DASHII_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VORTOX_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ZOMBUUL_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PUKKA_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SHABALOTH_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PO._[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_OJO_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LLEECH_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ASSASSIN_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SLAYER_[NIGHT%d]", night);
        addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //Rules to enforce maximum killings per role
        for (int killedPlayerID = 0; killedPlayerID < NUM_PLAYERS; killedPlayerID++)
        {
            /*
            IMP
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_IMP_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            /*
            ASSASSIN
            NOTE: this actually can only be triggered once per game
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_ASSASSIN_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            /*
            FANG_GU
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_FANG_GU_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            /*
            VIGORMORTIS
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_VIGORMORTIS_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            /*
            NO_DASHII
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NO_DASHII_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            /*
            VORTOX
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_VORTOX_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            /*
            ZOMBUUL
            */
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> AND <PLAYER>is_IMP_NIGHT<i> =>  <PLAYER>NOT_KILLED_<PLAYER_Y>_NIGHT<i>
            setTempRuleParams(rs, 1,0);
            for (int notKilledPlayerID = 0; notKilledPlayerID < NUM_PLAYERS; notKilledPlayerID++)
            {
                if (notKilledPlayerID != killedPlayerID)
                {
                    snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", notKilledPlayerID, night);
                    setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
                }
            }
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", killedPlayerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_ZOMBUUL_[NIGHT%d]", night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
            /*
            NOTE: need to add more complex demons to this list
            */

        }
    
        for (int playerID = 0; playerID < NUM_PLAYERS; playerID++)
        {
            
            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> => <PLAYER_X>SLEEP_DEATH_NIGHT<i> 
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "SLEEP_DEATH_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, -1000-playerID, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", playerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);

            // <PLAYER_X>NOT_SLEEP_DEATH_NIGHT<i> => <PLAYER>NOT_KILLED_<PLAYER_X>_NIGHT_<i>
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_KILLED_%d_[NIGHT%d]", playerID, night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs, kb, 1, "PLAYERS", buff, playerID);
            pushTempRule(rs);

            // <PLAYER>KILLED_<PLAYER_X>_NIGHT<i> => <PLAYER>NOT_<ROLE THAT DOESN'T KILL>
            setTempRuleParams(rs, 1,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARON_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SCARLET_WOMAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_EVIL_TWIN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_WITCH_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CERENOVUS_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PIT_HAG_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEVILS_ADVOCATE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MASTERMIND_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_WASHERWOMAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LIBRARIAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_INVESTIGATOR_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CHEF_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_EMPATH_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FORTUNE_TELLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_UNDERTAKER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MONK_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RAVENKEEPER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VIRGIN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SOLDIER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MAYOR_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CLOCKMAKER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DREAMER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SNAKE_CHARMER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MATHEMATICIAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FLOWERGIRL_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TOWN_CRIER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ORACLE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAVANT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SEAMSTRESS_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PHILOSOPHER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ARTIST_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_JUGGLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAGE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GRANDMOTHER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAILOR_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_CHAMBERMAID_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_EXORCIST_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_INNKEEPER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GAMBLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOSSIP_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_COURTIER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PROFESSOR_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINSTREL_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TEA_LADY_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_PACIFIST_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_FOOL_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BOUNTY_HUNTER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_ACROBAT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POPPY_GROWER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_VILLAGE_IDIOT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BUTLER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DRUNK_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SAINT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MUTANT_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SWEETHEART_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_BARBER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_KLUTZ_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_TINKER_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MOONCHILD_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_GOON_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_LUNATIC_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POLITICIAN_[NIGHT%d]", night);
            setTempRuleResultName(rs,kb, 0, "PLAYERS", buff);

            snprintf(buff, STRING_BUFF_SIZE, "KILLED_%d_[NIGHT%d]", playerID, night);
            addConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff);
            pushTempRule(rs);
        }
    }
}
/**
 * buildRules() - add all the rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
void buildRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS)
{
    //ROLE RULES
    roleAssociation(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    roleMutuallyExclusive(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    //META DATA RULES
    roleMetaData(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    roleMetaDataCounting(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    //CLASS RULES
    teamAndClassCountingArguments(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    classDeductionArguments(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    
    //RED HERRING RULES
    redHerringRules(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    //POISON RULES
    //poisonRules(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    //ROLE CONTINUITY ARGUMENTS
    roleContinuityArguments(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    //SUMMONER RULES
    summonerRules(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    //DEATH RULES
    deathRules(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    //POISONING RULES
    poisoningRules(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    //KILLING RULES
    killingRules(rs, kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
    
}