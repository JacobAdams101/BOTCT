#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rules.h"
#include "knowledge.h"
#include "constants.h"
#include "tb.h"

void printTitle(char *title)
{
    printf("======================================================================================================\n");
    printf("                                 %s\n", title);
    printf("======================================================================================================\n");
}
void printHeading(char *title)
{
    printf("\n");
    printf("   %s\n", title);
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
}

void setup(int *numPlayers, int *numMinions, int *numDemons, int *baseOutsiders)
{

    printTitle("BLOOD ON THE CLOCKTOWER SOLVER");
    printf("By Jacob Adams\n");
    printHeading("INTRODUCTION");
    printf("\n");
    printf("This program is used to aid in solving standard Trouble Brewing games of BOTCT\n");
    printf("Do NOT use without the storytelle's permission\n");
    printHeading("SETUP");
    printf("How many players are in the game?:\n");
    scanf("%d", numPlayers);

    printf("How many minions are in the game?:\n");
    scanf("%d", numMinions);

    printf("How many demons are in the game?:\n");
    scanf("%d", numDemons);

    printf("How many outsiders would there be in the game without a Baron?:\n");
    scanf("%d", baseOutsiders);
}

int getRoleIdFromString(char* roleName)
{
    for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
    {
        if (strcmp(roleName,ROLE_NAMES[roleID]) == 0)
        {
            return roleID;
        }
    }
    return -1;
}

int getRoleIDInput(char* message)
{
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string
    int roleID = -1;
    printf("%s:\n", message);
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        printf("%s\n", ROLE_NAMES[i]);
    }
    while (roleID == -1)
    {
        scanf("%255s", input); // Read a string (up to 99 characters to leave space for the null terminator)
        roleID = getRoleIdFromString(input);
        if (roleID == -1)
        {
            printf("ERROR: Invalid string!\n");
        }
    }
    return roleID;
}

void shown_role(KnowledgeBase* kb)
{
    int playerID;
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleID = -1;

    printf("ENERTING: SHOWN ROLE\n");

    printf("For player?:\n");
    scanf("%d", &playerID); // Read player ID

    roleID = getRoleIDInput("Role?");

    if (roleID < 5) //If role ID is on evil team, we're certain that they're not drunk
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[roleID]);
        addKnowledgeName(kb, "PLAYERS", playerID, buff);
    }
    else
    {
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (i == roleID || i == 19)
            { 
            }
            else
            { //Player can only be their role or drunk
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[i]);
                addKnowledgeName(kb, "PLAYERS", playerID, buff);
            }
        }
    }
}

void roleNotInGame(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleID = -1;

    printf("ENERTING: ROLE NOT IN GAME\n");

    roleID = getRoleIDInput("Role not in game?");

    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY", ROLE_NAMES[roleID]);
    addKnowledgeName(kb, "METADATA", 0, buff);
}

void noptions(KnowledgeBase* kb)
{
    
    printf("ENERTING: n POSSIBILITIES\n");

    int n;
    printf("Num Roles?:\n");
    scanf("%d", &n); // Read player ID

    int playerID;
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleIDs[n];

    

    printf("For player?:\n");
    scanf("%d", &playerID); // Read player ID


    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        printf("%s\n", ROLE_NAMES[i]);
    }
    for (int j = 0; j < n; j++)
    {
        roleIDs[j] = -1;
    }
    for (int j = 0; j < n; j++)
    {
        printf("Role %d?:\n",j);
        
        while (roleIDs[j] == -1)
        {
            scanf("%255s", input); // Read a string (up to 99 characters to leave space for the null terminator)
            roleIDs[j] = getRoleIdFromString(input);
            if (roleIDs[j] == -1)
            {
                printf("ERROR: Invalid string!\n");
            }
        }
    }

    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        int inRole = 0;
        for (int j = 0; j < n; j++)
        {
            if (roleIDs[j] == i)
            {
                inRole = 1;
            }
        }
        if (i == 19 || i < 5) //If i is on evil team or Drunk
        { //People as these roles could always give a false 3 for 3
            inRole = 1;
        }
        if (inRole == 1)
        { 
        }
        else
        { //Player can only be their role or drunk
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[i]);
            addKnowledgeName(kb, "PLAYERS", playerID, buff);
        }
    }
}

