#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tb.h"
#include "constants.h"
#include "rules.h"

char *ROLE_NAMES[NUM_BOTCT_ROLES];
char *ROLE_TEAMS[NUM_BOTCT_ROLES];
char *ROLE_CLASSES[NUM_BOTCT_ROLES];

void initTB(RuleSet** rs, KnowledgeBase** kb, int NUM_PLAYERS, int NUM_MINIONS, int NUM_DEMONS, int BASE_OUTSIDERS, int NUM_DAYS)
{
    printf("NAME THINGS...\n");
    //Roles
    //Demons
    ROLE_NAMES[0] = "IMP";
    ROLE_TEAMS[0] = "EVIL";
    ROLE_CLASSES[0] = "DEMON";

    //Minions
    ROLE_NAMES[1] = "BARON";
    ROLE_TEAMS[1] = "EVIL";
    ROLE_CLASSES[1] = "MINION";

    ROLE_NAMES[2] = "SCARLET_WOMAN";
    ROLE_TEAMS[2] = "EVIL";
    ROLE_CLASSES[2] = "MINION";

    ROLE_NAMES[3] = "SPY";
    ROLE_TEAMS[3] = "EVIL";
    ROLE_CLASSES[3] = "MINION";

    ROLE_NAMES[4] = "POISONER";
    ROLE_TEAMS[4] = "EVIL";
    ROLE_CLASSES[4] = "MINION";

    //Townsfolk
    ROLE_NAMES[5] = "WASHERWOMAN";
    ROLE_TEAMS[5] = "GOOD";
    ROLE_CLASSES[5] = "TOWNSFOLK";

    ROLE_NAMES[6] = "LIBRARIAN";
    ROLE_TEAMS[6] = "GOOD";
    ROLE_CLASSES[6] = "TOWNSFOLK";

    ROLE_NAMES[7] = "INVESTIGATOR";
    ROLE_TEAMS[7] = "GOOD";
    ROLE_CLASSES[7] = "TOWNSFOLK";

    ROLE_NAMES[8] = "CHEF";
    ROLE_TEAMS[8] = "GOOD";
    ROLE_CLASSES[8] = "TOWNSFOLK";

    ROLE_NAMES[9] = "EMPATH";
    ROLE_TEAMS[9] = "GOOD";
    ROLE_CLASSES[9] = "TOWNSFOLK";

    ROLE_NAMES[10] = "FORTUNE_TELLER";
    ROLE_TEAMS[10] = "GOOD";
    ROLE_CLASSES[10] = "TOWNSFOLK";

    ROLE_NAMES[11] = "UNDERTAKER";
    ROLE_TEAMS[11] = "GOOD";
    ROLE_CLASSES[11] = "TOWNSFOLK";

    ROLE_NAMES[12] = "MONK";
    ROLE_TEAMS[12] = "GOOD";
    ROLE_CLASSES[12] = "TOWNSFOLK";

    ROLE_NAMES[13] = "RAVENKEEPER";
    ROLE_TEAMS[13] = "GOOD";
    ROLE_CLASSES[13] = "TOWNSFOLK";

    ROLE_NAMES[14] = "VIRGIN";
    ROLE_TEAMS[14] = "GOOD";
    ROLE_CLASSES[14] = "TOWNSFOLK";

    ROLE_NAMES[15] = "SLAYER";
    ROLE_TEAMS[15] = "GOOD";
    ROLE_CLASSES[15] = "TOWNSFOLK";

    ROLE_NAMES[16] = "SOLDIER";
    ROLE_TEAMS[16] = "GOOD";
    ROLE_CLASSES[16] = "TOWNSFOLK";

    ROLE_NAMES[17] = "MAYOR";
    ROLE_TEAMS[17] = "GOOD";
    ROLE_CLASSES[17] = "TOWNSFOLK";

    //Outsiders
    ROLE_NAMES[18] = "BUTLER";
    ROLE_TEAMS[18] = "GOOD";
    ROLE_CLASSES[18] = "OUTSIDER";

    ROLE_NAMES[19] = "DRUNK";
    ROLE_TEAMS[19] = "GOOD";
    ROLE_CLASSES[19] = "OUTSIDER";

    ROLE_NAMES[20] = "RECLUSE";
    ROLE_TEAMS[20] = "GOOD";
    ROLE_CLASSES[20] = "OUTSIDER";

    ROLE_NAMES[21] = "SAINT";
    ROLE_TEAMS[21] = "GOOD";
    ROLE_CLASSES[21] = "OUTSIDER";

    printf("INIT DATA STRUCTURES...\n");
    //Init data structures
    printf("-RULE SET...\n");
    *rs = initRS();
    printf("-DONE!..\n");
    printf("-KNOWLEDGE BASE...\n");
    *kb = initKB(NUM_PLAYERS, NUM_DAYS);
    printf("-DONE!..\n");

    printf("BUILD RULES...\n");

    buildRules(*rs, *kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);
}

