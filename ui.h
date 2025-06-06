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

#pragma once

#include "rules.h"
#include "knowledge.h"
#include "constants.h"

/**
 * printTitle() - prints a title to the terminal
 * 
 * @title the title string
 * @subheading the subheading string
*/
void printTitle(char *title, char *subheading);

/**
 * printHeading() - prints a heading to the terminal
 * 
 * @title the title string
*/
void printHeading(char *title);

/**
 * printRedHeading() - prints a heading to the terminal
 * 
 * @title the title string
*/
void printRedHeading(char *title);

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
void setup(int *numPlayers, int *numMinions, int *numDemons, int *baseOutsiders, int *script);

/**
 * getNames() - gets a list of names
 * 
 * @names stores the strings enters
 * @numPlayers OUTPUTS the number of players to get the names of
*/
void getNames(char* names[NUM_SETS][MAX_SET_ELEMENTS], int numPlayers);

/**
 * getRoleIdFromString() - used to convert the role name into an index
 * 
 * @roleName the NAME of the role
 * 
 * @return returns the roleID/index associated with the string, -1 if no role is found
*/
int getRoleIdFromString(char* roleName);

/**
 * getRoleIDInput() - get a role input from the user
 * 
 * @message the message to ask the user
 * 
 * @return the roleID/index of the entered role
*/
int getRoleIDInput(char* message);

/**
 * getPlayerIDInput() - get the name of the player from the user
 * 
 * @kb the knowledge base
 * @message the message to ask the user
 * 
 * @return the playerID/index of the entered player
*/
int getPlayerIDInput(KnowledgeBase* kb, char* message);

/**
 * getInt() - get an integer between min (inclusive) and max (exclusive)
 * 
 * @min min number (inclusive)
 * @max max number (exclusive)
 * @message the message to ask the user
 * 
 * @return a number between min <= NUM < max
*/
int getInt(char* message, int min, int max);

/**
 * add_info() - add info to the knowledge base based on what the player adds
 * 
 * @kb the knowledge base to update
 * @rs the ruleset to update
 * @NUM_DAYS the max number of days the game can go on for
*/
int add_info(KnowledgeBase* kb, RuleSet* rs);

/*
 * EXTERNAL UI CALLS
*/

void shown_role(KnowledgeBase* kb, int playerID, int roleID, int night);
void roleNotInGame(KnowledgeBase* kb, int roleID, int night);
void noptions(KnowledgeBase* kb, int playerID, int n, int roleIDs[], int night);
void poisoned(KnowledgeBase* kb, int playerID, int night);
void notPoisoned(KnowledgeBase* kb, int playerID, int night);
/**
 * hasPoisoned() - assume that a player has been poisoned by someone
 * 
 * @kb the knowledge base to update
 * @NUM_DAYS the max number of days the game can go on for
*/
void hasPoisoned(KnowledgeBase* kb, int playerID, int poisonedID, int night);
void redHerring(KnowledgeBase* kb, int playerID);
void killedPlayer(KnowledgeBase* kb, int playerID, int playerX, int night);
void diedInNight(KnowledgeBase* kb, int n, int playerID[], int night);
void hung(KnowledgeBase* kb, int n, int playerID[], int night);
void nominationDeath(KnowledgeBase* kb, int n, int playerID[], int night);
void resurrected(KnowledgeBase* kb, int n, int playerID[], int night);

/**
 * resetMetaData() - reset the players's entire knowledge to "Unknown"
 * X(PLAYER) = FALSE and NOT_X(PLAYER) = FALSE FOR ALL X FOR PLAYERS
 * 
 * @kb the knowledge base to update
*/
void reset(KnowledgeBase* kb, int playerID);
/**
 * resetMetaData() - reset the metadata's entire knowledge to "Unknown"
 * X(METADATA) = FALSE and NOT_X(METADATA) = FALSE FOR ALL X FOR METADATA
 * 
 * @kb the knowledge base to update
*/
void resetMetaData(KnowledgeBase* kb);

void washerWomanPing(int playerIDinfoFrom, int selectedRole, int playerX, int playerY, KnowledgeBase* kb, RuleSet* rs);
void librarianPing(int playerIDinfoFrom, int selectedRole, int playerX, int playerY, KnowledgeBase* kb, RuleSet* rs);
void investigatorPing(int playerIDinfoFrom, int selectedRole, int playerX, int playerY, KnowledgeBase* kb, RuleSet* rs);
void chefPing(int playerIDinfoFrom, int countPairs, KnowledgeBase* kb, RuleSet* rs);
void empathPing(int playerIDinfoFrom, int countEvil, int night, KnowledgeBase* kb, RuleSet* rs);
void fortuneTellerPing(int playerIDinfoFrom, int count, int playerX, int playerY, int night, KnowledgeBase* kb, RuleSet* rs);
void undertakerPing(int playerIDinfoFrom, int selectedRole, int playerX, int night, KnowledgeBase* kb, RuleSet* rs);
void monkPing(int playerIDinfoFrom, int playerX, int night, KnowledgeBase* kb, RuleSet* rs);
void ravenkeeperPing(int playerIDinfoFrom, int selectedRole, int playerX, int night, KnowledgeBase* kb, RuleSet* rs);
void clockmakerPing(int playerIDinfoFrom, int numSteps, KnowledgeBase* kb, RuleSet* rs);