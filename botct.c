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

void shown_role(KnowledgeBase* kb)
{
    int playerID;
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleID = -1;

    printf("ENERTING: SHOWN ROLE\n");

    printf("For player?:\n");
    scanf("%d", &playerID); // Read player ID

    printf("Role?:\n");
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        printf("%s\n", ROLE_NAMES[i]);
    }
    while (roleID == -1)
    {
        scanf("%255s", input); // Read a string (up to 99 characters to leave space for the null terminator)
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(input,ROLE_NAMES[i]) == 0)
            {
                roleID = i;
            }
        }
        if (roleID == -1)
        {
            printf("ERROR: Invalid string!\n");
        }
    }

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
    char input[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 
    int roleID = -1;

    printf("ENERTING: ROLE NOT IN GAME\n");


    printf("Role not in game?:\n");
    for (int i = 0; i < NUM_BOTCT_ROLES; i++)
    {
        printf("%s\n", ROLE_NAMES[i]);
    }
    while (roleID == -1)
    {
        scanf("%255s", input); // Read a string (up to 99 characters to leave space for the null terminator)
        for (int i = 0; i < NUM_BOTCT_ROLES; i++)
        {
            if (strcmp(input,ROLE_NAMES[i]) == 0)
            {
                roleID = i;
            }
        }
        if (roleID == -1)
        {
            printf("ERROR: Invalid string!\n");
        }
    }

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
            for (int i = 0; i < NUM_BOTCT_ROLES; i++)
            {
                if (strcmp(input,ROLE_NAMES[i]) == 0)
                {
                    roleIDs[j] = i;
                }
            }
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

void died(KnowledgeBase* kb)
{
    char buff[STRING_BUFF_SIZE]; // Declare a character array to hold the string 

    int playerID;
    int night;

    printf("ENERTING: PLAYER DIED\n");

    printf("For player?:\n");
    scanf("%d", &playerID); // Read player ID

    printf("On night?:\n");
    scanf("%d", &night); // Read player ID

    snprintf(buff, STRING_BUFF_SIZE, "died_NIGHT%d", night);
    addKnowledgeName(kb, "PLAYERS", playerID, buff);
}

void addPingRule(KnowledgeBase* kb)
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
    int night;

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

        //If <PLAYER_INFO>is_WASHERWOMAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
        //If <PLAYER_INFO>is_WASHERWOMAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]
        /*
        int selectedRole = -1;
        
        RULES[NUM_RULES]->varCount = 2;
        RULES[NUM_RULES]->varsMutuallyExclusive=0;

        snprintf(buff, STRING_BUFF_SIZE, "is_%s_in_PLAY", ROLE_NAMES[selectedRole]);
        setRuleResultName(RULES[NUM_RULES], 1, "METADATA", buff);
        addFixedConditionToRuleName(RULES[NUM_RULES], 0, "PLAYERS", "is_WASHERWOMAN", 0);
        addFixedConditionToRuleName(RULES[NUM_RULES], 0, "PLAYERS", "is_NOT_poisonedNIGHT0", 0);
        addFixedConditionToRuleName(RULES[NUM_RULES], 1, "METADATA", "is_NOT_SPY_in_PLAY", 0);
        NUM_RULES++;
        */
        
        
    }
    else if (pingTypeID == 1)
    { //LIBRARIAN_PING
        /*
        Start knowing one of two players is a particular outsider
        */

        //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
        //If <PLAYER_INFO>is_LIBRARIAN AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]

    }
    else if (pingTypeID == 2)
    { //INVESTIGATOR_PING
        /*
        Start knowing one of two players is a particular minion
        */

        //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_RECLUSE_in_PLAY => <METADATA>is_<ROLE>_in_PLAY
        //If <PLAYER_INFO>is_INVESTIGATOR AND <PLAYER_INFO>is_NOT_poisonedNIGHT0 AND <METADATA>is_NOT_RECLUSE_in_PLAY => <PLAYER_H>is_NOT_<ROLE> [where H is not X or Y]

    }
    else if (pingTypeID == 3)
    { //CHEF_PING
        /*
        Start knowing how many pairs of evil players there are
        */

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

    }
    else if (pingTypeID == 6)
    { //UNDERTAKER_PING
        //For 2 good 0 evil 
        //If <PLAYER_INFO>is_UNDERTAKER AND <PLAYER_INFO>is_NOT_poisonedNIGHT<night> AND <METADATA>is_NOT_RECLUSE_in_PLAY AND <METADATA>is_NOT_SPY_in_PLAY => <PLAYER_X>is_<ROLE>
    }
    else if (pingTypeID == 7)
    { //RAVENKEEPER_PING

    }
    printf("On night?:\n");
    scanf("%d", &night); // Read player ID

    
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
        char FINISH[] = "FINISH";
        printf("ADD INFORMATION:\n");
        printf("- Type '%s' to enter what role a player was shown:\n", SHOWN_ROLE);
        printf("- Type '%s' to enter a role not in the game:\n", ROLE_NOT_IN_GAME);
        printf("- Type '%s' to submit a ping:\n", PING);
        printf("- Type '%s' to submit n possibilities for a player:\n", N_POSSIBILITIES);
        printf("- Type '%s' to submit that a player that is confirmed poisoned:\n", POISONED);
        printf("- Type '%s' to submit that a player died:\n", DIED);
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
            addPingRule(kb);
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
            died(kb);
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

    
    /*
    addKnowledgeName(knowledgeBase, "PLAYERS", 0, "is_IMP");
    addKnowledgeName(knowledgeBase, "PLAYERS", 1, "is_POISONER");
    addKnowledgeName(knowledgeBase, "PLAYERS", 2, "is_SPY");
    addKnowledgeName(knowledgeBase, "PLAYERS", 3, "is_DRUNK");
    addKnowledgeName(knowledgeBase, "PLAYERS", 4, "is_SAINT");
    */
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

    printHeading("CREATING GAME...");
    printf("There are %d players in the game\n", NUM_PLAYERS);
    printf("There are %d minions in the game\n", NUM_MINIONS);
    printf("There are %d demons in the game\n", NUM_DEMONS);
    printf("There are %d(+2) outsiders in the game\n", BASE_OUTSIDERS);

    printf("INITIALIZE KNOWLEDGE BASE...\n");
    KnowledgeBase* kb = NULL;
    RuleSet* rs = NULL;

    initTB(&rs, &kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS, NUM_DAYS);

    printRules(rs, kb);

    printf("BEGIN GAME LOOP...\n");
    
    while (1)
    {

        add_info(kb, rs);

        printHeading("INFER FACTS");
        for (int i = 0; i < 4; i++)
        {
            printf("INFER FACTS [ROUND%d]:\n",i);
            inferknowledgeBaseFromRules(rs, kb);
        }

        printHeading("KNOWLEDGE BASE");
        printKnowledgeBase(kb);
        printHeading("PLAYER TABLE");
        printPlayerTable(kb);
        printHeading("ROLE TABLE");
        printRoleTable(kb);
    }

    return 0;
}