void poisoned(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int playerID;
    int night;

    printf("ENERTING: PLAYER POISONED\n");

    printf("For player?:\n");
    scanf("%d", &playerID); // Read player ID

    printf("On night?:\n");
    scanf("%d", &night); // Read player ID

    snprintf(buff, STRING_BUFF_SIZE, "is_poisoned_NIGHT%d", night);
    addKnowledgeName(kb, "PLAYERS", playerID, buff);
}

void diedInNight(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int playerID;
    int night;

    printf("ENERTING: PLAYER DIED IN NIGHT\n");

    printf("For player?:\n");
    scanf("%d", &playerID); // Read player ID

    printf("On night?:\n");
    scanf("%d", &night); // Read player ID

    snprintf(buff, STRING_BUFF_SIZE, "died_NIGHT%d", night);
    addKnowledgeName(kb, "PLAYERS", playerID, buff);
    addKnowledgeName(kb, "PLAYERS", playerID, "died_in_NIGHT");
}

void hung(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int playerID;
    int night;

    printf("ENERTING: PLAYER HUNG\n");

    printf("For player?:\n");
    scanf("%d", &playerID); // Read player ID

    printf("On night?:\n");
    scanf("%d", &night); // Read player ID

    snprintf(buff, STRING_BUFF_SIZE, "died_NIGHT%d", night);
    addKnowledgeName(kb, "PLAYERS", playerID, buff);
    addKnowledgeName(kb, "PLAYERS", playerID, "died_by_HANGING");
}

