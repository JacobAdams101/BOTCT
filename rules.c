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

/**
 * getNumRules() - gets the number of rules stored
 * 
 * @ruleSet - the ruleset which stores the rules
 * 
 * @return the number of rules
*/
int getNumRules(RuleSet* ruleSet)
{
    return ruleSet->NUM_RULES;
}

/**
 * getRule() - gets the rule stored at index
 * 
 * @ruleSet - the ruleset which stores the rules
 * @index - the location of the rule to get
 * 
 * @return the rule at index
*/
Rule* getRule(RuleSet* ruleSet, int index)
{
    return ruleSet->RULES[index];
}

/**
 * resetRule() - reset all the data in a rule back to default
 * 
 * @rule - the rule to reset
*/
static void resetRule(Rule* rule)
{
    rule->varCount = 0;
    rule->resultVarName = 0;
    rule->resultFromSet = 0;
    rule->LHSSymmetric = 0;
    for (int i = 0; i < MAX_VARS_IN_RULE; i++)
    {
        rule->varConditionFromSet[i] = 0;
        for (int j = 0; j < FUNCTION_RESULT_SIZE; j++)
        {
            rule->varConditions[i][j] = 0;

        }
    }
    for (int j = 0; j < FUNCTION_RESULT_SIZE; j++)
    {
        rule->result[j] = 0;
    }

}

