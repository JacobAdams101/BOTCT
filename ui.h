#pragma once

#include "rules.h"
#include "knowledge.h"
#include "constants.h"
#include "tb.h"

void printTitle(char *title, char *subheading);
void printHeading(char *title);
void setup(int *numPlayers, int *numMinions, int *numDemons, int *baseOutsiders);
void getNames(char* names[NUM_SETS][MAX_SET_ELEMENTS], int numPlayers);
int getRoleIdFromString(char* roleName);
int getRoleIDInput(char* message);
int getPlayerIDInput(KnowledgeBase* kb, char* message);

void shown_role(KnowledgeBase* kb);
void roleNotInGame(KnowledgeBase* kb);
void noptions(KnowledgeBase* kb);
void poisoned(KnowledgeBase* kb);
void redHerring(KnowledgeBase* kb);
void diedInNight(KnowledgeBase* kb);
void hung(KnowledgeBase* kb);
void reset(KnowledgeBase* kb);
void addPingRule(KnowledgeBase* kb, RuleSet* rs);
void add_info(KnowledgeBase* kb, RuleSet* rs);