#pragma once

#include "rules.h"
#include "knowledge.h"


#define NUM_SOLVE_STEPS 5

void solve(KnowledgeBase* kb, RuleSet* rs, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS, const int NUM_DAYS);