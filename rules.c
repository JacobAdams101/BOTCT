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



int getNumRules(RuleSet* ruleSet)
{
    return ruleSet->NUM_RULES;
}

Rule* getRule(RuleSet* ruleSet, int index)
{
    return ruleSet->RULES[index];
}


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

void pushTempRule(RuleSet* ruleSet)
{
    //See if LHS is symmetric for an optimisation to checker
    ruleSet->temp_rule->LHSSymmetric = LHSSymmetric(ruleSet->temp_rule);

    //Copy temp rule
    memcpy(ruleSet->RULES[ruleSet->NUM_RULES], ruleSet->temp_rule, sizeof(Rule));
    //added one more rule
    ruleSet->NUM_RULES++;
    //Reset temp rule
    resetRule(ruleSet->temp_rule);
}

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
    }
    printf("--Reset builder rule...\n");
    ruleSet->temp_rule = (Rule*) malloc(sizeof(Rule));
    resetRule(ruleSet->temp_rule);
    printf("--Done!\n");

    return ruleSet;
}




void resetTempRule(RuleSet* rs)
{
    resetRule(rs->temp_rule);
}

void setTempRuleParams(RuleSet* rs, int varCount, int varsMutuallyExclusive)
{
    rs->temp_rule->varCount = varCount;
    rs->temp_rule->varsMutuallyExclusive = varsMutuallyExclusive;
}

static void setRuleResult(Rule* rule, int resultVarName, int set, int function)
{
    rule->resultVarName = resultVarName;
    rule->resultFromSet = set;

    int index, bit;
    getIndexAndBit(&index, &bit, function);

    long mask = 1L << bit;

    rule->result[index] |= mask;
}

void setTempRuleResult(RuleSet* rs, int resultVarName, int set, int function)
{
    setRuleResult(rs->temp_rule, resultVarName, set, function);
}

static void setRuleResultName(Rule* rule, KnowledgeBase* kb, int resultVarName, char* set, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    setRuleResult(rule, resultVarName, setID, functionID);
}

void setTempRuleResultName(RuleSet* rs, KnowledgeBase* kb, int resultVarName, char* set, char* function)
{
    setRuleResultName(rs->temp_rule, kb, resultVarName, set, function);
}

static void addConditionToRule(Rule* rule, int varName, int set, int function, int forcedSubstitution)
{
    int index, bit;
    getIndexAndBit(&index, &bit, function);

    long mask = 1L << bit;
    
    rule->varConditions[varName][index] |= mask;
    rule->varConditionFromSet[varName] = set;
    rule->varsForcedSubstitutions[varName] = forcedSubstitution;
}

void addConditionToTempRule(RuleSet* rs, int varName, int set, int function, int forcedSubstitution)
{
    addConditionToRule(rs->temp_rule, varName, set, function, forcedSubstitution);
}

static void addConditionToRuleName(Rule* rule, KnowledgeBase* kb, int varName, char* set, char* function)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    addConditionToRule(rule, varName, setID, functionID, -1);
}

void addConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function)
{
    addConditionToRuleName(rs->temp_rule, kb, varName, set, function);
}

static void addFixedConditionToRuleName(Rule* rule, KnowledgeBase* kb, int varName, char* set, char* function, int forcedSubstitution)
{
    int setID = getSetIDWithName(kb, set, 1);
    int functionID = getSetFunctionIDWithName(kb, setID, function, 1);

    addConditionToRule(rule, varName, setID, functionID, forcedSubstitution);
}