/**
 * LHSSymmetric() - used to check if the LHS of a rule is of the form:
 * EXP(w) AND EXP(x) AND EXP(y) AND EXP(z) AND ... => ...SOMETHING...
 * -where EXP() represents some identical expression for w,x,y,z
 * 
 * NOTE: this is useful for various optimisations by opserving permuting w,x,y,z... is wasteful,
 * saving O(n!) time complexity
 * 
 * @rule - the rule to test
 * 
 * @return TRUE if the left hand side of the rule impication is identical across all variables
*/
static int LHSSymmetric(Rule* rule)
{
    //Check LHS Symmetric
    for (int var = 1; var < rule->varCount; var++)
    {
        if (rule->varConditionFromSet[0] != rule->varConditionFromSet[var])
        {
            return 0;
        }
        if (rule->varsForcedSubstitutions[0] != rule->varsForcedSubstitutions[var])
        {
            return 0;
        }
        for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
        {
            if (rule->varConditions[0][i] != rule->varConditions[var][i])
            {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * pushTempRule() - tempRule is a working space to build rules quickly
 * temp rule is kindof similar toa  builder pattern
 * 
 * @ruleSet - the ruleset to put the temp rule
*/
void pushTempRule(RuleSet* ruleSet)
{
    long somethingInRuleExists = 0;
    for (int var = 0; var < MAX_VARS_IN_RULE; var++)
    {
        for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
        {
            somethingInRuleExists |= ruleSet->temp_rule->varConditions[var][i];
        }
    }
    for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
    {
        somethingInRuleExists |= ruleSet->temp_rule->result[i];
    }
    if (somethingInRuleExists == 0)
    {
        printf("INVALID RULE MADE!\n");
        exit(1);
    }
    //See if LHS is symmetric for an optimisation to checker
    ruleSet->temp_rule->LHSSymmetric = LHSSymmetric(ruleSet->temp_rule);

    //Copy temp rule
    memcpy(ruleSet->RULES[ruleSet->NUM_RULES], ruleSet->temp_rule, sizeof(Rule));
    //added one more rule
    ruleSet->NUM_RULES++;
    //Reset temp rule
    resetRule(ruleSet->temp_rule);
}

/**
 * initRS() - initialise the ruleset
 * 
 * @return returns the initilised ruleset
*/
RuleSet* initRS()
{
    printf("--Make Memory...\n");
    //Allocate memory
    RuleSet* ruleSet = (RuleSet*) malloc(sizeof(RuleSet));
    printf("--Set num rules...\n");
    ruleSet->NUM_RULES = 0;
    printf("--Reset rule...\n");
    //Reset the rules by writing zeroes everywhere
    for(int i = 0; i < MAX_NUM_RULES; i++)
    {
        ruleSet->RULES[i] = (Rule*) malloc(sizeof(Rule));
        resetRule(ruleSet->RULES[i]);

        ruleSet->RULE_ACTIVE[i] = 1;
    }
    printf("--Reset builder rule...\n");
    ruleSet->temp_rule = (Rule*) malloc(sizeof(Rule));
    resetRule(ruleSet->temp_rule);
    printf("--Done!\n");

    return ruleSet;
}

/**
 * resetTempRule() - reset the temp rule
 * 
 * @rs the ruleset with the temp rule to reset
*/
void resetTempRule(RuleSet* rs)
{
    resetRule(rs->temp_rule);
}

/**
 * setTempRuleParams() - set the parameter of the rule to add
 * 
 * @rs the ruleset with the temp rule to set
 * @varCount the number of variables in the rule
 * @varsMutuallyExclusive 1 if variables are mutually exclusive 0 otherwise
*/
void setTempRuleParams(RuleSet* rs, int varCount, int varsMutuallyExclusive)
{
    rs->temp_rule->varCount = varCount;
    rs->temp_rule->varsMutuallyExclusive = varsMutuallyExclusive;
}

/**
 * setRuleResult() - set the result of a rule
 * 
 * @rule the rule to set the result of
 * @resultVarName the var name on the RHS
 * @set the set of the element of the var
 * @function the function on the RHS
*/
static void setRuleResult(Rule* rule, int resultVarName, int set, int function)
{
    rule->resultVarName = resultVarName;
    rule->resultFromSet = set;

    int index, bit;
    getIndexAndBit(&index, &bit, function);

    long mask = 1L << bit;

    rule->result[index] |= mask;
}

/**
 * setTempRuleResult() - set the result of a temprule
 * 
 * @rs the ruleset with the temp rule to set the result of
 * @resultVarName the var name on the RHS
 * @set the set of the element of the var
 * @function the function on the RHS
*/
void setTempRuleResult(RuleSet* rs, int resultVarName, int set, int function)
{
    setRuleResult(rs->temp_rule, resultVarName, set, function);
}

/**
 * setRuleResultName() - 
 * 
 * @rule the rule to set the result of
 * @kb the knowledge base the rule is for
 * @resultVarName the var name on the RHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
*/
static void setRuleResultName(Rule* rule, KnowledgeBase* kb, int resultVarName, char* set, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    setRuleResult(rule, resultVarName, setID, functionID);
}

/**
 * setTempRuleResultName() - 
 * 
 * @rs the ruleset with the temprule to set the result of
 * @kb the knowledge base the rule is for
 * @resultVarName the var name on the RHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
*/
void setTempRuleResultName(RuleSet* rs, KnowledgeBase* kb, int resultVarName, char* set, char* function)
{
    setRuleResultName(rs->temp_rule, kb, resultVarName, set, function);
}

/**
 * addConditionToRule() - 
 * 
 * @rule the rule to add the condition to
 * @varName the var name on the LHS
 * @set the set of the element of the var
 * @function the function on the RHS
 * @forcedSubstitution if the subsitution is forced to a specific element in the knowledge base
*/
static void addConditionToRule(Rule* rule, int varName, int set, int function, int forcedSubstitution)
{
    int index, bit;
    getIndexAndBit(&index, &bit, function);

    long mask = 1L << bit;
    
    rule->varConditions[varName][index] |= mask;
    rule->varConditionFromSet[varName] = set;
    rule->varsForcedSubstitutions[varName] = forcedSubstitution;
}

/**
 * addConditionToTempRule() - 
 * 
 * @rs the ruleset with the temprule to add the condition to
 * @varName the var name on the LHS
 * @set the set of the element of the var
 * @function the function on the RHS
 * @forcedSubstitution if the subsitution is forced to a specific element in the knowledge base
*/
void addConditionToTempRule(RuleSet* rs, int varName, int set, int function, int forcedSubstitution)
{
    addConditionToRule(rs->temp_rule, varName, set, function, forcedSubstitution);
}

/**
 * addConditionToRuleName() - 
 * 
 * @rule the rule to add the condition to
 * @kb the knowledge base the rule is for
 * @varName the var name on the LHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
*/
static void addConditionToRuleName(Rule* rule, KnowledgeBase* kb, int varName, char* set, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    addConditionToRule(rule, varName, setID, functionID, -1);
}

/**
 * addConditionToTempRuleName() - 
 * 
 * @rs the ruleset with the temprule to add the condition to
 * @kb the knowledge base the rule is for
 * @varName the var name on the LHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
*/
void addConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function)
{
    addConditionToRuleName(rs->temp_rule, kb, varName, set, function);
}

/**
 * addFixedConditionToRuleName() - 
 * 
 * @rule the rule to add the condition to
 * @kb the knowledge base the rule is for
 * @varName the var name on the LHS
 * @set the NAME of the set of the element of the var
 * @function the NAME of the function on the RHS
 * @forcedSubstitution if the subsitution is forced to a specific element in the knowledge base
*/
static void addFixedConditionToRuleName(Rule* rule, KnowledgeBase* kb, int varName, char* set, char* function, int forcedSubstitution)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    addConditionToRule(rule, varName, setID, functionID, forcedSubstitution);
}

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
void addFixedConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function, int forcedSubstitution)
{
    addFixedConditionToRuleName(rs->temp_rule, kb, varName, set, function, forcedSubstitution);
}

/**
 * canRuleProvideNovelInformation() - 
 * 
 * @rule
 * @kb the knowledge base the rule is for
 * 
 * @return 1 if this rule can find novel information 0 if otherwise
*/
inline static int canRuleProvideNovelInformation(Rule* rule, KnowledgeBase* kb)
{
    for (int element = 0; element < kb->SET_SIZES[rule->resultFromSet]; element++)
    {
        for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
        {
            if ((kb->KNOWLEDGE_BASE[rule->resultFromSet][element][i] & rule->result[i]) != rule->result[i]) return 1;
        }
    }
    return 0;
}

/**
 * optimiseRuleset() - optimise the ruleset by seeing how many rules can be disabled
 * due to lack of novel information they will provide
 * 
 * @rs the ruleset to optimise
 * @kb the knowledge base the rule is for
 * 
 * @return 1 if this rule can find novel information 0 if otherwise
*/
void optimiseRuleset(RuleSet* rs, KnowledgeBase* kb)
{
    int count = 0;
    for (int rule = 0; rule < rs->NUM_RULES; rule++)
    {
       rs->RULE_ACTIVE[rule] = canRuleProvideNovelInformation(rs->RULES[rule], kb);
       if (rs->RULE_ACTIVE[rule] == 0) count++;
    }
    printf("%d / %d rules disabled\n", count, rs->NUM_RULES);
}

/**
 * printRule() - 
 * 
 * @rule the rule to print
 * @kb the knowledge base the rule is for
*/
void printRule(Rule* rule, KnowledgeBase* kb)
{
    //printf("PR-0\n"); //Remove
    int finalCount = 0;
    int count = 0;
    //Print conditions
    for (int var = 0; var < rule->varCount; var++)
    {
        //printf("PR-1\n"); //Remove
        finalCount = 0;
        for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            int index, bit;
            getIndexAndBit(&index, &bit, function);
            
            if (rule->varConditions[var][index] & (1L << bit))
            {
                finalCount++;
            }
        }
        count = 0;
        //printf("PR-2\n"); //Remove
        for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            //printf("PR-2A\n"); //Remove
            int index, bit;
            getIndexAndBit(&index, &bit, function);
            //printf("PR-2B\n"); //Remove
            if (rule->varConditions[var][index] & (1L << bit))
            {
                //printf("PR-2C\n"); //Remove
                //printf("PR-2CA %d\n", dgufgd); //Remove
                printf("%s(%d:%s)", kb->FUNCTION_NAME[rule->varConditionFromSet[var]][function], var, kb->SET_NAMES[rule->varConditionFromSet[var]]);
                count++;
                //printf("PR-2D\n"); //Remove
                if (count < finalCount)
                {
                    printf(" ^ ");
                }
            }
            //printf("PR-2Z\n"); //Remove
        }
    }
    //printf("PR-3\n"); //Remove
    printf(" => ");
    //Print result
    finalCount = 0;
    for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
    {
        int index, bit;
        getIndexAndBit(&index, &bit, function);
        
        if (rule->result[index] & (1L << bit))
        {
            finalCount++;
        }
    }
    //printf("PR-4\n"); //Remove
    count = 0;
    for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
    {
        int index, bit;
        getIndexAndBit(&index, &bit, function);
        
        if (rule->result[index] & (1L << bit))
        {
            printf("%s(%d:%s)", kb->FUNCTION_NAME[rule->resultFromSet][function], rule->resultVarName, kb->SET_NAMES[rule->resultFromSet]);
            count++;
            if (count < finalCount)
            {
                printf(" ^ ");
            }
        }
    }
    printf("\n");
}

