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

#include "constants.h"
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

    int LHSSymmetric;
} Rule;

typedef struct
{
    Rule *RULES[MAX_NUM_RULES];
    int RULE_ACTIVE[MAX_NUM_RULES];
    int NUM_RULES;
    Rule *temp_rule;
} RuleSet;

/**
 * initRS() - initialise the ruleset
 * 
 * @return returns the initilised ruleset
*/
RuleSet* initRS();

/**
 * getNumRules() - gets the number of rules stored
 * 
 * @ruleSet - the ruleset which stores the rules
 * 
 * @return the number of rules
*/
int getNumRules(RuleSet* ruleSet);

/**
 * getRule() - gets the rule stored at index
 * 
 * @ruleSet - the ruleset which stores the rules
 * @index - the location of the rule to get
 * 
 * @return the rule at index
*/
Rule* getRule(RuleSet* ruleSet, int index);

/**
 * pushTempRule() - tempRule is a working space to build rules quickly
 * temp rule is kindof similar toa  builder pattern
 * 
 * @ruleSet - the ruleset to put the temp rule
*/
void pushTempRule(RuleSet* ruleSet);

/**
 * resetTempRule() - reset the temp rule
 * 
 * @rs the ruleset with the temp rule to reset
*/
void resetTempRule(RuleSet* rs);

/**
 * setTempRuleParams() - set the parameter of the rule to add
 * 
 * @rs the ruleset with the temp rule to set
 * @varCount the number of variables in the rule
 * @varsMutuallyExclusive 1 if variables are mutually exclusive 0 otherwise
*/
void setTempRuleParams(RuleSet* rs, int varCount, int varsMutuallyExclusive);

/**
 * setTempRuleResult() - set the result of a temprule
 * 
 * @rs the ruleset with the temp rule to set the result of
 * @resultVarName the var name on the RHS
 * @set the set of the element of the var
 * @function the function on the RHS
*/
void setTempRuleResult(RuleSet* rs, int resultVarName, int set, int function);

/**
 * setTempRuleResultName() - 
 * 
 * @rs the ruleset with the temprule to set the result of
 * @kb the knowledge base the rule is for
 * @resultVarName the var name on the RHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
*/
void setTempRuleResultName(RuleSet* rs, KnowledgeBase* kb, int resultVarName, char* set, char* function);

/**
 * addConditionToTempRule() - 
 * 
 * @rs the ruleset with the temprule to add the condition to
 * @varName the var name on the LHS
 * @set the set of the element of the var
 * @function the function on the RHS
 * @forcedSubstitution if the subsitution is forced to a specific element in the knowledge base
*/
void addConditionToTempRule(RuleSet* rs, int varName, int set, int function, int forcedSubstitution);

/**
 * addConditionToTempRuleName() - 
 * 
 * @rs the ruleset with the temprule to add the condition to
 * @kb the knowledge base the rule is for
 * @varName the var name on the LHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
*/
void addConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function);

/**
 * addFixedConditionToRuleName() - 
 * 
 * @rs the ruleset with the temprule to add the condition to
 * @kb the knowledge base the rule is for
 * @varName the var name on the LHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
 * @forcedSubstitution if the subsitution is forced to a specific element in the knowledge base
*/
void addFixedConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function, int forcedSubstitution);

/**
 * optimiseRuleset() - optimise the ruleset by seeing how many rules can be disabled
 * due to lack of novel information they will provide
 * 
 * @rs the ruleset to optimise
 * @kb the knowledge base the rule is for
 * 
 * @return 1 if this rule can find novel information 0 if otherwise
*/
void optimiseRuleset(RuleSet* rs, KnowledgeBase* kb);

/**
 * printRule() - 
 * 
 * @rule the rule to print
 * @kb the knowledge base the rule is for
*/
void printRule(Rule* rule, KnowledgeBase* kb);

/**
 * printRuleAssignment() - 
 * 
 * @rule the rule to print
 * @kb the knowledge base the rule is for
 * @assignement assignment for LHS
 * @resultAssignement assignment for RHS
*/
void printRuleAssignment(Rule* rule, KnowledgeBase* kb, int assignement[MAX_VARS_IN_RULE], int resultAssignement);

/**
 * printRules() - print all the rules in a ruleset
 * 
 * @rs the set of rules to print
 * @kb the knowledge base
*/
void printRules(RuleSet* rs, KnowledgeBase* kb);

/**
 * satisfiesRule() - check if a knowledge base satisfies a rules LHS in a novel way
 * if it is add novel information to the KB
 * 
 * A novel solution is descibed as
 * 
 * X AND Y AND Z AND ... => A
 * 
 * WHERE KB already knows X,Y,Z,... but does NOT know A (so it's novel)
 * 
 * NOTE: this is used for optimisations with how many implication rounds to run when infering facts
 * 
 * @rule the rule to check if the LHS is satsified
 * @kb the knowledge base
 * @verbose print out information
 * 
 * @return TRUE if a novel solution is found
*/
int satisfiesRule(Rule* rule, KnowledgeBase* kb, int verbose);

/**
 * inferknowledgeBaseFromRules() - For all rules in a ruleset 
 * check if any novel information can be infered
 * 
 * @rs the set of rules
 * @kb the knowledge base
 * @verbose print discoveries
 * 
 * @return TRUE if a novel solution is found
*/
int inferknowledgeBaseFromRules(RuleSet* rs, KnowledgeBase* kb, int verbose);