void addFixedConditionToTempRuleName(RuleSet* rs, KnowledgeBase* kb, int varName, char* set, char* function, int forcedSubstitution)
{
    addFixedConditionToRuleName(rs->temp_rule, kb, varName, set, function, forcedSubstitution);
}

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
            
            if (((rule->varConditions[var][index] >> bit) & 1) == 1)
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
            if (((rule->varConditions[var][index] >> bit) & 1) == 1)
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
        
        if (((rule->result[index] >> bit) & 1) == 1)
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
        
        if (((rule->result[index] >> bit) & 1) == 1)
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
            
            if (((rule->varConditions[var][index] >> bit) & 1) == 1)
            {
                finalCount++;
            }
        }
        count = 0;
        for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            int index, bit;
            getIndexAndBit(&index, &bit, function);
            
            if (((rule->varConditions[var][index] >> bit) & 1) == 1)
            {
                printf("%s(%d:%s)", kb->FUNCTION_NAME[rule->varConditionFromSet[var]][function], assignement[var], kb->SET_NAMES[rule->varConditionFromSet[var]]);
                count++;
                if (count < finalCount)
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
        
        if (((rule->result[index] >> bit) & 1) == 1)
        {
            finalCount++;
        }
    }
    count = 0;
    for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
    {
        int index, bit;
        getIndexAndBit(&index, &bit, function);
        
        if (((rule->result[index] >> bit) & 1) == 1)
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

void getAssignment(int satisfied[MAX_VARS_IN_RULE][MAX_SET_ELEMENTS], int lengths[MAX_VARS_IN_RULE], int assignment[MAX_VARS_IN_RULE], int var, long count)
{
    int nextCount = count / lengths[var];
    int takeFromList = count - (nextCount * lengths[var]);

    assignment[var] = satisfied[var][takeFromList];

    if (var + 1 < MAX_VARS_IN_RULE)
    {
        getAssignment(satisfied, lengths, assignment, var+1, nextCount);
    }
    
}

static void applyRule(Rule* rule, KnowledgeBase* kb, int assignement[MAX_VARS_IN_RULE], int verbose)
{
    if (rule->resultVarName >= 0)
    { //Result found in condition
        
        int novelInformation = 0;
        for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            int index = function / INT_LENGTH;
            int bit = function - (index * INT_LENGTH);
            
            if (((rule->result[index] >> bit) & 1) == 1)
            {
                if (isKnown(kb, rule->resultFromSet, assignement[rule->resultVarName], function) == 0)
                {
                    novelInformation = 1;
                }
                addKnowledge(kb, rule->resultFromSet, assignement[rule->resultVarName], function);
            }
        }
        if (novelInformation == 1 && verbose == 1)
        {
            printRuleAssignment(rule, kb, assignement, assignement[rule->resultVarName]);
        }
        
    }
    else if (rule->resultVarName == -1)
    { //Result can be anything NOT in condition
        for (int setElement = 0; setElement < kb->SET_SIZES[rule->resultFromSet]; setElement++)
        {
            int novelInformation = 0;
            for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
            {
                int index = function / INT_LENGTH;
                int bit = function - (index * INT_LENGTH);
                
                if (((rule->result[index] >> bit) & 1) == 1)
                {
                    int inAssignment = 0;
                    for (int i = 0; i < rule->varCount; i++)
                    {
                        if (assignement[i] == setElement && rule->varConditionFromSet[i] == rule->resultFromSet)
                        {
                            inAssignment = 1;
                        }
                    }
                    if (inAssignment == 0)
                    { //If not in assignment
                        if (isKnown(kb, rule->resultFromSet, setElement, function) == 0)
                        {
                            novelInformation = 1;
                        }
                        addKnowledge(kb, rule->resultFromSet, setElement, function);
                    }
                }
            }
            if (novelInformation == 1 && verbose == 1)
            {
                printRuleAssignment(rule, kb, assignement, setElement);
            }
        }
        
    }
    else if (rule->resultVarName == -2)
    { //Result can be anything IN condition

    }
    else if (rule->resultVarName <= -1000)
    { //Result can be ONLY -1XXX where XXX is the element ID
        int varToSub = (-rule->resultVarName)-1000;

        int novelInformation = 0;
        for(int function = 0; function < FUNCTION_RESULT_SIZE*INT_LENGTH; function++)
        {
            int index = function / INT_LENGTH;
            int bit = function - (index * INT_LENGTH);
            
            if (((rule->result[index] >> bit) & 1) == 1)
            {
                if (isKnown(kb, rule->resultFromSet, varToSub, function) == 0)
                {
                    novelInformation = 1;
                }
                addKnowledge(kb, rule->resultFromSet, varToSub, function);
            }
        }
        if (novelInformation == 1 && verbose == 1)
        {
            printRuleAssignment(rule, kb, assignement, varToSub);
        }
    }
}