/**
 * printRuleAssignment() - 
 * 
 * @rule the rule to print
 * @kb the knowledge base the rule is for
 * @assignement assignment for LHS
 * @resultAssignement assignment for RHS
*/
void printRuleAssignment(Rule* rule, KnowledgeBase* kb, int assignement[MAX_VARS_IN_RULE], int resultAssignement)
{
    int finalCount = 0;
    int count = 0;
    //Print conditions
    for (int var = 0; var < rule->varCount; var++)
    {
        finalCount = 0;
        for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            int index, bit;
            getIndexAndBit(&index, &bit, function);
            
            if (rule->varConditions[var][index] & (1L << bit))
            {
                finalCount++;
            }
        }
        count = 0;
        for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            int index, bit;
            getIndexAndBit(&index, &bit, function);
            
            if (rule->varConditions[var][index] & (1L << bit))
            {
                printf("%s(%d:%s)", kb->FUNCTION_NAME[rule->varConditionFromSet[var]][function], assignement[var], kb->SET_NAMES[rule->varConditionFromSet[var]]);
                count++;
                if (count < finalCount || var+1 < rule->varCount)
                {
                    printf(" ^ ");
                }
            }
        }
    }
    printf(" => ");
    //Print result
    finalCount = 0;
    for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
    {
        int index, bit;
        getIndexAndBit(&index, &bit, function);
        
        if (rule->result[index] & (1L << bit))
        {
            finalCount++;
        }
    }
    count = 0;
    for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
    {
        int index, bit;
        getIndexAndBit(&index, &bit, function);
        
        if (rule->result[index] & (1L << bit))
        {
            printf("%s(%d:%s)", kb->FUNCTION_NAME[rule->resultFromSet][function], resultAssignement, kb->SET_NAMES[rule->resultFromSet]);
            count++;
            if (count < finalCount)
            {
                printf(" ^ ");
            }
        }
    }
    printf("\n");
}