void addPingRule(KnowledgeBase* kb, RuleSet* rs)
{
    char inputPingType[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    char WASHERWOMAN_PING[] = "WASHERWOMAN";
    char LIBRARIAN_PING[] = "LIBRARIAN";
    char INVESTIGATOR_PING[] = "INVESTIGATOR";
    char CHEF_PING[] = "CHEF";
    char EMPATH_PING[] = "EMPATH";
    char FORTUNE_TELLER_PING[] = "FORTUNE_TELLER";
    char UNDERTAKER_PING[] = "UNDERTAKER";
    //char MONK_PING[] = "MONK"; //Monks don't have pings
    char RAVENKEEPER_PING[] = "RAVENKEEPER";
    int pingTypeID = -1;

    int playerIDinfoFrom;

    printf("ENERTING: PLAYER PING (Unreliable information)\n");

    printf("Info from player?:\n");
    scanf("%d", &playerIDinfoFrom); // Read player ID

    printf("Ping Type?:\n");
    printf("- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n", WASHERWOMAN_PING, LIBRARIAN_PING, INVESTIGATOR_PING, CHEF_PING, EMPATH_PING, FORTUNE_TELLER_PING, UNDERTAKER_PING, RAVENKEEPER_PING);
    while (pingTypeID == -1)
    {
        scanf("%255s", inputPingType); // Read a string (up to 99 characters to leave space for the null terminator)
        if (strcmp(inputPingType,WASHERWOMAN_PING) == 0)
        {
            pingTypeID = 0;
        } 
        else if (strcmp(inputPingType,LIBRARIAN_PING) == 0)
        {
            pingTypeID = 1;
        }
        else if (strcmp(inputPingType,INVESTIGATOR_PING) == 0)
        {
            pingTypeID = 2;
        }
        else if (strcmp(inputPingType,CHEF_PING) == 0)
        {
            pingTypeID = 3;
        }
        else if (strcmp(inputPingType,EMPATH_PING) == 0)
        {
            pingTypeID = 4;
        }
        else if (strcmp(inputPingType,FORTUNE_TELLER_PING) == 0)
        {
            pingTypeID = 5;
        }
        else if (strcmp(inputPingType,UNDERTAKER_PING) == 0)
        {
            pingTypeID = 6;
        }
        else if (strcmp(inputPingType,RAVENKEEPER_PING) == 0)
        {
            pingTypeID = 7;
        }
        if (pingTypeID == -1)
        {
            printf("ERROR: Invalid string!\n");
        }
    }

    if (pingTypeID == 0)
    { //WASHERWOMAN_PING
        /*
        Start knowing one of two players is a particular townsfolk
        */

        int selectedRole = -1;
        int playerX = -1;
        int playerY = -1;

        selectedRole = getRoleIDInput("Role Shown?");

        printf("For player 1?:\n");
        scanf("%d", &playerX); // Read player ID

        printf("For player 2?:\n");
        scanf("%d", &playerY); // Read player ID
        //Originally this but converted to the stronger argument
        //If <PLAYER_INFO>is_WASHERWOMAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
        //It was converted to this as <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_X>is_NOT_SPY
        //If <PLAYER_INFO>is_WASHERWOMAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_SPY AND <PLAYER_Y>is_NOT_SPY => <METADATA>is_<ROLE>_in_PLAY
        setTempRuleParams(rs, 3,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY", ROLE_NAMES[selectedRole]);
        setTempRuleResultName(rs, kb, 0, "METADATA", buff);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_WASHERWOMAN", playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_poisoned_NIGHT0", playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY", playerX);
        addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY", playerY);
        pushTempRule(rs);

        //If <PLAYER_INFO>is_WASHERWOMAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_SPY AND <PLAYER_Y>is_NOT_SPY => <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]
        for (int i = 0; i < kb->SET_SIZES[0]; i++)
        {
            if (i == playerX || i == playerY)
            {

            }
            else
            {
                setTempRuleParams(rs, 2,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[selectedRole]);
                setTempRuleResultName(rs, kb, -i-1000, "PLAYERS", buff); //iterate over players not X or Y
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_WASHERWOMAN", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_poisoned_NIGHT0", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY", playerX);
                addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY", playerY);
                pushTempRule(rs);
            }
        }

    }
    else if (pingTypeID == 1)
    { //LIBRARIAN_PING
        /*
        Start knowing one of two players is a particular outsider
        */

        int selectedRole = -1;
        int playerX = -1;
        int playerY = -1;

        selectedRole = getRoleIDInput("Role Shown?");

        printf("For player 1?:\n");
        scanf("%d", &playerX); // Read player ID

        printf("For player 2?:\n");
        scanf("%d", &playerY); // Read player ID
        //Originally this but converted to the stronger argument
        //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
        //It was converted to this as <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_X>is_NOT_SPY
        //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_SPY AND <PLAYER_Y>is_NOT_SPY => <METADATA>is_<ROLE>_in_PLAY
        setTempRuleParams(rs, 3,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY", ROLE_NAMES[selectedRole]);
        setTempRuleResultName(rs, kb, 0, "METADATA", buff);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_LIBRARIAN", playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_poisoned_NIGHT0", playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY", playerX);
        addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY", playerY);
        pushTempRule(rs);

        //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_SPY AND <PLAYER_Y>is_NOT_SPY=> <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]
        for (int i = 0; i < kb->SET_SIZES[0]; i++)
        {
            if (i == playerX || i == playerY)
            {

            }
            else
            {
                setTempRuleParams(rs, 2,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[selectedRole]);
                setTempRuleResultName(rs, kb, -i-1000, "PLAYERS", buff); //iterate over players not X or Y
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_LIBRARIAN", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_poisoned_NIGHT0", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY", playerX);
                addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY", playerY);
                pushTempRule(rs);
            }
        }
    }
    else if (pingTypeID == 2)
    { //INVESTIGATOR_PING
        /*
        Start knowing one of two players is a particular minion
        */

        int selectedRole = -1;
        int playerX = -1;
        int playerY = -1;

        selectedRole = getRoleIDInput("Role Shown?");

        printf("For player 1?:\n");
        scanf("%d", &playerX); // Read player ID

        printf("For player 2?:\n");
        scanf("%d", &playerY); // Read player ID
        //Originally this but converted to the stronger argument
        //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_RECLUSE_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
        //It was converted to this as <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_X>is_NOT_SPY
        //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_Y>is_NOT_RECLUSE => <METADATA>is_<ROLE>_in_PLAY
        setTempRuleParams(rs, 3,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY", ROLE_NAMES[selectedRole]);
        setTempRuleResultName(rs, kb, 0, "METADATA", buff);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_INVESTIGATOR", playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_poisoned_NIGHT0", playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE", playerX);
        addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_RECLUSE", playerY);
        pushTempRule(rs);

        //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_Y>is_NOT_RECLUSE=> <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]
        for (int i = 0; i < kb->SET_SIZES[0]; i++)
        {
            if (i == playerX || i == playerY)
            {

            }
            else
            {
                setTempRuleParams(rs, 2,0);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s", ROLE_NAMES[selectedRole]);
                setTempRuleResultName(rs, kb, -i-1000, "PLAYERS", buff); //iterate over players not X or Y
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_INVESTIGATOR", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_poisoned_NIGHT0", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE", playerX);
                addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_RECLUSE", playerY);
                pushTempRule(rs);
            }
        }
    }
    else if (pingTypeID == 3)
    { //CHEF_PING
        /*
        Start knowing how many pairs of evil players there are
        */

        //0 - implies two adjancent players cant be bad 

    }
    else if (pingTypeID == 4)
    { //EMPATH_PING
        /*
        Each night learn how many of your alive neighbours are evil
        */
        //For 2 good 0 evil 
        //If <PLAYER_INFO>is_EMPATH AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_X & Y>is_NOT_<ROLE> (where ROLE != GOOD, ROLE != SPY) {player X and Y is either good or a spy}
    
        //For 0 good 2 evil
        //If <PLAYER_INFO>is_EMPATH AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_X & Y>is_NOT_<ROLE> (where ROLE != EVIL, ROLE != RECLUSE) {player X and Y is either evil or a recluse}
    }
    else if (pingTypeID == 5)
    { //FORTUNE_TELLER_PING
        /*
        Each night, choose 2 players: you learn if either is a Demon. There is 1 good player that registers falsely to you.
        */
        int count = -1;
        int playerX = -1;
        int playerY = -1;
        int night = -1;

        printf("What was ping? (0 - NO, 1 - YES):\n");
        scanf("%d", &count); // Read player ID

        printf("For player 1?:\n");
        scanf("%d", &playerX); // Read player ID

        printf("For player 2?:\n");
        scanf("%d", &playerY); // Read player ID

        printf("On night?:\n");
        scanf("%d", &night); // Read night

        if (count == 0)
        {
            //If 0 Ping
            //If <PLAYER_INFO>is_FORTUNE_TELLER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_X>is_NOT_DEMON
            //If <PLAYER_INFO>is_FORTUNE_TELLER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_Y>is_NOT_DEMON
        }
        else
        {
            //If 1 Ping
            
        }
    }
    else if (pingTypeID == 6)
    { //UNDERTAKER_PING
        /*
        Each night*, you learn which character died by execution today.
        */
        int selectedRole = -1;
        int playerX = -1;
        int night = -1;

        selectedRole = getRoleIDInput("Role Shown?");

        printf("For player?:\n");
        scanf("%d", &playerX); // Read player ID

        printf("On night?:\n");
        scanf("%d", &night); // Read night

        //If <PLAYER_INFO>is_UNDERTAKER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_X>is_NOT_SPY => <PLAYER_X>is_<ROLE>
        setTempRuleParams(rs, 3,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[selectedRole]);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_UNDERTAKER", playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_poisoned_NIGHT%d", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE", playerX);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY", playerX);
        pushTempRule(rs);
    }
    else if (pingTypeID == 7)
    { //RAVENKEEPER_PING
        /*
        If you die at night, you are woken to choose a player: you learn their character.
        */
        int selectedRole = -1;
        int playerX = -1;
        int night = -1;

        selectedRole = getRoleIDInput("Role Shown?");

        printf("For player?:\n");
        scanf("%d", &playerX); // Read player ID

        printf("On night?:\n");
        scanf("%d", &night); // Read night

        //If <PLAYER_INFO>is_RAVENKEEPER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_X>is_NOT_SPY => <PLAYER_X>is_<ROLE>
        setTempRuleParams(rs, 3,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_%s", ROLE_NAMES[selectedRole]);
        setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_RAVENKEEPER", playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_poisoned_NIGHT%d", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE", playerX);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY", playerX);
        pushTempRule(rs);

    }
    //printf("On night?:\n");
    //scanf("%d", &night); // Read player ID

    
}

void add_info(KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printHeading("ADD INFORMATION");

    int running = 1;
    while (running)
    {
        char SHOWN_ROLE[] = "SR";
        char ROLE_NOT_IN_GAME[] = "RNIG";
        char PING[] = "PNG";
        char N_POSSIBILITIES[] = "TFT";
        char POISONED[] = "PS";
        char DIED[] = "D";
        char HUNG[] = "H";
        char FINISH[] = "FINISH";
        printf("ADD INFORMATION:\n");
        printf("- Type '%s' to enter what role a player was shown:\n", SHOWN_ROLE);
        printf("- Type '%s' to enter a role not in the game:\n", ROLE_NOT_IN_GAME);
        printf("- Type '%s' to submit a ping:\n", PING);
        printf("- Type '%s' to submit n possibilities for a player:\n", N_POSSIBILITIES);
        printf("- Type '%s' to submit that a player that is confirmed poisoned:\n", POISONED);
        printf("- Type '%s' to submit that a player died :\n", DIED);
        printf("- Type '%s' to submit that a player was hung:\n", HUNG);
        printf("\n");
        printf("- Type '%s' to finish entering data:\n", FINISH);

        
 
        scanf("%255s", buff); // Read a string (up to 99 characters to leave space for the null terminator)

        if (strcmp(buff,SHOWN_ROLE) == 0)
        {
            shown_role(kb);
        }
        else if (strcmp(buff,ROLE_NOT_IN_GAME) == 0)
        {
            roleNotInGame(kb);
        }
        else if (strcmp(buff,PING) == 0)
        {
            addPingRule(kb, rs);
        }
        else if (strcmp(buff,N_POSSIBILITIES) == 0)
        {
            noptions(kb);
        }
        else if (strcmp(buff,POISONED) == 0)
        {
            poisoned(kb);
        }
        else if (strcmp(buff,DIED) == 0)
        {
            diedInNight(kb);
        }
        else if (strcmp(buff,HUNG) == 0)
        {
            hung(kb);
        }
        else if (strcmp(buff,FINISH) == 0)
        {
            printf("EXITING...");
            running = 0;
        }
        else
        {
            printf("ERROR: Invalid string!\n");
        }

    }
}

int main()
{
    /*
    DEFAULT SETTINGS 
    int NUM_PLAYERS = 12;
    int NUM_MINIONS = 2;
    int NUM_DEMONS = 1;
    int BASE_OUTSIDERS = 2;

    int NUM_DAYS = 10;
    */
    int NUM_PLAYERS;
    int NUM_MINIONS;
    int NUM_DEMONS;
    int BASE_OUTSIDERS;

    int NUM_DAYS = 10;
    setup(&NUM_PLAYERS, &NUM_MINIONS, &NUM_DEMONS, &BASE_OUTSIDERS);

    printHeading("CREATING GAME..."); //UI HEADING
    printf("There are %d players in the game\n", NUM_PLAYERS);
    printf("There are %d minions in the game\n", NUM_MINIONS);
    printf("There are %d demons in the game\n", NUM_DEMONS);
    printf("There are %d(+2) outsiders in the game\n", BASE_OUTSIDERS);

    printf("INITIALIZE KNOWLEDGE BASE...\n");
    KnowledgeBase* kb = NULL;
    RuleSet* rs = NULL;

    initTB(&rs, &kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS, NUM_DAYS);

    printRules(rs, kb);

    KnowledgeBase* temp_kb = initKB(NUM_PLAYERS, NUM_DAYS);

    printf("BEGIN GAME LOOP...\n");

    int contradiction = 0;
    
    while (1)
    {
        

        //Create copy as backup incase of contradictions
        copyTo(temp_kb, kb);
        add_info(kb, rs);

        
        printHeading("INFER FACTS"); //UI HEADING
        
        for (int i = 0; i < 4; i++)
        {
            printf("INFER FACTS [ROUND%d]:\n",i);
            inferknowledgeBaseFromRules(rs, kb);
        }
        //Check for contradictions
        contradiction = hasExplicitContradiction(kb);

        if (contradiction == 0)
        {
            printHeading("KNOWLEDGE BASE"); //UI HEADING
            printKnowledgeBase(kb);
            printHeading("PLAYER TABLE"); //UI HEADING
            printPlayerTable(kb);
            printHeading("ROLE TABLE"); //UI HEADING
            printRoleTable(kb);
        }

        if (contradiction == 1)
        {
            printHeading("CONTRADICTION FOUND"); //UI HEADING
            printf("Rolling back Knowledge base\n");
            //Roll back knowledge base
            copyTo(kb, temp_kb);
        }

    }

    return 0;
}