void buildRules(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders)
{
    int numEvil = numMinions+numDemons;
    int numGood = numPlayers-numEvil;

    int maxOutsiders = baseOutsiders + 2; //For Baron
     
    // ===========================================
    //  Setup memory
    // ===========================================
    printf("-SETUP MEMORY...\n");
    //Temporary string buffer for writing names into
    char buff[STRING_BUFF_SIZE];
    
    

    
    // ===========================================
    //  Make role based rules
    // ===========================================
    printf("-ROLE RULES...\n");
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        //A role implies a team
        //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE_TEAM>
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_TEAMS[i]);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[i]);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //A role implies a class
        //<PLAYER>is_<ROLE> => <PLAYER>is_<ROLE_CLASS>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_CLASSES[i]);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[i]);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //Only one player can have a role
        //<PLAYER_A>is_<ROLE> => <PLAYER_B>is_NOT<ROLE>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[i]);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[i]);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        
        //A player must have one role
        //<PLAYER>is_NOT_{<ROLE_A>, <ROLE_B>...} => <PLAYER>is_<ROLE_Z>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[i]);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff); //A player is role ROLE_NAME[i] if
        for (int j = 0; j < NUM_BOTCT_ROLES; j++)
        {
            if (j!=i)
            { //A player is NOT role ROLE_NAME[j] for all i!=j
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[j]);
                addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
            }
        }
        pushTempRule(rs);
        
    }

    
    // ===========================================
    //  More role rules
    // ===========================================
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        //Each player only has one role
        //<PLAYER>is_<ROLE_A> => <PLAYER>is_NOT<ROLE_B>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[i]);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        for (int j = 0; j < NUM_BOTCT_ROLES; j++)
        {
            if (i!=j)
            { //I and j are mutually exclusive
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[j]);
                setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
            }
        }
        pushTempRule(rs);
    }
    //works up to here
    // ===========================================
    //  Role Metadata implications
    // ===========================================
    printf("-METADATA RULES...\n");
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        //<PLAYER>is_<ROLE> => <METADATA>is_<ROLE>_in_PLAY
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY", ROLE_NAMES[i]);
        setTempRuleResultName(rs, kb, -1, "METADATA", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[i]);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

        //take the contrapositive
        //<PLAYER>is_NOT_<ROLE>_in_PLAY => <PLAYER>is_NOT_<ROLE>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[i]);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY", ROLE_NAMES[i]);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", buff);
        pushTempRule(rs);
    }
    //return;
    //Counting arguments for role metadata 
    //If found all minions in play set all unused minions to is_NOT_<MINION>_in_PLAY
    if (0 < numMinions && numMinions < 4)
    {
        for (int i = 0; i < 16; i++)
        {
            //Permute all subsets of size 4
            int count = 0;
            for (int j = 0; j < 4; j++)
            {
                if (((i >> j) & 1) == 1)
                {
                    count++;
                }
            }
            if (count == numMinions)
            {
                //<PLAYER>is_NOT_<MINION>_in_PLAY => <METADATA>is_<MINION>_in_PLAY
                setTempRuleParams(rs, 1,0);
                

                for (int j = 0; j < 4; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+2]);
                    }
                    else
                    {
                        setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+3]);
                    }
                }
                pushTempRule(rs);
            }
        }
    }
    int numUnusedMinions = 4-numMinions;
    if (0 < numUnusedMinions && numUnusedMinions < 4)
    {
        for (int i = 0; i < 16; i++)
        {
            //Permute all subsets of size 4
            int count = 0;
            for (int j = 0; j < 4; j++)
            {
                if (((i >> j) & 1) == 1)
                {
                    count++;
                }
            }
            if (count == numUnusedMinions)
            {
                //<PLAYER>is_<MINION>_in_PLAY => <METADATA>is_NOT_<MINION>_in_PLAY
                setTempRuleParams(rs, 1,0);
                

                for (int j = 0; j < 4; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+3]);
                    }
                    else
                    {
                        setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+2]);
                    }
                }
                pushTempRule(rs);
            }
        }
    }


    //If found all outsiders (either with or without baron) in play set all unused outsiders to is_NOT_<OUTSIDER>_in_PLAY

    //Base outsiders (requires knowing there is not a baron)
    if (baseOutsiders == 0)
    {
        //<METADATA>is_NOT_BARON_in_PLAY => <METADATA>is_NOT_<OUTSIDER>_in_PLAY (x4)
        setTempRuleParams(rs, 1,0);
        

        //The baron can't be in play for this
        addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_NOT_BARON_in_PLAY");
        for (int j = 0; j < 4; j++)
        {
            setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+37]);
        }
        pushTempRule(rs);
    }
    else if (0 < baseOutsiders && baseOutsiders < 4)
    {
        for (int i = 0; i < 16; i++)
        {
            //Permute all subsets of size 4
            int count = 0;
            for (int j = 0; j < 4; j++)
            {
                if (((i >> j) & 1) == 1)
                {
                    count++;
                }
            }
            if (count == baseOutsiders)
            {
                //<METADATA>is_<OUTSIDER>_in_PLAY (xA) AND <METADATA>is_NOT_BARON_in_PLAY => <METADATA>is_NOT_<OUTSIDER>_in_PLAY (x4-A)
                setTempRuleParams(rs, 1,0);
                

                //The baron can't be in play for this
                addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_NOT_BARON_in_PLAY");

                for (int j = 0; j < 4; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+36]);
                    }
                    else
                    {
                        setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+37]);
                    }
                }
                pushTempRule(rs);
            }
        }
    }
    
    int numUnusedOutsiders = 4-numMinions;
    if (0 < numUnusedOutsiders && numUnusedOutsiders < 4)
    {
        for (int i = 0; i < 16; i++)
        {
            //Permute all subsets of size 4
            int count = 0;
            for (int j = 0; j < 4; j++)
            {
                if (((i >> j) & 1) == 1)
                {
                    count++;
                }
            }
            if (count == numUnusedOutsiders)
            {
                //<METADATA>is_NOT_<OUTSIDER>_in_PLAY (xA) AND <METADATA>is_NOT_BARON_in_PLAY => <METADATA>is_<OUTSIDER>_in_PLAY (x4-A)
                setTempRuleParams(rs, 1,0);
                

                //The baron can't be in play for this
                addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_NOT_BARON_in_PLAY");

                for (int j = 0; j < 4; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+37]);
                    }
                    else
                    {
                        setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+36]);
                    }
                }
                pushTempRule(rs);
            }
        }
    }
    //Extended outsiders outsiders (requires knowing there is a baron)
    int numOutsidersWithBaron = baseOutsiders+2;
    if (numOutsidersWithBaron == 4)
    {
        //<METADATA>is_BARON_in_PLAY => <METADATA>is_<OUTSIDER>_in_PLAY (x4)
        setTempRuleParams(rs, 1,0);
        

        //The baron can't be in play for this
        addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_BARON_in_PLAY");
        for (int j = 0; j < 4; j++)
        {
            setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+36]);
        }
        pushTempRule(rs);
    }
    else if (0 < numOutsidersWithBaron && numOutsidersWithBaron < 4)
    {
        for (int i = 0; i < 16; i++)
        {
            //Permute all subsets of size 4
            int count = 0;
            for (int j = 0; j < 4; j++)
            {
                if (((i >> j) & 1) == 1)
                {
                    count++;
                }
            }
            if (count == numOutsidersWithBaron)
            {
                //<METADATA>is_<OUTSIDER>_in_PLAY (xA) AND <METADATA>is_BARON_in_PLAY => <METADATA>is_NOT_<OUTSIDER>_in_PLAY (x4-A)
                setTempRuleParams(rs, 1,0);
                

                //The baron can't be in play for this
                addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_BARON_in_PLAY");

                for (int j = 0; j < 4; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+36]);
                    }
                    else
                    {
                        setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+37]);
                    }
                }
                pushTempRule(rs);
            }
        }
    }
    
    int numUnusedOutsidersWithBaron = 4-numMinions;
    if (0 < numUnusedOutsidersWithBaron && numUnusedOutsidersWithBaron < 4)
    {
        for (int i = 0; i < 16; i++)
        {
            //Permute all subsets of size 4
            int count = 0;
            for (int j = 0; j < 4; j++)
            {
                if (((i >> j) & 1) == 1)
                {
                    count++;
                }
            }
            if (count == numUnusedOutsidersWithBaron)
            {
                //<METADATA>is_NOT_<OUTSIDER>_in_PLAY (xA) AND <METADATA>is_BARON_in_PLAY => <METADATA>is_<OUTSIDER>_in_PLAY (x4-A)
                setTempRuleParams(rs, 1,0);
                

                //The baron can't be in play for this
                addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_BARON_in_PLAY");

                for (int j = 0; j < 4; j++)
                {
                    if (((i >> j) & 1) == 1)
                    {
                        addConditionToTempRuleName(rs,kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+37]);
                    }
                    else
                    {
                        setTempRuleResultName(rs, kb, 0, "METADATA", kb->FUNCTION_NAME[2][j*2+36]);
                    }
                }
                pushTempRule(rs);
            }
        }
    }

    // ===========================================
    //  Team Deduction argument rules
    // ===========================================
    //A player can only have one class
    //<PLAYER>is_TOWNSFOLK => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_OUTSIDER
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_DEMON");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_MINION");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_TOWNSFOLK");
    pushTempRule(rs);

    //<PLAYER>is_OUTSIDER => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_TOWNSFOLK
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_DEMON");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_MINION");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_OUTSIDER");
    pushTempRule(rs);

    //<PLAYER>is_MINION => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_DEMON");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_MINION");
    pushTempRule(rs);


    //<PLAYER>is_DEMON => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_MINION");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_DEMON");
    pushTempRule(rs);

    //If a player is none of the roles within a class that player can't be that class (deduction)
    //<PLAYER>is_NOT_IMP => <PLAYER>is_NOT_DEMON
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_IMP");
    pushTempRule(rs);

    //The converse is also true
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_IMP");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_DEMON");
    pushTempRule(rs);

    //<PLAYER>is_NOT_BARON AND <PLAYER>is_NOT_SCARLET_WOMAN AND <PLAYER>is_NOT_SPY AND <PLAYER>is_NOT_POISONER=> <PLAYER>is_NOT_MINION
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_MINION");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_BARON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_SCARLET_WOMAN");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_SPY");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONER");
    pushTempRule(rs);

    //The converse is also true
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_BARON");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_SCARLET_WOMAN");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_SPY");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_POISONER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_MINION");
    pushTempRule(rs);

    //<PLAYER>is_NOT_BUTLER AND <PLAYER>is_NOT_DRUNK AND <PLAYER>is_NOT_RECLUSE AND <PLAYER>is_NOT_SAINT=> <PLAYER>is_NOT_OUTSIDER
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_BUTLER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_DRUNK");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_RECLUSE");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_SAINT");
    pushTempRule(rs);

    //The converse is also true
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_BUTLER");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_DRUNK");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_RECLUSE");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_SAINT");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    pushTempRule(rs);


    //<PLAYER>is_NOT_WASHERWOMAN AND <PLAYER>is_NOT_LIBRARIAN AND <PLAYER>is_NOT_INVESTIGATOR AND <PLAYER>is_NOT_CHEF
    //AND <PLAYER>is_NOT_EMPATH AND <PLAYER>is_NOT_FORTUNE_TELLER AND <PLAYER>is_NOT_UNDERTAKER AND <PLAYER>is_NOT_MONK
    //AND <PLAYER>is_NOT_RAVENKEEPER AND <PLAYER>is_NOT_VIRGIN AND <PLAYER>is_NOT_SLAYER AND <PLAYER>is_NOT_SOLDIER
    //AND <PLAYER>is_NOT_MAYOR => <PLAYER>is_NOT_TOWNSFOLK
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_WASHERWOMAN");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_LIBRARIAN");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_INVESTIGATOR");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_CHEF");

    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_EMPATH");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_FORTUNE_TELLER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_UNDERTAKER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_MONK");

    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_RAVENKEEPER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_VIRGIN");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_SLAYER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_SOLDIER");

    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_MAYOR");
    pushTempRule(rs);

    //The converse is also true
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_WASHERWOMAN");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_LIBRARIAN");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_INVESTIGATOR");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_CHEF");

    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_EMPATH");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_FORTUNE_TELLER");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_UNDERTAKER");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_MONK");

    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_RAVENKEEPER");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_VIRGIN");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_SLAYER");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_SOLDIER");

    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_MAYOR");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    pushTempRule(rs);


    //A player must have a class
    //<PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_OUTSIDER => <PLAYER>is_TOWNSFOLK
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_TOWNSFOLK");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_MINION");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    pushTempRule(rs);

    //<PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_TOWNSFOLK => <PLAYER>is_OUTSIDER
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_OUTSIDER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_MINION");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    pushTempRule(rs);

    //<PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK => <PLAYER>is_MINION
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_MINION");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    pushTempRule(rs);

    //<PLAYER>is_NOT_MINION AND <PLAYER>is_NOT_OUTSIDER AND <PLAYER>is_NOT_TOWNSFOLK => <PLAYER>is_DEMON
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_MINION");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    pushTempRule(rs);


    //Deduction by clases
    //<PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION => <PLAYER>is_GOOD
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_GOOD");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_MINION");
    pushTempRule(rs);

    //<PLAYER>is_NOT_TOWNSFOLK AND <PLAYER>is_NOT_OUTSIDER => <PLAYER>is_EVIL
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_EVIL");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    pushTempRule(rs);

    //Other way of the iff (if and only if)
    //<PLAYER>is_GOOD => <PLAYER>is_NOT_DEMON AND <PLAYER>is_NOT_MINION
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_MINION");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_DEMON");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_GOOD");
    pushTempRule(rs);

    //<PLAYER>is_EVIL => <PLAYER>is_NOT_TOWNSFOLK AND <PLAYER>is_NOT_OUTSIDER
    setTempRuleParams(rs, 1,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_TOWNSFOLK");
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_OUTSIDER");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_EVIL");
    pushTempRule(rs);

    // ===========================================
    //  Team Counting argument rules
    // ===========================================
    //RULE DISABLED DUE TO BAD PERFORMANCE
    /*
    //If all good players have been found the rest are evil
    //<PLAYER_1>is_GOOD AND ... <PLAYER_[num good players]>is_GOOD => <PLAYER_A>is_EVIL
    RULES[NUM_RULES]->varCount = numGood;
    RULES[NUM_RULES]->varsMutuallyExclusive=1;
    
    setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_EVIL"); 
    
    for (int j = 0; j < numGood; j++)
    {
        addConditionToTempRuleName(rs,kb, j, "PLAYERS", "is_GOOD");
    }
    pushTempRule(rs);
    */
    //If all evil players have been found the rest are good
    //<PLAYER_1>is_EVIL AND ... <PLAYER_[num evil players]>is_EVIL => <PLAYER_A>is_GOOD
    setTempRuleParams(rs, numEvil,1);
    
    setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_GOOD"); 
    for (int j = 0; j < numEvil; j++)
    {
        addConditionToTempRuleName(rs,kb, j, "PLAYERS", "is_EVIL");
    }
    pushTempRule(rs);

    //If all minion players have been found the rest are not minions
    //<PLAYER_1>is_MINION AND ... <PLAYER_[num minion players]>is_MINION => <PLAYER_A>is_NOT_MINION
    setTempRuleParams(rs, numMinions,1);
    
    setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_NOT_MINION"); 
    for (int j = 0; j < numMinions; j++)
    {
        addConditionToTempRuleName(rs,kb, j, "PLAYERS", "is_MINION");
    }
    pushTempRule(rs);

    //If all demon players have been found the rest are not demons
    //<PLAYER_1>is_DEMON AND ... <PLAYER_[num minion players]>is_DEMON => <PLAYER_A>is_NOT_DEMON
    setTempRuleParams(rs, numDemons,1);
    
    setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_NOT_DEMON"); 
    for (int j = 0; j < numDemons; j++)
    {
        addConditionToTempRuleName(rs,kb, j, "PLAYERS", "is_DEMON");
    }
    pushTempRule(rs);


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
        //<METADATA>is_NOT_POISONER_in_PLAY => <PLAYER>is_NOT_poisoned_NIGHT<i>
        setTempRuleParams(rs, 1,0);
        
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_poisoned_NIGHT%d", i);
        setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);
        addConditionToTempRuleName(rs,kb, 0, "METADATA", "is_NOT_POISONER_in_PLAY");
        pushTempRule(rs);

        //Contrapositive logic
        // <PLAYER>is_poisoned_NIGHT<i> => <METADATA>is_POISONER_in_PLAY
        setTempRuleParams(rs, 1,0);
        
        setTempRuleResultName(rs, kb, -1, "METADATA", "is_POISONER_in_PLAY");
        snprintf(buff, STRING_BUFF_SIZE, "is_poisoned_NIGHT%d", i);
        addConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff);
        pushTempRule(rs);

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

    }
    // ===========================================
    //  Red Herring rules
    // ===========================================
    //<PLAYER>is_REDHERRING => <PLAYER_Y>is_NOT_REDHERRING
    setTempRuleParams(rs, 2,0);
    
    setTempRuleResultName(rs, kb, -1, "PLAYERS", "is_NOT_REDHERRING");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_REDHERRING");
    pushTempRule(rs);

    //<PLAYER>is_REDHERRING => <PLAYER>is_GOOD
    setTempRuleParams(rs, 2,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_GOOD");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_REDHERRING");
    pushTempRule(rs);

    //Contrapositive logic
    //<PLAYER>is_EVIL => <PLAYER>is_NOT_REDHERRING
    setTempRuleParams(rs, 2,0);
    
    setTempRuleResultName(rs, kb, 0, "PLAYERS", "is_NOT_REDHERRING");
    addConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_EVIL");
    pushTempRule(rs);
}