/**
 * getAssignment() - recursivley assign variables by permuting the variables which are satisfied
 * 
 * @satisfied the variables elementIDs which satisfy condition [index]
 * @lengths the lengths
 * @assignment the output assignment array
 * @var the variable for the recursive nature
 * @count the seed which generates the permutation
*/
static inline void getAssignment(int satisfied[MAX_VARS_IN_RULE][MAX_SET_ELEMENTS], int lengths[MAX_VARS_IN_RULE], int assignment[MAX_VARS_IN_RULE], long count)
{
    for (int var = 0; var < MAX_VARS_IN_RULE; var++)
    {
        int nextCount = count / lengths[var];
        int takeFromList = count - (nextCount * lengths[var]);

        assignment[var] = satisfied[var][takeFromList];

        count = nextCount;
    }
}

/**
 * applyResult() - 
 * 
 * @kb
 * @result
 * @set
 * @element
 * 
 * @return
*/
static inline int applyResult(KnowledgeBase* kb, long result[FUNCTION_RESULT_SIZE], int set, int element)
{
    int novelInformation = 0;
    for (int i = 0; i < FUNCTION_RESULT_SIZE; i++)
    {
        novelInformation |= (kb->KNOWLEDGE_BASE[set][element][i] & result[i]) != result[i];
        kb->KNOWLEDGE_BASE[set][element][i] |= result[i];
    }
    return novelInformation;
}

