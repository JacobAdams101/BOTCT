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

#include "rules.h"
#include "knowledge.h"
#include "constants.h"
#include "scripts.h"
#include "ui.h"
#include "util.h"

/**
 * printTitle() - prints a title to the terminal
 * 
 * @title the title string
 * @subheading the subheading string
*/
void printTitle(char *title, char *subheading)
{
    PRINT_TITLE
    printf("======================================================================================================\n");
    printf("\n");
    printf("                                 %s\n", title);
    printf("                                                                  %s\n", subheading);
    printf("======================================================================================================\n");
    PRINT_END
}

/**
 * printHeading() - prints a heading to the terminal
 * 
 * @title the title string
*/
void printHeading(char *title)
{
    PRINT_TITLE
    printf("\n");
    printf("   %s\n", title);
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    PRINT_END
}

/**
 * printRedHeading() - prints a heading to the terminal
 * 
 * @title the title string
*/
void printRedHeading(char *title)
{
    PRINT_TITLE
    PRINT_RED
    printf("\n");
    printf("   %s\n", title);
    printf("‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    PRINT_END
}


/**
 * setup() - ask the questions to the user to sertup a game
 * and write the results into function parameters
 * 
 * @numPlayers OUTPUTS the number of players in the game
 * @numMinions OUTPUTS the number of base starting minions in the game
 * @numDemons OUTPUTS the number of base starting demons in the game
 * @baseOutsiders OUTPUTS the number of base starting outsiders in the game
 * @script OUTPUTS the scriptID
*/
void setup(int *numPlayers, int *numMinions, int *numDemons, int *baseOutsiders, int *script)
{

    printTitle("BLOOD ON THE CLOCKTOWER SOLVER", "By Jacob Adams");
    printHeading("INTRODUCTION");
    printf("\n");
    printf("This program is used to aid in solving standard Trouble Brewing games of BOTCT\n");
    printf("Do NOT use without the storyteller's permission\n");
    printHeading("INTRODUCTION");
    printf("Nights are seperated by different numbers\n");
    printf("| SEEN ROLE | NIGHT | DAYTIME | VOTING | NIGHT | DAYTIME | VOTING | NIGHT | DAYTIME | VOTING | NIGHT |\n");
    printf("|_DAY 0_____________|_DAY 1____________________|_DAY 2____________________|_DAY 3____________________|\n");
    printHeading("SETUP");
    //Script info
    *script = getInt("What script are you playing 0-TB, 1-S&V, 2-BMR?", 0, 3);

    //Expected night 1 Player count infor
    *numPlayers = getInt("How many players are in the game?", 3, 17);
    *numMinions = getInt("How many minions are in the game?", 0, 5);
    *numDemons = getInt("How many demons are in the game?", 0, 5);
    *baseOutsiders = getInt("How many outsiders would there be in the game without a Baron?", 0, 5);
}

/**
 * getNames() - gets a list of names
 * 
 * @names stores the strings enters
 * @numPlayers OUTPUTS the number of players to get the names of
*/
void getNames(char* names[NUM_SETS][MAX_SET_ELEMENTS], int numPlayers)
{
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string
    int set = 0;

    for (int player = 0; player < numPlayers; player++)
    {
        printf("Player %d name:\n", player);
        scanf("%255s", input); // Read a string (up to 99 characters to leave space for the null terminator)
        snprintf(names[set][player], STRING_BUFF_SIZE, "%s", input);
    }
    
}

/**
 * getRoleIdFromString() - used to convert the role name into an index
 * 
 * @roleName the NAME of the role
 * 
 * @return returns the roleID/index associated with the string, -1 if no role is found
*/
int getRoleIdFromString(char* roleName)
{
    for (int roleID = 0; roleID < NUM_BOTCT_ROLES; roleID++)
    {
        if (ROLE_IN_SCRIPT[roleID] == 1)
        {
            if (strcasecmp(roleName,ROLE_NAMES[roleID]) == 0)
            {
                return roleID;
            }
        }
    }
    return -1;
}

/**
 * printRolesInScript() - prints all the roles to the terminal
*/
static void printRolesInScript()
{
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        if (ROLE_IN_SCRIPT[i] == 1)
        {
            printf("%s\n", ROLE_NAMES[i]);
        }
    }
}

