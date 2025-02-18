#pragma once

#define MAX_NUM_RULES 1000
#define MAX_VARS_IN_RULE 16
#define FUNCTION_RESULT_SIZE 2

#include "knowledge.h"

typedef struct
{
    //Meta data
    int varsMutuallyExclusive; //Are the variables mutually exclusive
    int varCount; //How many variables are there in the condition side

    //Condition side of implication
    long varConditions[MAX_VARS_IN_RULE][FUNCTION_RESULT_SIZE];
    int varConditionFromSet[MAX_VARS_IN_RULE];
    int varsForcedSubstitutions[MAX_VARS_IN_RULE]; //If this is used the program wont try and substitue values but instead check if these elements in the set are valid


    //Result side
    int resultVarName; //[0...MAX_VARS_IN_RULE] - same as in implication
                       //-1 -anything NOT in the conditions
                       //-2 -anything IN the conditions
                       //-10XX force substitue element XX
 
    long result[FUNCTION_RESULT_SIZE];
    int resultFromSet; 

    int LHSSymmetricAndIndependant;
} Rule;

typedef struct
{
    Rule *RULES[MAX_NUM_RULES];
    int NUM_RULES;
    Rule *temp_rule;
} RuleSet;

RuleSet* initRS();

int getNumRules(RuleSet* ruleSet);

Rule* getRule(RuleSet* ruleSet, int index);

void pushTempRule(RuleSet* ruleSet);
/*

Made private

void resetRule(Rule* rule);
void setRuleResult(Rule* rule, int resultVarName, int set, int function);
void setRuleResultName(Rule* rule, KnowledgeBase* kb, int resultVarName, char* set, char* function);
void addConditionToRule(Rule* rule, int varName, int set, int function, int forcedSubstitution);
void addConditionToRuleName(Rule* rule, KnowledgeBase* kb, int varName, char* set, char* function);
void addFixedConditionToRuleName(Rule* rule, KnowledgeBase* kb, int varName, char* set, char* function, int forcedSubstitution);
*/
int LHSSymmetricAndIndependant(Rule* rule);
void resetTempRule(RuleSet* rs);
void setTempRuleParams(RuleSet* rs, int varCount, int varsMutuallyExclusive);
void setTempRuleResult(RuleSet* rs, int resultVarName, int set, int function);
void setTempRuleResultName(RuleSet* rs, KnowledgeBase* kb, int resultVarName, char* set, char* function);
void addConditionToTempRule(RuleSet* rs, int varName, int set, int function, int forcedSubstitution);
void addConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function);
void addFixedConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function, int forcedSubstitution);



void printRule(Rule* rule, KnowledgeBase* kb);
void printRuleAssignment(Rule* rule, KnowledgeBase* kb, int assignement[MAX_VARS_IN_RULE], int resultAssignement);
void printRules(RuleSet* rs, KnowledgeBase* kb);

int satisfiesRule(Rule* rule, KnowledgeBase* kb, int verbose);
void inferknowledgeBaseFromRules(RuleSet* rs, KnowledgeBase* kb, int verbose);