/**
 * applyRule() - 
 * 
 * @rule
 * @kb
 * @assignment
 * @verbose
 * 
 * @return
*/
static int applyRule(Rule* rule, KnowledgeBase* kb, int assignement[MAX_VARS_IN_RULE], int verbose)
{
    int foundNovelInformation = 0;
    if (rule->resultVarName >= 0)
    { //Result found in condition
        if (applyResult(kb, rule->result, rule->resultFromSet, assignement[rule->resultVarName]))
        {
            foundNovelInformation = 1;
            if (verbose) printRuleAssignment(rule, kb, assignement, assignement[rule->resultVarName]);
        }
        
    }
    else if (rule->resultVarName == -1)
    { //Result can be anything NOT in condition
        for (int setElement = 0; setElement < kb->SET_SIZES[rule->resultFromSet]; setElement++)
        {
            int inAssignment = 0;
            for (int i = 0; i < rule->varCount; i++)
            {
                if (assignement[i] == setElement && rule->varConditionFromSet[i] == rule->resultFromSet)
                {
                    inAssignment = 1;
                    break;
                }
            }
            if (inAssignment == 0 && applyResult(kb, rule->result, rule->resultFromSet, setElement))
            {
                foundNovelInformation = 1;
                if (verbose) printRuleAssignment(rule, kb, assignement, setElement);
            }
        }
        
    }
    else if (rule->resultVarName == -2)
    { //Result can be anything IN condition
    }
    else if (rule->resultVarName <= -1000)
    { //Result can be ONLY -1XXX where XXX is the element ID
        int varToSub = (-rule->resultVarName)-1000;

        if (applyResult(kb, rule->result, rule->resultFromSet, varToSub))
        {
            foundNovelInformation = 1;
            if (verbose) printRuleAssignment(rule, kb, assignement, varToSub);
        }
    }
    return foundNovelInformation;
}

/**
 * elementSatisfiesVarConditions() - 
 * 
 * @rule the rule to check if the LHS is satsified
 * @kb the knowledge base
 * @set 
 * @element
 * @var
*/
static inline int elementSatisfiesVarConditions(Rule* rule, KnowledgeBase* kb, int set, int element, int var)
{
    for(int i = 0; i < FUNCTION_RESULT_SIZE; i++)
    {
        if ((kb->KNOWLEDGE_BASE[set][element][i] & rule->varConditions[var][i]) != rule->varConditions[var][i]) return 0;
    }
    return 1;
}

/**
 * findPossibleSubstitutions() - find elements of sets that
 * can satisfy some of the LHS conditions of a rule
 * Used to cull set elements before permuting them to look for
 * ways to satisfy the rules
 * 
 * @rule the rule to check if the LHS is satsified
 * @kb the knowledge base
 * @satisfied writing what variables satisfy the result
 * @lengths how many variables satisfy the result
 * 
 * @return returns 0 if the valid substitutions trivially won't work (lengths[x] == 0 for some x), 1 otherwise
*/
static inline int findPossibleSubstitutions(Rule* rule, KnowledgeBase* kb, int satisfied[MAX_VARS_IN_RULE][MAX_SET_ELEMENTS], int lengths[MAX_VARS_IN_RULE])
{
    //Loop through vars in rules
    for (int var = 0; var<rule->varCount; var++)
    {
        lengths[var] = 0;

        int set = rule->varConditionFromSet[var];
        int forcedSub = rule->varsForcedSubstitutions[var];
        if (forcedSub != -1)
        {  //Look for forced substitutions
            //Check if the variable that has the forced substitution satisfies condition
            if (elementSatisfiesVarConditions(rule, kb, set, forcedSub, var))
            {
                satisfied[var][lengths[var]] = forcedSub;
                lengths[var]++;
            }
            else return 0;
        }
        else
        { //If not forced sub check all elements
            //Loop through elements of sets
            for(int element = 0; element<MAX_SET_ELEMENTS; element++)
            {
                if (elementSatisfiesVarConditions(rule, kb, set, element, var))
                { //in "set": fact num "element" would satisfy var number "var"
                    satisfied[var][lengths[var]] = element;
                    lengths[var]++;
                }
            }
            if (lengths[var] == 0) return 0;
        }
    }
    for (int var = rule->varCount; var<MAX_VARS_IN_RULE; var++) lengths[var] = 0;
    return 1;
    
}

