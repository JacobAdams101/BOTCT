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

#define NUM_BOTCT_ROLES 72
#include "rules.h"
#include "knowledge.h"

/**
 * initScript() - initialise a script for blood on the clocktower
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @SCRIPT the script to play
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
void initScript(RuleSet** rs, KnowledgeBase** kb, const int SCRIPT, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS, const int NUM_DAYS);

/**
 * buildRules() - add all the rules to the game
 * 
 * @rs the ruleset object to write to
 * @kb the knoweledge base to write to
 * @NUM_PLAYERS the number of players playing
 * @NUM_MINIONS the number of base minions in the script
 * @NUM_DEMONS the number of base, starting demons in the script
 * @BASE_OUTSIDERS the number of base starting outsiders in the script
 * @NUM_DAYS the maximium number of days the game can run for
*/
void buildRules(RuleSet* rs, KnowledgeBase* kb, const int NUM_PLAYERS, const int NUM_MINIONS, const int NUM_DEMONS, const int BASE_OUTSIDERS, const int NUM_DAYS);

/*
 * Global variables, ew
*/
extern char *ROLE_NAMES[NUM_BOTCT_ROLES];
extern char *ROLE_TEAMS[NUM_BOTCT_ROLES];
extern char *ROLE_CLASSES[NUM_BOTCT_ROLES]; 
extern int ROLE_IN_SCRIPT[NUM_BOTCT_ROLES]; 

extern int NUM_ROLES_IN_SCRIPT;