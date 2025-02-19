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

int main()
{
    int NUM_PLAYERS;
    int NUM_MINIONS;
    int NUM_DEMONS;
    int BASE_OUTSIDERS;

    int NUM_DAYS = 10;
    setup(&NUM_PLAYERS, &NUM_MINIONS, &NUM_DEMONS, &BASE_OUTSIDERS);

    printHeading("CREATING GAME..."); //UI HEADING
    printf("There are %d players in the game\n", NUM_PLAYERS);
    printf("There are %d minions in the game\n", NUM_MINIONS);
    printf("There are %d demons in the game\n", NUM_DEMONS);
    printf("There are %d(+2) outsiders in the game\n", BASE_OUTSIDERS);

    printf("INITIALIZE KNOWLEDGE BASE...\n");
    KnowledgeBase* kb = NULL;
    RuleSet* rs = NULL;

    initTB(&rs, &kb, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS, NUM_DAYS);

    getNames(kb->ELEMENT_NAMES, NUM_PLAYERS);

    printRules(rs, kb);

    KnowledgeBase* temp_kb = initKB(NUM_PLAYERS, NUM_DAYS);
    KnowledgeBase* contradiction_kb = initKB(NUM_PLAYERS, NUM_DAYS);

    

    printf("BEGIN GAME LOOP...\n");

    int contradiction = 0;
    
    while (1)
    {
        

        //Create copy as backup incase of contradictions
        copyTo(temp_kb, kb);
        add_info(kb, rs);

        
        printHeading("INFER FACTS"); //UI HEADING
        
        for (int i = 0; i < 5; i++)
        {
            printf("INFER FACTS [ROUND%d]:\n",i);
            inferknowledgeBaseFromRules(rs, kb, 1);
        }
        //Check for contradictions
        contradiction = hasExplicitContradiction(kb);

        printHeading("LOOK FOR PROOF BY CONTRADICTION"); //UI HEADING
        if (contradiction == 0)
        { //If no immediate contradictions 
            //see if it's possible to rule out any further worlds
            //proof by contradiction
            for (int player = 0; player < NUM_PLAYERS; player++)
            {
                for (int function = 0; function < INT_LENGTH*FUNCTION_RESULT_SIZE; function++)
                {
                    int functionNeg;
                    // Using the fact KB[2n] = A KB[2n+1] = NOT A
                    if (function % 2 == 0)
                    {
                        functionNeg = function+1;
                    }
                    else
                    {
                        functionNeg = function-1;
                    }

                    if (isKnown(kb, 0, player, function) == 0 && isKnown(kb, 0, player, functionNeg) == 0)
                    { //If this is not already assumed
                        //Create copy of main space to look for contradiction
                        copyTo(contradiction_kb, kb);

                        //Assume true
                        addKnowledge(contradiction_kb, 0, player, function);

                        //Infer knowledge (to see if a contradiction arises)
                        for (int i = 0; i < 5; i++)
                        {
                            inferknowledgeBaseFromRules(rs, contradiction_kb, 0);
                        }

                        if (hasExplicitContradiction(contradiction_kb) == 1)
                        { //If contradiction found by only adding "function" to assumptions we know NOT function is true 
                            //Update KB
                            printf("FOUND CONTRADICTION: %s(%d:%s) => %s(%d:%s)\n", kb->FUNCTION_NAME[0][function], player, kb->SET_NAMES[0], kb->FUNCTION_NAME[0][functionNeg], player, kb->SET_NAMES[0]);
                            addKnowledge(kb, 0, player, functionNeg);
                        }
                    }
                }
            }
        }

        if (contradiction == 0)
        {
            printHeading("KNOWLEDGE BASE"); //UI HEADING
            printKnowledgeBase(kb);
            printHeading("PLAYER TABLE"); //UI HEADING
            printPlayerTable(kb);
            printHeading("ROLE TABLE"); //UI HEADING
            printRoleTable(kb);
        }

        if (contradiction == 1)
        {
            printHeading("CONTRADICTION FOUND"); //UI HEADING
            printf("Rolling back Knowledge base\n");
            //Roll back knowledge base
            copyTo(kb, temp_kb);
        }

    }

    return 0;
}