static inline int isRepeatVar(int assignement[MAX_VARS_IN_RULE], int varCount)
{
    //Check that there are no duplicate assignements
    for(int var1 = 0; var1 < varCount-1; var1++)
    {
        for(int var2 = var1+1; var2 < varCount; var2++)
        {
            if (assignement[var1] == assignement[var2]) return 1;
        }
    }
    return 0;
}

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
int satisfiesRule(Rule* rule, KnowledgeBase* kb, int verbose)
{
    //Store arrays of possible var substittutions
    int satisfied[MAX_VARS_IN_RULE][MAX_SET_ELEMENTS];
    int lengths[MAX_VARS_IN_RULE];

    //Store array of potential assignement
    int assignement[MAX_VARS_IN_RULE];

    //Stores if any novel solution has been found
    int foundNovelSolution = 0;

    //Find possible substitutions for variable elements
    //ALSO: Check if there are trivial ways of showing no solutions to the rule
    //If there is trivially no solution exit function early
    if (findPossibleSubstitutions(rule, kb, satisfied, lengths) == 0) return 0;
    

    //If it satifies the rule continue by iterating 
    if (rule->LHSSymmetric)
    { //If it is symmetric and independant we done
        //Test If Assignement is valid
        if (rule->varsMutuallyExclusive)
        {
            if (lengths[0] < rule->varCount) //Only check first variable lengths[0] as symmetric so lengths[0]==lengths[i] for all i
            { //Lengths < rule->varCount   ---> no valid substitutions
            }
            else if (lengths[0] == rule->varCount)
            { //Lengths == rule->varCount   ---> One valid substitution
                for (int var = 0; var < rule->varCount; var++)
                {
                    assignement[var] = satisfied[var][var];
                }
                //If the assignement is valid
                //Update Knowledge Base
                foundNovelSolution |= applyRule(rule, kb, assignement, verbose); //If some novel information was added
            }
            else
            { //Lengths > rule->varCount   ---> Multiple valid substitutions
                for (int potentialSub = 0; potentialSub < lengths[0]; potentialSub++)
                { //Use potential sub as the variable to ignore
                    int count = 0;
                    for (int var = 0; var < rule->varCount; var++)
                    {
                        count += (count == potentialSub);
                        assignement[var] = satisfied[0][count];
                        count++;
                    }
                    //If the assignement is valid
                    //Update Knowledge Base
                    foundNovelSolution |= applyRule(rule, kb, assignement, verbose); //If some novel information was added
                }
            }
        }
        else
        {
            for (int potentialSub = 0; potentialSub < lengths[0]; potentialSub++)
            {
                for (int var = 0; var < rule->varCount; var++)
                {
                    assignement[var] = satisfied[var][potentialSub];
                }
                //If the assignement is valid
                //Update Knowledge Base
                foundNovelSolution |= applyRule(rule, kb, assignement, verbose); //If some novel information was added
            }
        }
        
    }
    else
    { //If not symmetric and independant we need to permute it
        //Figure out how many combinations we have
        long numCombinations = 1;
        for (int var = 0; var < rule->varCount; var++) numCombinations *= lengths[var];

        for(long count = 0; count < numCombinations; count++)
        {
            //Generate Assignement
            getAssignment(satisfied, lengths, assignement, count);

            if (!rule->varsMutuallyExclusive || !isRepeatVar(assignement, rule->varCount))
            { //If the assignement is valid
                //Update Knowledge Base
                foundNovelSolution |= applyRule(rule, kb, assignement, verbose); //If some novel information was added
            }
        }
    }
    return foundNovelSolution;
}

/**
 * inferknowledgeBaseFromRules() - For all rules in a ruleset 
 * check if any novel information can be infered
 * 
 * @rs the set of rules
 * @kb the knowledge base
 * @verbose print discoveries
 * 
 * @return 1 if a novel solution is found, -1 if a contradiction is found, 0 otherwise
*/
int inferknowledgeBaseFromRules(RuleSet* rs, KnowledgeBase* kb, int verbose)
{
    int foundNovelSolution = 0;
    for (int i = 0; i < rs->NUM_RULES; i++)
    {
        if (rs->RULE_ACTIVE[i])
        { //Only apply a rule if it hasn't been culled
            foundNovelSolution |= satisfiesRule(rs->RULES[i], kb, verbose);
            if (foundNovelSolution && hasExplicitContradiction(kb)) return -1;
        }
    }
    return foundNovelSolution;
}

/**
 * printRules() - print all the rules in a ruleset
 * 
 * @rs the set of rules to print
 * @kb the knowledge base
*/
void printRules(RuleSet* rs, KnowledgeBase* kb)
{
    for (int i = 0; i < rs->NUM_RULES; i++)
    {
        printRule(rs->RULES[i], kb);
    }
}