/**
 * getRoleIDInput() - get a role input from the user
 * 
 * @message the message to ask the user
 * 
 * @return the roleID/index of the entered role
*/
int getRoleIDInput(char* message)
{
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string
    int roleID = -1;
    printf("%s:\n", message);
    printRolesInScript();
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

/**
 * getPlayerIDInput() - get the name of the player from the user
 * 
 * @kb the knowledge base
 * @message the message to ask the user
 * 
 * @return the playerID/index of the entered player
*/
int getPlayerIDInput(KnowledgeBase* kb, char* message)
{
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string
    int roleID = -1;
    printf("%s:\n", message);
    while (roleID == -1)
    {
        scanf("%255s", input); // Read a string (up to 99 characters to leave space for the null terminator)
        
        roleID = getSetElementIDWithName(kb, 0, input, 0);
        
        if (roleID == -1)
        {
            printf("ERROR: Invalid string!\n");
        }
    }
    return roleID;
}

/**
 * getInt() - get an integer between min (inclusive) and max (exclusive)
 * 
 * @min min number (inclusive)
 * @max max number (exclusive)
 * @message the message to ask the user
 * 
 * @return a number between min <= NUM < max
*/
int getInt(char* message, int min, int max)
{
    int input;
    int loop = 1;
    char ch;
    
    printf("%s:\n", message);
    while (loop)
    {
        if (scanf("%d", &input) == 1)
        {
            if (min <= input && input < max)
            {
                return input;
            }
        }
        printf("ERROR: Invalid number!\n");
        // Clear the input buffer
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
    return -1;
}

/**
 * getYNInput() - 
 * 
 * @message the message to ask the user
 * 
 * @return 1 for yes, 0 for no
*/
int getYNInput(char* message)
{
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string
    int result = -1;
    printf("%s (Y/N):\n", message);
    while (result == -1)
    {
        scanf("%255s", input); // Read a string (up to 99 characters to leave space for the null terminator)

        if (strcasecmp(input,"y") == 0 || strcasecmp(input,"yes") == 0) result = 1;
        if (strcasecmp(input,"n") == 0 || strcasecmp(input,"no") == 0) result = 0;
    }
    return result;
}
/**
 * shown_role() - used to update the knowledge base assuming a player was shown a certain role
 * this function also takes into account the innacuracies of being shown roles (drunk, lunatic etc.)
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void shown_role(KnowledgeBase* kb)
{
    int playerID;
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleID = -1;
    int night;

    printf("ENERTING: SHOWN ROLE\n");

    playerID = getPlayerIDInput(kb, "For player?"); // Read player ID

    night = getInt("On night?", 0, NUM_DAYS);

    roleID = getRoleIDInput("Role?");

    if (strcmp(ROLE_CLASSES[roleID], "DEMON") == 0) //If demon team, they're either a demon or the lunatic
    {
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if ((strcmp(ROLE_NAMES[i], ROLE_NAMES[roleID]) == 0) || (strcmp(ROLE_NAMES[i], "LUNATIC") == 0))
            { //Demon could either be lunatic or actual role

            }
            else
            { 
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                addKnowledgeName(kb, "PLAYERS", playerID, buff);
            }
        }

    }
    else if (strcmp(ROLE_CLASSES[roleID], "MINION") == 0) //If minion, we're certain they are a minion
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[roleID], night);
        addKnowledgeName(kb, "PLAYERS", playerID, buff);
    }
    else if (strcmp(ROLE_CLASSES[roleID], "OUTSIDER") == 0) //If outsider, we're certain they are an outsider
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[roleID], night);
        addKnowledgeName(kb, "PLAYERS", playerID, buff);
    }
    else
    { //Townsfolk
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if ((strcmp(ROLE_NAMES[i], ROLE_NAMES[roleID]) == 0) || (strcmp(ROLE_NAMES[i], "DRUNK") == 0))
            { 
            }
            else
            { //Player can only be their role or drunk
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
                addKnowledgeName(kb, "PLAYERS", playerID, buff);
            }
        }
    }
}

/**
 * roleNotInGame() - used to update the knowledge base assuming a role is not in the game
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static  void roleNotInGame(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleID = -1;
    int night;

    printf("ENERTING: ROLE NOT IN GAME\n");

    roleID = getRoleIDInput("Role not in game?");

    night = getInt("On night? (Type -1 for every night)", -1, NUM_DAYS);

    if (night == -1)
    {
        for (int i = 0; i < NUM_DAYS; i++)
        {
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[roleID], i);
            addKnowledgeName(kb, "METADATA", 0, buff);
        }
    }
    else
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_in_PLAY_[NIGHT%d]", ROLE_NAMES[roleID], night);
        addKnowledgeName(kb, "METADATA", 0, buff);
    }
}

/**
 * noptions() - given that a player gives you n options ("Three for Three" etc.)
 * update the knowledge base
 * Assume good "not mad" players always tell the truth (to the best of their knowledge [drunk etc])
 * Assume bad players will be lying about a good role
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void noptions(KnowledgeBase* kb)
{
    
    printf("ENERTING: n POSSIBILITIES\n");

    int n;
    printf("Num Roles?:\n");
    scanf("%d", &n); // Read player ID

    int playerID;
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleIDs[n];
    int night;

    playerID = getPlayerIDInput(kb, "For player?"); // Read player ID

    night = getInt("On night?", 0, NUM_DAYS);

    printRolesInScript();
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
        //Loop through entered three for three roles
        for (int j = 0; j < n; j++)
        {
            if (roleIDs[j] == i)
            {
                inRole = 1;
            }
        }
        //Roles someone might be that would cause them to lie about their three for three
        if ((strcmp(ROLE_NAMES[i], "POLITICIAN") == 0) || (strcmp(ROLE_NAMES[i], "DRUNK") == 0) || (strcmp(ROLE_NAMES[i], "MUTANT") == 0) || (strcmp(ROLE_TEAMS[i], "EVIL") == 0)) //If i is on evil team or Drunk, or Mutant
        { //People as these roles could always give a false 3 for 3
            inRole = 1;
        }

        if (inRole == 1)
        { 
        }
        else
        { //Player can only be their role or drunk
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            addKnowledgeName(kb, "PLAYERS", playerID, buff);
        }
    }
}

/**
 * poisoned() - assume that a player is poisoned
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void poisoned(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int playerID;
    int night;

    printf("ENERTING: PLAYER POISONED\n");

    playerID = getPlayerIDInput(kb, "Which player is poisoned?"); // Read player ID

    night = getInt("On night?", 0, NUM_DAYS);

    snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
    addKnowledgeName(kb, "PLAYERS", playerID, buff);
}

/**
 * notPoisoned() - assume that a player is poisoned
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void notPoisoned(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int playerID;
    int night;

    printf("ENERTING: PLAYER POISONED\n");

    playerID = getPlayerIDInput(kb, "Which player is not poisoned?"); // Read player ID

    night = getInt("On night?", 0, NUM_DAYS);

    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addKnowledgeName(kb, "PLAYERS", playerID, buff);
}

/**
 * redHerring() - assume that a player is a red herring
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void redHerring(KnowledgeBase* kb)
{
    int playerID;

    printf("ENERTING: PLAYER RED HERRING\n");

    playerID = getPlayerIDInput(kb, "Which player is the red herring?"); // Read player ID

    addKnowledgeName(kb, "PLAYERS", playerID, "is_REDHERRING");
}

/**
 * diedInNight() - assume that a player died in the night
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void diedInNight(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int night;

    printf("ENERTING: PLAYER(s) DIED IN NIGHT\n");

    int n;
    n = getInt("Num Deaths?", 0, 17);

    night = getInt("On night?", 0, NUM_DAYS);

    int playerID[n];

    for (int i = 0; i < n; i++)
    {
        playerID[i] = getPlayerIDInput(kb, "Which player(s) died in the night?"); // Read player ID
    }

    for (int i = 0; i < kb->SET_SIZES[0]; i++)
    {
        int inSet = 0;
        for (int j = 0; j < n; j++)
        {
            if (playerID[j] == i) inSet = 1;
        }
        if (inSet)
        {
            snprintf(buff, STRING_BUFF_SIZE, "SLEEP_DEATH_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
        else
        {
            snprintf(buff, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
    }
}

/**
 * hung() - assume that a player was hung
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void hung(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int night;

    printf("ENERTING: PLAYER(s) HUNG\n");

    int n;
    n = getInt("Num Deaths?", 0, 17);

    night = getInt("On night?", 0, NUM_DAYS);

    int playerID[n];

    for (int i = 0; i < n; i++)
    {
        playerID[i] = getPlayerIDInput(kb, "Which player(s) were hung?"); // Read player ID
    }

    for (int i = 0; i < kb->SET_SIZES[0]; i++)
    {
        int inSet = 0;
        for (int j = 0; j < n; j++)
        {
            if (playerID[j] == i) inSet = 1;
        }
        if (inSet)
        {
            snprintf(buff, STRING_BUFF_SIZE, "HANGING_DEATH_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
        else
        {
            snprintf(buff, STRING_BUFF_SIZE, "NOT_HANGING_DEATH_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
    }
}

/**
 * nominationDeath() - assume that a player died instantly when nominating another player
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void nominationDeath(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int night;

    printf("ENERTING: PLAYER(s) NOMINATION DEATH\n");

    int n;
    n = getInt("Num Deaths?", 0, 17);

    night = getInt("On night?", 0, NUM_DAYS);

    int playerID[n];

    for (int i = 0; i < n; i++)
    {
        playerID[i] = getPlayerIDInput(kb, "Which player(s) died when nominating someone?"); // Read player ID
    }

    for (int i = 0; i < kb->SET_SIZES[0]; i++)
    {
        int inSet = 0;
        for (int j = 0; j < n; j++)
        {
            if (playerID[j] == i) inSet = 1;
        }
        if (inSet)
        {
            snprintf(buff, STRING_BUFF_SIZE, "NOMINATION_DEATH_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
        else
        {
            snprintf(buff, STRING_BUFF_SIZE, "NOT_NOMINATION_DEATH_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
    }
}

/**
 * nominationDeath() - assume that a player was ressurrected
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void resurrected(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int night;

    printf("ENERTING: PLAYER(s) RESURRECTED\n");

    int n;
    n = getInt("Num Resurrections?", 0, 17);

    night = getInt("On night?", 0, NUM_DAYS);

    int playerID[n];

    for (int i = 0; i < n; i++)
    {
        playerID[i] = getPlayerIDInput(kb, "Which player(s) were resurrected?"); // Read player ID
    }

    for (int i = 0; i < kb->SET_SIZES[0]; i++)
    {
        int inSet = 0;
        for (int j = 0; j < n; j++)
        {
            if (playerID[j] == i) inSet = 1;
        }
        if (inSet)
        {
            snprintf(buff, STRING_BUFF_SIZE, "RESURRECTED_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
        else
        {
            snprintf(buff, STRING_BUFF_SIZE, "NOT_RESURRECTED_[NIGHT%d]", night);
            addKnowledgeName(kb, "PLAYERS", i, buff);
        }
    }
}

/**
 * nominationDeath() - reset a player's entire knowledge to "Unknown"
 * X(P) = FALSE and NOT_X(P) = FALSE FOR ALL X FOR SOME player P
 * 
 * @kb the knowledge base to update
*/
static void reset(KnowledgeBase* kb)
{
    int playerID;

    printf("ENERTING: PLAYER RESET\n");

    playerID = getPlayerIDInput(kb, "Reset player?"); // Read player ID

    resetElement(kb, 0, playerID);
}

/**
 * resetMetaData() - reset the metadata's entire knowledge to "Unknown"
 * X(METADATA) = FALSE and NOT_X(METADATA) = FALSE FOR ALL X FOR METADATA
 * 
 * @kb the knowledge base to update
*/
static void resetMetaData(KnowledgeBase* kb)
{
    resetElement(kb, 2, 0);
}

/**
 * washerWomanPing() - runs a ping
 * Start knowing one of two players is a particular townsfolk
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void washerWomanPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int selectedRole = -1;
    int playerX = -1;
    int playerY = -1;

    selectedRole = getRoleIDInput("Role Shown?");

    playerX = getPlayerIDInput(kb, "For player 1?"); // Read player ID 

    playerY = getPlayerIDInput(kb, "For player 2?"); // Read player ID 

    //Originally this but converted to the stronger argument
    //If <PLAYER_INFO>is_WASHERWOMAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
    //It was converted to this as <METADATA>is_NOT_SPY_in_PLAY ---> <PLAYER_X>is_NOT_SPY
    //If <PLAYER_INFO>is_WASHERWOMAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_SPY AND <PLAYER_Y>is_NOT_SPY => <METADATA>is_<ROLE>_in_PLAY
    setTempRuleParams(rs, 3,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT0]", ROLE_NAMES[selectedRole]);
    setTempRuleResultName(rs, kb, 0, "METADATA", buff);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_WASHERWOMAN_[NIGHT0]", playerIDinfoFrom);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerX);
    addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerY);
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
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT0]", ROLE_NAMES[selectedRole]);
            setTempRuleResultName(rs, kb, -i-1000, "PLAYERS", buff); //iterate over players not X or Y
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_WASHERWOMAN_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerX);
            addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerY);
            pushTempRule(rs);
        }
    }
}

/**
 * librarianPing() - runs a ping
 * Start knowing one of two players is a particular outsider
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void librarianPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int selectedRole = -1;
    int playerX = -1;
    int playerY = -1;

    selectedRole = getRoleIDInput("Role Shown?");

    playerX = getPlayerIDInput(kb, "For player 1?"); // Read player ID 

    playerY = getPlayerIDInput(kb, "For player 2?"); // Read player ID 

    //Originally this but converted to the stronger argument
    //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
    //It was converted to this as <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_X>is_NOT_SPY
    //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_SPY AND <PLAYER_Y>is_NOT_SPY => <METADATA>is_<ROLE>_in_PLAY
    setTempRuleParams(rs, 3,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT0]", ROLE_NAMES[selectedRole]);
    setTempRuleResultName(rs, kb, 0, "METADATA", buff);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_LIBRARIAN_[NIGHT0]", playerIDinfoFrom);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerX);
    addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerY);
    pushTempRule(rs);

    //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_SPY AND <PLAYER_Y>is_NOT_SPY=> <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]
    for (int player = 0; player < kb->SET_SIZES[0]; player++)
    {
        if (player == playerX || player == playerY)
        {

        }
        else
        {
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT0]", ROLE_NAMES[selectedRole]);
            setTempRuleResultName(rs, kb, -player-1000, "PLAYERS", buff); //iterate over players not X or Y
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_LIBRARIAN_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerX);
            addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerY);
            pushTempRule(rs);
        }
    }
}

/**
 * investigatorPing() - runs a ping
 * Start knowing one of two players is a particular minion
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void investigatorPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int selectedRole = -1;
    int playerX = -1;
    int playerY = -1;

    selectedRole = getRoleIDInput("Role Shown?");

    playerX = getPlayerIDInput(kb, "For player 1?"); // Read player ID 

    playerY = getPlayerIDInput(kb, "For player 2?"); // Read player ID 

    //Originally this but converted to the stronger argument
    //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_RECLUSE_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
    //It was converted to this as <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_X>is_NOT_SPY
    //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_Y>is_NOT_RECLUSE => <METADATA>is_<ROLE>_in_PLAY
    setTempRuleParams(rs, 3,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY_[NIGHT0]", ROLE_NAMES[selectedRole]);
    setTempRuleResultName(rs, kb, 0, "METADATA", buff);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_INVESTIGATOR_[NIGHT0]", playerIDinfoFrom);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerX);
    addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerY);
    pushTempRule(rs);

    //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_Y>is_NOT_RECLUSE=> <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]
    for (int player = 0; player < kb->SET_SIZES[0]; player++)
    {
        if (player == playerX || player == playerY)
        {

        }
        else
        {
            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT0]", ROLE_NAMES[selectedRole]);
            setTempRuleResultName(rs, kb, -player-1000, "PLAYERS", buff); //iterate over players not X or Y
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_INVESTIGATOR_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerX);
            addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerY);
            pushTempRule(rs);
        }
    }
}

/**
 * chefPing() - runs a ping
 * Start knowing how many pairs of evil players there are
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void chefPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    int countPairs = -1;
    printf("What was the number of pairs?:\n");
    scanf("%d", &countPairs); // Read player ID


    //0 - implies two adjancent players cant be bad 
    if (countPairs == 0)
    {
        for (int player = 0; player < kb->SET_SIZES[0]; player++)
        {
            int playerLHS = player - 1;
            int playerRHS = player + 1;
            if (playerLHS < 0)
            {
                playerLHS += kb->SET_SIZES[0];
            }
            if (playerRHS >= kb->SET_SIZES[0])
            {
                playerRHS -= kb->SET_SIZES[0];
            }
            //If player is evil, then player LHS cannot be evil (assuming ping is true)
            setTempRuleParams(rs, 2,0);
            setTempRuleResultName(rs, kb, -playerLHS-1000, "PLAYERS", "is_GOOD_[NIGHT0]");
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_EVIL_[NIGHT0]", player);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_CHEF_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
            //Commented out as if they are a reclcuse they are still good
            //addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerLHS);
            pushTempRule(rs);
            //If player is evil, then player RHS cannot be evil
            setTempRuleParams(rs, 2,0);
            setTempRuleResultName(rs, kb, -playerRHS-1000, "PLAYERS", "is_GOOD_[NIGHT0]");
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_EVIL_[NIGHT0]", player);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_CHEF_[NIGHT0]", playerIDinfoFrom);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
            //Commented out as if they are a reclcuse they are still good
            //addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerRHS);
            pushTempRule(rs);

        }
    }
}

/**
 * empathPing() - runs a ping
 * Each night learn how many of your alive neighbours are evil
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void empathPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int countEvil = -1;
    int playerX;
    int playerY;
    int night = -1;

    countEvil = getInt("What was ping? (0, 1, 2 evil)", 0, 3);

    night = getInt("On night?", 0, NUM_DAYS);

    //Find 2 adjacent alive players
    playerX = playerIDinfoFrom;
    playerY = playerIDinfoFrom;

    int isDead = 0;
    do
    {
        playerX--;
        if (playerX < 0)
        {
            playerX = kb->SET_SIZES[0]-1;
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
        isDead = isKnownName(kb, "PLAYERS", playerX, buff);
    } while (isDead == 1);
    isDead = 0;
    do
    {
        playerY++;
        if (playerY > kb->SET_SIZES[0]-1)
        {
            playerY = 0;
        }
        snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
        isDead = isKnownName(kb, "PLAYERS", playerY, buff);
    } while (isDead == 1);

    printf("Empath ping on %s and %s\n", kb->ELEMENT_NAMES[0][playerX], kb->ELEMENT_NAMES[0][playerY]);
    
    
    if (countEvil == 0)
    {
        //For 0 evil 2 good 
        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
        pushTempRule(rs);

        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerY);
        pushTempRule(rs);
    }
    else if (countEvil == 1)
    {
        //For 1 evil 1 good 
        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerY);
        pushTempRule(rs);

        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerY);
        pushTempRule(rs);

        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerY);
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerX);
        pushTempRule(rs);

        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerY);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerX);
        pushTempRule(rs);
    }
    else if (countEvil == 2)
    {
        //For 2 evil 0 good 
        //If <PLAYER_INFO>is_EMPATH AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_X & Y>is_EVIL
        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
        pushTempRule(rs);

        setTempRuleParams(rs, 2,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerY);
        pushTempRule(rs);
    }
}

/**
 * fortuneTellerPing() - runs a ping
 * Each night, choose 2 players: you learn if either is a Demon. There is 1 good player that registers falsely to you.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void fortuneTellerPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int count = -1;
    int playerX;
    int playerY;
    int night = -1;

    count = getYNInput("What was ping?");

    playerX = getPlayerIDInput(kb, "For player 1?"); // Read player ID 

    playerY = getPlayerIDInput(kb, "For player 2?"); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    if (count == 0)
    {
        //If 0-NO Ping
        //If <PLAYER_INFO>is_FORTUNE_TELLER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_X>is_NOT_DEMON
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -playerX-1000, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        pushTempRule(rs);

        //If <PLAYER_INFO>is_FORTUNE_TELLER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_X>is_NOT_REDHERRING
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_REDHERRING_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -playerX-1000, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        pushTempRule(rs);

        //If <PLAYER_INFO>is_FORTUNE_TELLER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_Y>is_NOT_DEMON
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -playerY-1000, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        pushTempRule(rs);

        //If <PLAYER_INFO>is_FORTUNE_TELLER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> => <PLAYER_Y>is_NOT_REDHERRING
        setTempRuleParams(rs, 1,0);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_REDHERRING_[NIGHT%d]", night);
        setTempRuleResultName(rs, kb, -playerY-1000, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_FORTUNE_TELLER_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
        addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
        pushTempRule(rs);
    }
    else
    {
        //If 1-YES Ping

        //Remove one of is_NOT_DEMON, is_NOT_SCARLET_WOMAN, is_NOT_REDHERRING, is_NOT_RECLUSE for the result

        //If <PLAYER_INFO>is_FORTUNE_TELLER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> 
        //AND <PLAYER_X>is_NOT_DEMON AND <PLAYER_X>is_NOT_SCARLET_WOMAN AND <PLAYER_X>is_NOT_REDHERRING AND <PLAYER_X>is_NOT_RECLUSE 
        //AND <PLAYER_Y>is_NOT_DEMON AND <PLAYER_Y>is_NOT_SCARLET_WOMAN AND <PLAYER_Y>is_NOT_REDHERRING AND <PLAYER_Y>is_NOT_RECLUSE 
        //  => <PLAYER_X>is_REDHERRING

        for (int xy = 0; xy < 2; xy++)
        {
            for (int impliedRole = 0; impliedRole < 4; impliedRole++)
            {
                setTempRuleParams(rs, 3, 0);

                if (impliedRole == 0) snprintf(buff, STRING_BUFF_SIZE, "is_SCARLET_WOMAN_[NIGHT%d]", night);
                if (impliedRole == 1) snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
                if (impliedRole == 2) snprintf(buff, STRING_BUFF_SIZE, "is_REDHERRING");
                if (impliedRole == 3) snprintf(buff, STRING_BUFF_SIZE, "is_RECLUSE_[NIGHT%d]", night);

                if (xy == 0)
                {
                    setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
                }
                else
                {
                    setTempRuleResultName(rs, kb, 2, "PLAYERS", buff);
                }

                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_FORTUNE_TELLER", playerIDinfoFrom);
                snprintf(buff, STRING_BUFF_SIZE, "is_NOT_poisoned_NIGHT%d", night);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
                //Scarlet Woman
                snprintf(buff, STRING_BUFF_SIZE, "is_SCARLET_WOMAN_[NIGHT%d]", night);
                if (xy != 0 || impliedRole != 0) addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
                if (xy != 1 || impliedRole != 0) addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerY);
                //Demon
                snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
                if (xy != 0 || impliedRole != 1) addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
                if (xy != 1 || impliedRole != 1) addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerY);
                //Redherring
                snprintf(buff, STRING_BUFF_SIZE, "is_REDHERRING");
                if (xy != 0 || impliedRole != 2) addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
                if (xy != 1 || impliedRole != 2) addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerY);
                //Recluse
                snprintf(buff, STRING_BUFF_SIZE, "is_RECLUSE_[NIGHT%d]", night);
                if (xy != 0 || impliedRole != 3) addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
                if (xy != 1 || impliedRole != 3) addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", buff, playerY);
                pushTempRule(rs);
            }
        }

    }
}

/**
 * undertakerPing() - runs a ping
 * Each night*, you learn which character died by execution today.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void undertakerPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int selectedRole = -1;
    int playerX;
    int night = -1;

    selectedRole = getRoleIDInput("Role Shown?");

    playerX = getPlayerIDInput(kb, "For player?"); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    //If <PLAYER_INFO>is_UNDERTAKER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_X>is_NOT_SPY => <PLAYER_X>is_<ROLE>
    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[selectedRole], night);
    setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
    snprintf(buff, STRING_BUFF_SIZE, "is_UNDERTAKER_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    pushTempRule(rs);

}

/**
 * monkPing() - runs a ping
 * 
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void monkPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int selectedRole = -1;
    int playerX;
    int night = -1;


    playerX = getPlayerIDInput(kb, "Protected player?"); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    setTempRuleParams(rs, 1,0);
    snprintf(buff, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, playerX-1000, "PLAYERS", buff);

    snprintf(buff, STRING_BUFF_SIZE, "is_MONK_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);

    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MONK_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, 0, "PLAYERS", buff);

    snprintf(buff, STRING_BUFF_SIZE, "SLEEP_DEATH_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);
}

/**
 * ravenkeeperPing() - runs a ping
 * If you die at night, you are woken to choose a player: you learn their character.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void ravenkeeperPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int selectedRole = -1;
    int playerX;
    int night = -1;

    selectedRole = getRoleIDInput("Role Shown?");

    playerX = getPlayerIDInput(kb, "For player?"); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    //If <PLAYER_INFO>is_RAVENKEEPER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> AND <PLAYER_X>is_NOT_RECLUSE AND <PLAYER_X>is_NOT_SPY => <PLAYER_X>is_<ROLE>
    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[selectedRole], night);
    setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
    snprintf(buff, STRING_BUFF_SIZE, "is_RAVENKEEPER_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    pushTempRule(rs);
}

/**
 * clockmakerPing() - runs a ping
 * You start knowing how many steps from the Demon to nearest Minion.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void clockmakerPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    int numSteps = -1;

    const int MAX_STEPS = kb->SET_SIZES[0]/2; //was (kb->SET_SIZES[0]/2)-1 but we add +1 at getInt anyways as non inclusive

    numSteps = getInt("numSteps?", 0, MAX_STEPS);
    for (int demonPlayer = 0; demonPlayer < kb->SET_SIZES[0]; demonPlayer++)
    {
        int playerLHS = demonPlayer;
        int playerRHS = demonPlayer;
        
        for (int step = 0; step <= numSteps; step++)
        { // For all previous steps unless there is a spy, no one is the minion
            playerLHS--;
            playerRHS++;
            if (playerLHS < 0)
            {
                playerLHS += kb->SET_SIZES[0];
            }
            if (playerRHS >= kb->SET_SIZES[0])
            {
                playerRHS -= kb->SET_SIZES[0];
            }
            if (step < numSteps)
            {
                setTempRuleParams(rs, 3,0);
                setTempRuleResultName(rs, kb, 1, "PLAYERS", "is_NOT_MINION_[NIGHT0]");
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_CLOCKMAKER_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerLHS);
                addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_DEMON_[NIGHT0]", demonPlayer);
                pushTempRule(rs);

                setTempRuleParams(rs, 3,0);
                setTempRuleResultName(rs, kb, 1, "PLAYERS", "is_NOT_MINION_[NIGHT0]");
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_CLOCKMAKER_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_SPY_[NIGHT0]", playerRHS);
                addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_DEMON_[NIGHT0]", demonPlayer);
                pushTempRule(rs);
            }
            else
            {
                setTempRuleParams(rs, 4,0);
                setTempRuleResultName(rs, kb, 1, "PLAYERS", "is_MINION_[NIGHT0]");
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_CLOCKMAKER_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerLHS);
                addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_DEMON_[NIGHT0]", demonPlayer);
                addFixedConditionToTempRuleName(rs,kb, 3, "PLAYERS", "is_NOT_MINION_[NIGHT0]", playerRHS);
                addFixedConditionToTempRuleName(rs,kb, 3, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerRHS);
                pushTempRule(rs);

                setTempRuleParams(rs, 4,0);
                setTempRuleResultName(rs, kb, 1, "PLAYERS", "is_MINION_[NIGHT0]");
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_CLOCKMAKER_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", "is_NOT_POISONED_[NIGHT0]", playerIDinfoFrom);
                addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerRHS);
                addFixedConditionToTempRuleName(rs,kb, 2, "PLAYERS", "is_DEMON_[NIGHT0]", demonPlayer);
                addFixedConditionToTempRuleName(rs,kb, 3, "PLAYERS", "is_NOT_MINION_[NIGHT0]", playerLHS);
                addFixedConditionToTempRuleName(rs,kb, 3, "PLAYERS", "is_NOT_RECLUSE_[NIGHT0]", playerLHS);
                pushTempRule(rs);
            }
        }
    }
}


/**
 * dreamerPing() - runs a ping
 * 
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void dreamerPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int shownRole1 = -1;
    int shownRole2 = -1;
    int playerX;
    int night = -1;

    playerX = getPlayerIDInput(kb, "Chosen player?"); // Read player ID 

    shownRole1 = getRoleIDInput("Good Role Shown?");
    shownRole2 = getRoleIDInput("Evil Role Shown?");

    night = getInt("On night?", 0, NUM_DAYS);

    //Player must be one of those two roles
    setTempRuleParams(rs, 2,0);
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        if ((strcmp(ROLE_NAMES[i], ROLE_NAMES[shownRole1]) == 0) || (strcmp(ROLE_NAMES[i], ROLE_NAMES[shownRole2]) == 0))
        { 
        }
        else
        { //Player can only be their role or drunk
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[i], night);
            setTempRuleResultName(rs, kb, -playerX-1000, "PLAYERS", buff);
        }
    }
    snprintf(buff, STRING_BUFF_SIZE, "is_DREAMER_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);
}

/**
 * snakeCharmerPing() - runs a ping
 * Each night, choose an alive player: a chosen Demon swaps characters & alignments with you & is then poisoned.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void snakeCharmerPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * mathematicianPing() - runs a ping
 * Mathematician Each night, you learn how many players' abilities worked abnormally (since dawn) due to another character's ability.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void mathematicianPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int numAbilitiesWorkedAbnormally = -1;
    int night = -1;

    numAbilitiesWorkedAbnormally = getInt("How many abilities worked abnormally?", 0, kb->SET_SIZES[0]+1);

    night = getInt("On night?", 0, NUM_DAYS);
    

    //Player must be one of those two roles
    setTempRuleParams(rs, numAbilitiesWorkedAbnormally+1,1);

    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, -1, "PLAYERS", buff);

    for (int i = 0; i < numAbilitiesWorkedAbnormally; i++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, i, "PLAYERS", buff);
    }
    snprintf(buff, STRING_BUFF_SIZE, "is_MATHEMATICIAN_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, numAbilitiesWorkedAbnormally, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, numAbilitiesWorkedAbnormally, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);
}

/**
 * flowerGirlPing() - runs a ping
 * Each night*, you learn if the Demon voted today.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void flowerGirlPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int n;
    int night = -1;
    int didDemonVote = -1;

    n = getInt("How many players voted?", 0, kb->SET_SIZES[0]); // Read player ID 
    
    int playerVotedIDs[n];


    night = getInt("On night?", 0, NUM_DAYS);

    didDemonVote = getInt("Did the demon vote (0 - NO, 1 - YES)?", 0, 2);


    for (int j = 0; j < n; j++)
    {
        playerVotedIDs[j] = -1;
    }
    for (int j = 0; j < n; j++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "Player %d?", j);
        playerVotedIDs[j] = getPlayerIDInput(kb, buff);
    }

    for (int player = 0; player < kb->SET_SIZES[0]; player++)
    {
        int voted = 0;
        //Loop through to check if player voted
        for (int j = 0; j < n; j++)
        {
            if (playerVotedIDs[j] == player)
            {
                voted = 1;
            }
        }
        if (voted != didDemonVote)
        { //Player cannot be the demon as they voted different to the demon

            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_FLOWERGIRL_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, player);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, player);
            pushTempRule(rs);
        }
    }
}

/**
 * townCrierPing() - runs a ping
 * Each night*, you learn if a Minion nominated today.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void townCrierPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int n;
    int night = -1;
    int didMinionNominate = -1;

    n = getInt("How many players nominated?", 0, kb->SET_SIZES[0]); // Read player ID 
    
    int playerVotedIDs[n];


    night = getInt("On night?", 0, NUM_DAYS);

    didMinionNominate = getInt("Did the minion nominate (0 - NO, 1 - YES)?", 0, 2);


    for (int j = 0; j < n; j++)
    {
        playerVotedIDs[j] = -1;
    }
    for (int j = 0; j < n; j++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "Player %d?", j);
        playerVotedIDs[j] = getPlayerIDInput(kb, buff);
    }

    for (int player = 0; player < kb->SET_SIZES[0]; player++)
    {
        int voted = 0;
        //Loop through to check if player voted
        for (int j = 0; j < n; j++)
        {
            if (playerVotedIDs[j] == player)
            {
                voted = 1;
            }
        }
        if (voted == 1 && didMinionNominate == 0)
        { //Player cannot be the minion as no minions voted

            setTempRuleParams(rs, 2,0);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_MINION_[NIGHT%d]", night);
            setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
            snprintf(buff, STRING_BUFF_SIZE, "is_TOWN_CRIER_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, player);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, player);
            pushTempRule(rs);
        }
    }
}

/**
 * oraclePing() - runs a ping
 * Each night*, you learn how many dead players are evil.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void oraclePing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int numEvil;
    int numGood;
    int night = -1;

    numEvil = getInt("How many players evil?", 0, kb->SET_SIZES[0]); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    int numDead = 0;

    for (int player = 0; player < kb->SET_SIZES[0]; player++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
        if (isKnownName(kb, "PLAYERS", player, buff) == 1) numDead++;
    }

    setTempRuleParams(rs, numEvil+2,1);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_EVIL_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, numEvil, "PLAYERS", buff);
    for (int i = 0; i < numEvil; i++)
    {
        snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, i, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, i, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, i, "PLAYERS", buff);
        snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
        addConditionToTempRuleName(rs,kb, i, "PLAYERS", buff);
    }
    snprintf(buff, STRING_BUFF_SIZE, "is_DEAD_[NIGHT%d]", night);
    addConditionToTempRuleName(rs,kb, numEvil, "PLAYERS", buff);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
    addConditionToTempRuleName(rs,kb, numEvil, "PLAYERS", buff);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
    addConditionToTempRuleName(rs,kb, numEvil, "PLAYERS", buff);
 
    snprintf(buff, STRING_BUFF_SIZE, "is_ORACLE_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, numEvil+1, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, numEvil+1, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);
}

/**
 * savantPing() - runs a ping
 * Each day, you may visit the Storyteller to learn 2 things in private: 1 is true & 1 is false.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void savantPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * seamstressPing() - runs a ping
 * Once per game, at night, choose 2 players (not yourself: you learn if they are the same alignment.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void seamstressPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    
    int playerX;
    int playerY;
    int night = -1;

    int sameAlignment = -1;

    playerX = getPlayerIDInput(kb, "Chosen player 1?"); // Read player ID 
    playerY = getPlayerIDInput(kb, "Chosen player 1?"); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    sameAlignment = getInt("Same alignment? (1: Yes 0: No)", 0, 2);

    for (int align = 0; align <= 1; align++)
    {
        for (int lhsPlayer = 0; lhsPlayer <= 1; lhsPlayer++)
        {
            setTempRuleParams(rs, 2,0);
            if (align == 0) snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
            else snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);
            
            if (lhsPlayer == 0) setTempRuleResultName(rs, kb, -playerX-1000, "PLAYERS", buff);
            else setTempRuleResultName(rs, kb, -playerY-1000, "PLAYERS", buff);

            int lhsAlign = (align + sameAlignment + 1) % 2; 

            if (lhsAlign == 0) snprintf(buff, STRING_BUFF_SIZE, "is_EVIL_[NIGHT%d]", night);
            else snprintf(buff, STRING_BUFF_SIZE, "is_GOOD_[NIGHT%d]", night);

            if (lhsPlayer == 0) addFixedConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff, playerY);
            else addFixedConditionToTempRuleName(rs, kb, 0, "PLAYERS", buff, playerX);
            
            snprintf(buff, STRING_BUFF_SIZE, "is_SEAMSTRESS_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerIDinfoFrom);
            snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
            addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerIDinfoFrom);
            pushTempRule(rs);
        }
    }
}

/**
 * philosopherPing() - runs a ping
 * 
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void philosopherPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * artistPing() - runs a ping
 * Once per game, during the day, privately ask the Storyteller any yes/ no question
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void artistPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * jugglerPing() - runs a ping
 * 
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void jugglerPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * sagePing() - runs a ping
 * If the Demon kills you, you learn that it is 1 of 2 players.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void sagePing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    
    int playerX;
    int playerY;
    int night = -1;

    playerX = getPlayerIDInput(kb, "Potential demon 1?"); // Read player ID 
    playerY = getPlayerIDInput(kb, "Potential demon 2?"); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, -playerX-1000, "PLAYERS", buff);

    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerY);

    snprintf(buff, STRING_BUFF_SIZE, "is_SAGE_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);

    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_DEMON_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, -playerY-1000, "PLAYERS", buff);

    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_DEMON_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);

    snprintf(buff, STRING_BUFF_SIZE, "is_SAGE_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);
}

/**
 * grandMotherPing() - runs a ping
 * You start knowing a good player & their character. If the Demon kills them, you die too
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void grandMotherPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    
    int playerX;
    int playerRole;
    int night = -1;

    playerX = getPlayerIDInput(kb, "Known Good Player?"); // Read player ID 
    playerRole = getRoleIDInput("Known Role?");

    night = 0; //First night

    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_%s_[NIGHT%d]", ROLE_NAMES[playerRole], night);
    setTempRuleResultName(rs, kb, 1, "PLAYERS", buff);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_RECLUSE_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_SPY_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    snprintf(buff, STRING_BUFF_SIZE, "is_GRANDMOTHER_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);
}

/**
 * chamberMaidPing() - runs a ping
 * Each night, choose 2 alive players (not yourself: you learn how many woke tonight due to their ability.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void chamberMaidPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * exorcistPing() - runs a ping
 * Each night*, choose a player (different to last night): the Demon, if chosen, learns who you are & doesn't wake tonight.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void exorcistPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * innkeeperPing() - runs a ping
 * Each night*, choose 2 players: they can't die tonight, but 1 is drunk until dusk.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void innkeeperPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    
    int playerX;
    int playerY;
    int night = -1;

    playerX = getPlayerIDInput(kb, "Chosen player 1?"); // Read player ID 
    playerY = getPlayerIDInput(kb, "Chosen player 1?"); // Read player ID 

    night = getInt("On night?", 0, NUM_DAYS);

    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, -playerY-1000, "PLAYERS", buff);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerX);
    snprintf(buff, STRING_BUFF_SIZE, "is_INNKEEPER_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);

    setTempRuleParams(rs, 2,0);
    snprintf(buff, STRING_BUFF_SIZE, "is_POISONED_[NIGHT%d]", night);
    setTempRuleResultName(rs, kb, -playerX-1000, "PLAYERS", buff);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 1, "PLAYERS", buff, playerY);
    snprintf(buff, STRING_BUFF_SIZE, "is_INNKEEPER_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    snprintf(buff, STRING_BUFF_SIZE, "is_NOT_POISONED_[NIGHT%d]", night);
    addFixedConditionToTempRuleName(rs,kb, 0, "PLAYERS", buff, playerIDinfoFrom);
    pushTempRule(rs);
}

/**
 * gamblerPing() - runs a ping
 * Each night*, choose a player & guess their character: if you guess wrong, you die.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void gamblerPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * gossipPing() - runs a ping
 * 
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void gossipPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * professorPing() - runs a ping
 * Once per game, at night*, choose a dead player: if they are a Townsfolk, they are resurrected.
 * 
 * @playerIDinfoFrom the playerID/index the ping is from
 * @kb the knowledge base to update
 * @rs the rulset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void professorPing(int playerIDinfoFrom, KnowledgeBase* kb, RuleSet* rs)
{
    //char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    printf("NOT SUPPORTED!\n");
}

/**
 * addPingRule() - add a player ping to the game
 * ASSUME NOTHING other than if that player actually is who they say they are 
 * they were telling the truth about their ping 
 * 
 * @kb the knowledge base to update
 * @rs the ruleset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
static void addPingRule(KnowledgeBase* kb, RuleSet* rs)
{
    char inputPingType[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    //TB
    char WASHERWOMAN_PING[] = "WASHERWOMAN";
    char LIBRARIAN_PING[] = "LIBRARIAN";
    char INVESTIGATOR_PING[] = "INVESTIGATOR";
    char CHEF_PING[] = "CHEF";
    char EMPATH_PING[] = "EMPATH";
    char FORTUNE_TELLER_PING[] = "FORTUNE_TELLER";
    char UNDERTAKER_PING[] = "UNDERTAKER";
    char MONK_PING[] = "MONK"; 
    char RAVENKEEPER_PING[] = "RAVENKEEPER";

    //SV
    char CLOCKMAKER_PING[] = "CLOCKMAKER";
    char DREAMER_PING[] = "DREAMER";
    char SNAKE_CHARMER_PING[] = "SNAKE_CHARMER";
    char MATHEMATICIAN_PING[] = "MATHEMATICIAN";
    char FLOWERGIRL_PING[] = "FLOWERGIRL";
    char TOWN_CRIER_PING[] = "TOWN_CRIER";
    char ORACLE_PING[] = "ORACLE";
    char SAVANT_PING[] = "SAVANT";
    char SEAMSTRESS_PING[] = "SEAMSTRESS";
    char PHILOSOPHER_PING[] = "PHILOSOPHER";
    char ARTIST_PING[] = "ARTIST";
    char JUGGLER_PING[] = "JUGGLER";
    char SAGE_PING[] = "SAGE";

    //BMR
    char GRANDMOTHER_PING[] = "GRANDMOTHER";
    char CHAMBERMAID_PING[] = "CHAMBERMAID";
    char EXORCIST_PING[] = "EXORCIST";
    char INNKEEPER_PING[] = "INNKEEPER";
    char GAMBLER_PING[] = "GAMBLER";
    char GOSSIP_PING[] = "GOSSIP";
    //char COURTIER_PING[] = "COURTIER";
    char PROFESSOR_PING[] = "PROFESSOR";
    //char MINSTREL_PING[] = "MINSTREL";


    int loop = 1;

    int playerIDinfoFrom;

    printf("ENERTING: PLAYER PING (Unreliable information)\n");

    playerIDinfoFrom = getPlayerIDInput(kb, "Info from player?"); // Read player ID 

    printf("Ping Type?:\n");
    printf("- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n", WASHERWOMAN_PING, LIBRARIAN_PING, INVESTIGATOR_PING, CHEF_PING, EMPATH_PING, FORTUNE_TELLER_PING, UNDERTAKER_PING, MONK_PING, RAVENKEEPER_PING);
    printf("- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n", CLOCKMAKER_PING, DREAMER_PING, SNAKE_CHARMER_PING, MATHEMATICIAN_PING, FLOWERGIRL_PING, TOWN_CRIER_PING, ORACLE_PING, SAVANT_PING, SEAMSTRESS_PING, PHILOSOPHER_PING, ARTIST_PING, JUGGLER_PING, SAGE_PING);
    printf("- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n- %s\n", GRANDMOTHER_PING, CHAMBERMAID_PING, EXORCIST_PING, INNKEEPER_PING, GAMBLER_PING, GOSSIP_PING, PROFESSOR_PING);
    while (loop)
    {
        scanf("%255s", inputPingType); // Read a string (up to 99 characters to leave space for the null terminator)
        loop = 0;
        //TB
        if (strcasecmp(inputPingType,WASHERWOMAN_PING) == 0) washerWomanPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,LIBRARIAN_PING) == 0) librarianPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,INVESTIGATOR_PING) == 0) investigatorPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,CHEF_PING) == 0) chefPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,EMPATH_PING) == 0) empathPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,FORTUNE_TELLER_PING) == 0) fortuneTellerPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,UNDERTAKER_PING) == 0) undertakerPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,MONK_PING) == 0) monkPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,RAVENKEEPER_PING) == 0) ravenkeeperPing(playerIDinfoFrom, kb, rs);
        //SV
        else if (strcasecmp(inputPingType,CLOCKMAKER_PING) == 0) clockmakerPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,DREAMER_PING) == 0) dreamerPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,SNAKE_CHARMER_PING) == 0) snakeCharmerPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,MATHEMATICIAN_PING) == 0) mathematicianPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,FLOWERGIRL_PING) == 0) flowerGirlPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,TOWN_CRIER_PING) == 0) townCrierPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,ORACLE_PING) == 0) oraclePing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,SAVANT_PING) == 0) savantPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,SEAMSTRESS_PING) == 0) seamstressPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,PHILOSOPHER_PING) == 0) philosopherPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,ARTIST_PING) == 0) artistPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,JUGGLER_PING) == 0) jugglerPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,SAGE_PING) == 0) sagePing(playerIDinfoFrom, kb, rs);
        //BMR
        else if (strcasecmp(inputPingType,GRANDMOTHER_PING) == 0) grandMotherPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,CHAMBERMAID_PING) == 0) chamberMaidPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,EXORCIST_PING) == 0) exorcistPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,INNKEEPER_PING) == 0) innkeeperPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,GAMBLER_PING) == 0) gamblerPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,GOSSIP_PING) == 0) gossipPing(playerIDinfoFrom, kb, rs);
        else if (strcasecmp(inputPingType,PROFESSOR_PING) == 0) professorPing(playerIDinfoFrom, kb, rs);
        else 
        {
            printf("ERROR: Invalid string!\n");
            loop = 1;
        }
    }
}

static void printTodoList(KnowledgeBase* kb, RuleSet* rs)
{
    char buff[STRING_BUFF_SIZE];
    char buffNeg[STRING_BUFF_SIZE];
    int foundSomethingToDo = 0;
    int night = 0;
    while (foundSomethingToDo == 0 && night < NUM_DAYS)
    {
        snprintf(buff, STRING_BUFF_SIZE, "SLEEP_DEATH_[NIGHT%d]", night);
        snprintf(buffNeg, STRING_BUFF_SIZE, "NOT_SLEEP_DEATH_[NIGHT%d]", night);
        if (isKnownName(kb, "PLAYERS", 0, buff) == 0 && isKnownName(kb, "PLAYERS", 0, buffNeg) == 0)
        {
            printf("%d. - ENTER WHO DIED IN THE NIGHT\n", foundSomethingToDo);
            foundSomethingToDo++;
        }
        snprintf(buff, STRING_BUFF_SIZE, "HANGING_DEATH_[NIGHT%d]", night);
        snprintf(buffNeg, STRING_BUFF_SIZE, "NOT_HANGING_DEATH_[NIGHT%d]", night);
        if (isKnownName(kb, "PLAYERS", 0, buff) == 0 && isKnownName(kb, "PLAYERS", 0, buffNeg) == 0)
        {
            printf("%d. - ENTER WHO WAS HUNG\n", foundSomethingToDo);
            foundSomethingToDo++;
        }
        snprintf(buff, STRING_BUFF_SIZE, "NOMINATION_DEATH_[NIGHT%d]", night);
        snprintf(buffNeg, STRING_BUFF_SIZE, "NOT_NOMINATION_DEATH_[NIGHT%d]", night);
        if (isKnownName(kb, "PLAYERS", 0, buff) == 0 && isKnownName(kb, "PLAYERS", 0, buffNeg) == 0)
        {
            printf("%d. - ENTER IF ANYONE DIED WHEN NOMINATING\n", foundSomethingToDo);
            foundSomethingToDo++;
        }
        snprintf(buff, STRING_BUFF_SIZE, "RESURRECTED_[NIGHT%d]", night);
        snprintf(buffNeg, STRING_BUFF_SIZE, "NOT_RESURRECTED_[NIGHT%d]", night);
        if (isKnownName(kb, "PLAYERS", 0, buff) == 0 && isKnownName(kb, "PLAYERS", 0, buffNeg) == 0)
        {
            printf("%d. - ENTER IF ANYONE WAS RESURRECTED\n", foundSomethingToDo);
            foundSomethingToDo++;
        }
        if (foundSomethingToDo != 0)
        {
            if (night == 0)
            {
                printf("%d. - ENTER ANY ROLE CLAIMS\n", foundSomethingToDo);
                foundSomethingToDo++;
            }
            printf("%d. - ENTER ANY PINGS\n", foundSomethingToDo);
            foundSomethingToDo++;
            printf("[TASKS FOR NIGHT %d]\n", night);
        }
        night++;
    }
}

/**
 * add_info() - add info to the knowledge base based on what the player adds
 * 
 * @kb the knowledge base to update
 * @rs the ruleset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
int add_info(KnowledgeBase* kb, RuleSet* rs)
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
        char NOT_POISONED[] = "NPS";
        char RED_HERRING[] = "RH";
        char DIED[] = "D";
        char HUNG[] = "H";
        char NOMINATION_DEATH[] = "ND";
        char FAILED_HANGING[] = "FH";
        char RESURRECTED[] = "R";
        char RESET[] = "RST";
        char FINISH[] = "SHOWDATA";
        char FINISH_PROB[] = "SHOWPROB";
        printf("ADD INFORMATION:\n");
        printf("- Type '%s' to enter what role a player was shown:\n", SHOWN_ROLE);
        printf("\n");
        printf("- Type '%s' to enter a role not in the game:\n", ROLE_NOT_IN_GAME);
        printf("- Type '%s' to submit n possibilities for a player:\n", N_POSSIBILITIES);
        printf("\n");
        printf("- Type '%s' to submit a ping:\n", PING);
        printf("\n");
        printf("- Type '%s' to submit that a player that is confirmed poisoned:\n", POISONED);
        printf("- Type '%s' to submit that a player that is confirmed not poisoned:\n", NOT_POISONED);
        printf("- Type '%s' to submit that a player that is confirmed red herring:\n", RED_HERRING);
        printf("\n");
        printf("- Type '%s' to submit that a/some player(s) died :\n", DIED);
        printf("- Type '%s' to submit that a/some player(s) that died when nominating a player:\n", NOMINATION_DEATH);
        printf("- Type '%s' to submit that a/some player(s) was hung and died:\n", HUNG);
        printf("- Type '%s' to submit that a/some player(s) was hung, but didn't die:\n", FAILED_HANGING);
        printf("- Type '%s' to submit that a/some player(s) was resurrected:\n", RESURRECTED);
        printf("\n");
        printf("- Type '%s' to reset a players data:\n", RESET);
        printf("\n");
        printf("- Type '%s' to finish entering data:\n", FINISH);
        printf("- Type '%s' to finish entering data and calculate the probabaility:\n", FINISH_PROB);
        printf("\n");
        printHeading("TODO LIST");
        printTodoList(kb, rs);

        
 
        scanf("%255s", buff); // Read a string (up to 99 characters to leave space for the null terminator)

        if (strcasecmp(buff,SHOWN_ROLE) == 0)
        {
            shown_role(kb);
        }
        else if (strcmp(buff,ROLE_NOT_IN_GAME) == 0)
        {
            roleNotInGame(kb);
        }
        else if (strcasecmp(buff,PING) == 0)
        {
            addPingRule(kb, rs);
        }
        else if (strcasecmp(buff,N_POSSIBILITIES) == 0)
        {
            noptions(kb);
        }
        else if (strcasecmp(buff,POISONED) == 0)
        {
            poisoned(kb);
        }
        else if (strcasecmp(buff,NOT_POISONED) == 0)
        {
            notPoisoned(kb);
        }
        else if (strcasecmp(buff,RED_HERRING) == 0)
        {
            redHerring(kb);
        }
        else if (strcasecmp(buff,DIED) == 0)
        {
            diedInNight(kb);
        }
        else if (strcasecmp(buff,HUNG) == 0)
        {
            hung(kb);
        }
        else if (strcasecmp(buff,NOMINATION_DEATH) == 0)
        {
            nominationDeath(kb);
        }
        else if (strcasecmp(buff,FAILED_HANGING) == 0)
        {
            //TODO
        }
        else if (strcasecmp(buff,RESURRECTED) == 0)
        {
            resurrected(kb);
        }
        else if (strcasecmp(buff,RESET) == 0)
        {
            reset(kb);
        }
        else if (strcasecmp(buff,FINISH) == 0)
        {
            printf("EXITING...");
            return 0;
        }
        else if (strcasecmp(buff,FINISH_PROB) == 0)
        {
            printf("EXITING...");
            return 1;
        }
        else
        {
            printf("ERROR: Invalid string!\n");
        }

    }

    return 0;
}