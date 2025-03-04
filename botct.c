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

 /*
 *
 * ====> BLOOD ON THE CLOCKTOWER SOLVER
 * ==> By Jacob Adams
 * 
 * This program is designed to aid with games of blood on the clocktower
 * Do NOT use without the consent of your storyteller as that would ruin the game
 * 
 * The intention of this program is to help with notetaking and in the late game suggest potential worlds
 * 
 * 
 * ====> LIMITATIONS
 * 
 * - This program cannot iterate over all possible worlds and therefore will not be able to always eliminate cases
 * as well as the best human players can
 * 
 * - This program can only use roles from the following scripts:
 * "Trouble Brewing", "Sects and Violets" and "Bad Moon Rising" (As of 24/02/2025)
 * 
 * - This program often assumes the "Metascript" of every role in the 3 standard scripts:
 * "Trouble Brewing", "Sects and Violets" and "Bad Moon Rising" (As of 24/02/2025)
 * Becuase of this some features may be redudant in some scripts or seem unnessecary
 * 
 * - Do not support Windows... pthreads moment :(
 * 
 * The solver partitions information into nights nights run from the sleeping phase to the voting phase
 * For a clear example, see the diagram below:
 * 
 * | SEEN ROLE | NIGHT | DAYTIME | VOTING | NIGHT | DAYTIME | VOTING | NIGHT | DAYTIME | VOTING |
 * |_NIGHT 0______________________________|_NIGHT 1__________________|_NIGHT 2__________________|
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rules.h"
#include "knowledge.h"
#include "constants.h"
#include "scripts.h"
#include "ui.h"
#include "util.h"
#include "solver.h"



/**
 * main() - Main function
 *
*/
int main()
{
    
    
    initRand();

    //Variables
    int NUM_PLAYERS;
    int NUM_MINIONS;
    int NUM_DEMONS;
    int BASE_OUTSIDERS;
    int SCRIPT;

    setup(&NUM_PLAYERS, &NUM_MINIONS, &NUM_DEMONS, &BASE_OUTSIDERS, &SCRIPT);

    printHeading("CREATING GAME..."); //UI HEADING
    printf("There are %d players in the game\n", NUM_PLAYERS);
    printf("There are %d minions in the game\n", NUM_MINIONS);
    printf("There are %d demons in the game\n", NUM_DEMONS);
    printf("There are %d(+2) outsiders in the game\n", BASE_OUTSIDERS);

    printf("INITIALIZE KNOWLEDGE BASE...\n");
    //Make knowlegde base
    KnowledgeBase* kb = NULL;
    RuleSet* rs = NULL;

    initScript(&rs, &kb, SCRIPT, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    getNames(kb->ELEMENT_NAMES, NUM_PLAYERS);
    
    //Don't print rules
    //printRules(rs, kb);

    solve(kb, rs, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    return 0;
}