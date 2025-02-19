#pragma once

#define NUM_BOTCT_ROLES 72
#include "rules.h"
#include "knowledge.h"

void initTB(RuleSet** rs, KnowledgeBase** kb, int NUM_PLAYERS, int NUM_MINIONS, int NUM_DEMONS, int BASE_OUTSIDERS, int NUM_DAYS);
void buildRules(RuleSet* rs, KnowledgeBase* kb, int numPlayers, int numMinions, int numDemons, int baseOutsiders);

extern char *ROLE_NAMES[NUM_BOTCT_ROLES];
extern char *ROLE_TEAMS[NUM_BOTCT_ROLES];
extern char *ROLE_CLASSES[NUM_BOTCT_ROLES]; 
extern int ROLE_IN_SCRIPT[NUM_BOTCT_ROLES]; 