int satisfiesRule(Rule* rule, KnowledgeBase* kb, int verbose)
{

    int satisfied[MAX_VARS_IN_RULE][MAX_SET_ELEMENTS];
    for (int var = 0; var < MAX_VARS_IN_RULE; var++)
    {
        for (int element = 0; element < MAX_SET_ELEMENTS; element++)
        {
            satisfied[var][element] = -1;
        }
    }

    //Loop through vars in rules
    for (int var = 0; var<rule->varCount; var++)
    {
        int set = rule->varConditionFromSet[var];
        //Loop through elements of sets
        for(int element = 0; element<MAX_SET_ELEMENTS; element++)
        {
            int match = 1;
            for(int i = 0; i < FUNCTION_RESULT_SIZE; i++)
            {
                if ((kb->KNOWLEDGE_BASE[set][element][i] & rule->varConditions[var][i]) != rule->varConditions[var][i])
                {
                    match = 0;
                }
            }
            if (match == 1)
            { //in "set": fact num "element" would satisfy var number "var"
                int i = 0;
                while(satisfied[var][i] != -1)
                {
                    i++;
                }
                satisfied[var][i] = element;
            }
        }
    }

    //Look for forced substitutions
    for (int var = 0; var < rule->varCount; var++)
    {
        int forcedSub = rule->varsForcedSubstitutions[var];
        if (forcedSub != -1)
        {
            //Loop through all variables that satisfy this variable
            //Check if the variable that has the forced substitution appears
            //Remove every variable
            //IF the forced sub appears re add it at index 0
            int forcedSubValid = 0;
            for (int i = 0; i < MAX_SET_ELEMENTS; i++)
            {
                if (satisfied[var][i] == forcedSub)
                {
                    forcedSubValid = 1;
                }
                satisfied[var][i] = -1;
            }
            if (forcedSubValid == 1)
            {
                satisfied[var][0] = forcedSub;
            }
        }
    }

    //Check if there are trivial ways of showing no solutions to the rule
    for (int var = 0; var < rule->varCount; var++)
    {
        if (satisfied[var][0] == -1) return 0;
    }
    
    //If it satifies the rule continue by iterating 

    int lengths[MAX_VARS_IN_RULE];
    int assignement[MAX_VARS_IN_RULE];
    for (int var = 0; var < rule->varCount; var++)
    {
        int lengthCounter = 0;
        while(satisfied[var][lengthCounter] != -1)
        {
            lengthCounter++;
        }
        lengths[var] = lengthCounter;
    }

    long numCombinations = 1;
    for (int var = 0; var < rule->varCount; var++)
    {
        numCombinations *= lengths[var];
    }

    if (rule->LHSSymmetric == 1)
    { //If it is symmetric and independant we dont

        //Test If Assignement is valid
        if (rule->varsMutuallyExclusive == 1)
        {
            if (lengths[0] < rule->varCount)
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
                applyRule(rule, kb, assignement, verbose);
            }
            else
            { //Lengths > rule->varCount   ---> Multiple valid substitutions
                for (int potentialSub = 0; potentialSub < lengths[0]; potentialSub++)
                { //Use potential sub as the variable to ignore
                    int count = 0;
                    for (int var = 0; var < rule->varCount; var++)
                    {
                        if (count == potentialSub)
                        {
                            count++;
                        }
                        assignement[var] = satisfied[0][count];
                        count++;
                    }
                    //If the assignement is valid
                    //Update Knowledge Base
                    
                    applyRule(rule, kb, assignement, verbose);
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
                applyRule(rule, kb, assignement, verbose);
            }
        }
        
    }
    else
    { //If not symmetric and independant we need to permute it
        for(long count = 0; count < numCombinations; count++)
        {
            //Generate Assignement
            getAssignment(satisfied, lengths, assignement, 0, count);

            int validAssignment = 1;

            //Test If Assignement is valid
            if (rule->varsMutuallyExclusive == 1)
            {
                //Check that there are no duplicate assignements
                for(int var1 = 0; var1 < rule->varCount-1; var1++)
                {
                    for(int var2 = var1+1; var2 < rule->varCount; var2++)
                    {
                        if (assignement[var1] == assignement[var2])
                        {
                            validAssignment = 0;
                        }
                    }
                }
            }

            if (validAssignment == 1)
            { //If the assignement is valid
                //Update Knowledge Base
                applyRule(rule, kb, assignement, verbose);
            }
        }
    }
    
    return 1;
}

void inferknowledgeBaseFromRules(RuleSet* rs, KnowledgeBase* kb, int verbose)
{
    for (int i = 0; i < rs->NUM_RULES; i++)
    {
        int result = satisfiesRule(rs->RULES[i], kb, verbose);
    }
}

void printRules(RuleSet* rs, KnowledgeBase* kb)
{
    for (int i = 0; i < rs->NUM_RULES; i++)
    {
        printRule(rs->RULES[i], kb);
    }
}

