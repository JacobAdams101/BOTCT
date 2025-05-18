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
 * | SEEN ROLE | NIGHT | DAYTIME | VOTING | NIGHT | DAYTIME | VOTING | NIGHT | DAYTIME | VOTING | NIGHT |
 * |_DAY 0_____________|_DAY 1____________________|_DAY 2____________________|_DAY 3____________________|
 * 
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <pthread.h>

#include "uitest.h"

#include "rules.h"
#include "knowledge.h"
#include "constants.h"
#include "scripts.h"
#include "ui.h"
#include "util.h"
#include "solver.h"

#define WIDTH 1600
#define HEIGHT 900

#define MAX_UI_ELEMENTS 1000
#define MAX_UI_ZONES 7

/***************************************************
 * UI ELEMENTS
 ***************************************************/
TextBox UI_ELEMENTS[MAX_UI_ZONES][MAX_UI_ELEMENTS];
int COUNT[MAX_UI_ZONES];
int UI_LINES[MAX_UI_ZONES][MAX_UI_ELEMENTS][4];
int LINE_COUNT[MAX_UI_ZONES];
int currentNight = 0;
int soloWorldPlayer = -1;
int soloWorldRole = -1;
bool reRenderCall = false;
int subMenuOpen = 0;
int subSubMenuOpen = 0;
int subSubSubMenuOpen = 0;
const int MAX_BUTTON_OPTIONS = NUM_BOTCT_ROLES+1;
int subSubSubSubMenuSelected[MAX_BUTTON_OPTIONS];
int subSubSubSubSubMenuSelected[MAX_BUTTON_OPTIONS*3];

/***************************************************
 * PROGRAM DATA
 ***************************************************/
KnowledgeBase* KNOWLEDGE_BASE = NULL;
KnowledgeBase* REVERT_KB = NULL;

ProbKnowledgeBase* WORLD_TALLY = NULL;

int WORLD_GENERATION = 1;

RuleSet* RULE_SET = NULL;


const int NUM_THREADS = 12;
const int NUM_ITERATIONS = 8;

ProbKnowledgeBase* threadTallies[NUM_THREADS];
KnowledgeBase* possibleWorldKB[NUM_THREADS];
KnowledgeBase**** possibleWorldTempKB[NUM_THREADS];
struct getProbApproxArgs* threadArgs[NUM_THREADS];

KnowledgeBase* POSSIBLE_WORLDS_FOR_PROB[MAX_SET_ELEMENTS][NUM_BOTCT_ROLES];
int POSSIBLE_WORLD_GENERATED[MAX_SET_ELEMENTS][NUM_BOTCT_ROLES];
//Thread object
pthread_t threads[NUM_THREADS];

/***************************************************
 * UI CODE
 ***************************************************/
void event_PrevNight()
{
    currentNight = currentNight - 1;
    if (currentNight < 0) currentNight += NUM_DAYS;
    reRenderCall = true;
    //printf("PREV NIGHT CLICKED!\n");
}
void event_NextNight()
{
    currentNight = currentNight + 1;
    if (currentNight >= NUM_DAYS) currentNight -= NUM_DAYS;
    reRenderCall = true;
    //printf("Next NIGHT CLICKED!\n");
}

void renderTextBox(SDL_Renderer *renderer, TextBox* tb)
{
    //Draw box
    if (tb->highlighted) SDL_SetRenderDrawColor(renderer, tb->highlightColor.r, tb->highlightColor.g, tb->highlightColor.b, tb->highlightColor.a);
    else SDL_SetRenderDrawColor(renderer, tb->boxColor.r, tb->boxColor.g, tb->boxColor.b, tb->boxColor.a);
    
    SDL_RenderFillRect(renderer, &tb->box);

    //Draw text
    SDL_Surface *surface = TTF_RenderText_Blended(tb->font, &(tb->text[0]), tb->textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {
        tb->box.x + (tb->box.w - surface->w) / 2,
        tb->box.y + (tb->box.h - surface->h) / 2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    //Free memory
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawUIElements(SDL_Renderer *renderer)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int uiZone = 0; uiZone < MAX_UI_ZONES; uiZone++)
    {
        for (int uiElement = 0; uiElement < COUNT[uiZone]; uiElement++)
        {
            TextBox* tb = &UI_ELEMENTS[uiZone][uiElement];
            renderTextBox(renderer, tb);
        }
    }
    for (int uiZone = 0; uiZone < MAX_UI_ZONES; uiZone++)
    {
        for (int uiElement = 0; uiElement < LINE_COUNT[uiZone]; uiElement++)
        {
            // Set draw color to white for the line
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            // Draw a line from (100, 100) to (700, 500)
            SDL_RenderDrawLine(renderer, UI_LINES[uiZone][uiElement][0], UI_LINES[uiZone][uiElement][1], UI_LINES[uiZone][uiElement][2], UI_LINES[uiZone][uiElement][3]);
        }
    }
    SDL_RenderPresent(renderer);
    
}

void resetScreen(int uiZone)
{
    COUNT[uiZone] = 0;
    LINE_COUNT[uiZone] = 0;
}

int addLine(
    int x, int y, int x2, int y2, 
    int uiZone
)
{
    if (LINE_COUNT[uiZone] >= MAX_UI_ELEMENTS) return -1;
    UI_LINES[uiZone][LINE_COUNT[uiZone]][0] = x;
    UI_LINES[uiZone][LINE_COUNT[uiZone]][1] = y;
    UI_LINES[uiZone][LINE_COUNT[uiZone]][2] = x2;
    UI_LINES[uiZone][LINE_COUNT[uiZone]][3] = y2;
    LINE_COUNT[uiZone]++;
}

int addTextBox(
    int x, int y, int width, int height, 
    Uint8 boxr, Uint8 boxg, Uint8 boxb, 
    Uint8 highlightboxr, Uint8 highlightboxg, Uint8 highlightboxb, 
    Uint8 textr, Uint8 textg, Uint8 textb, 
    const char * text, TTF_Font *FONT,
    EventFunction clickEvent,
    int eventID,
    int uiZone
)
{
    if (COUNT[uiZone] >= MAX_UI_ELEMENTS) return -1;

    TextBox* tb = &UI_ELEMENTS[uiZone][COUNT[uiZone]];
    tb->box.x = x;
    tb->box.y = y;
    tb->box.w = width;
    tb->box.h = height;
    
    tb->boxColor.r = boxr;
    tb->boxColor.g = boxg;
    tb->boxColor.b = boxb;
    tb->boxColor.a = 255;

    tb->highlightColor.r = highlightboxr;
    tb->highlightColor.g = highlightboxg;
    tb->highlightColor.b = highlightboxb;
    tb->highlightColor.a = 255;

    tb->textColor.r = textr;
    tb->textColor.g = textg;
    tb->textColor.b = textb;
    tb->textColor.a = 255;
    tb->font = FONT;

    snprintf(tb->text, STRING_BUFF_SIZE, "%s", text);

    tb->clickEventFunction = clickEvent;
    tb->eventID = eventID;
    COUNT[uiZone]++;
    return 1;
}

int isInBounds(TextBox* tb, int x, int y)
{
    return (tb->box.x <= x) && (x <= tb->box.x+tb->box.w) && (tb->box.y <= y) && (y <= tb->box.y+tb->box.h);
}

void updatedButtonInBounds(int mx, int my)
{
    for (int uiZone = 0; uiZone < MAX_UI_ZONES; uiZone++)
    {
        for (int uiElement = 0; uiElement < COUNT[uiZone]; uiElement++)
        {
            TextBox* tb = &UI_ELEMENTS[uiZone][uiElement];
            tb->highlighted = isInBounds(tb, mx, my);
        }
    }
}

void runButtonInBounds()
{
    for (int uiZone = 0; uiZone < MAX_UI_ZONES; uiZone++)
    {
        for (int uiElement = 0; uiElement < COUNT[uiZone]; uiElement++)
        {
            TextBox* tb = &UI_ELEMENTS[uiZone][uiElement];
            if (tb->highlighted && tb->clickEventFunction != NULL)
            {
                tb->clickEventFunction(tb->eventID);
            }
        }
    }
}



void viewSoloWorld(int eventID)
{
    soloWorldRole = eventID % NUM_BOTCT_ROLES;
    eventID /= NUM_BOTCT_ROLES;
    soloWorldPlayer = eventID;
    reRenderCall = true;
}

void viewProbWorld(int eventID)
{
    soloWorldPlayer = -1;
    soloWorldRole = -1;
    reRenderCall = true;
}

void makeTable(KnowledgeBase* kb, ProbKnowledgeBase* probkb, TTF_Font *FONT, int night)
{
    char buff[STRING_BUFF_SIZE];

    const int X_WIDTH = 40;
    const int Y_WIDTH = 15;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 50;

    int y = 50;
    int x = X_START;

    snprintf(buff, STRING_BUFF_SIZE, "ROLE TABLE [NIGHT%d]", night);

    //printf("buff: %s", buff);

    addTextBox(
        x, y, X_WIDTH*5, Y_WIDTH, //bb
        0, 0, 0, //Box colour
        0, 0, 0, //Highlighted Box colour
        255, 255, 255, //Text colour
        buff, 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP*5;
    addTextBox(
        x, y, X_WIDTH*2, Y_WIDTH, //bb
        50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "PREV NIGHT", 
        FONT,
        event_PrevNight,
        0,
        0
    );
    x += X_STEP*2;
    addTextBox(
        x, y, X_WIDTH*2, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "NEXT NIGHT", 
        FONT,
        event_NextNight,
        0,
        0
    );
    x = X_START;
    y += Y_STEP;


    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            int red;
            int green;
            int blue;
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0)
            {
                red = 255;
                green = 0;
                blue = 0;
            }
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0)
            {
                red = 255;
                green = 255;
                blue = 0;
            }
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0)
            {
                red = 255;
                green = 0;
                blue = 255;
            }
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0)
            {
                red = 0;
                green = 255;
                blue = 0;
            }
            snprintf(buff, STRING_BUFF_SIZE, "%.*s", 3, ROLE_NAMES[role]);
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                red, green, blue, //Text colour
                buff, 
                FONT,
                NULL,
                0,
                0
            );

            x += X_STEP;
        }
    }

    
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "CLASS", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "TEAM", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "POISONED", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "ALIVE", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;

    int set = 0;
    for (int element = 0; element < kb->SET_SIZES[set]; element++)
    {
        y += Y_STEP;
        x = X_START - X_STEP;
        
        snprintf(buff, STRING_BUFF_SIZE, "%s", kb->ELEMENT_NAMES[0][element]);
        addTextBox(
            x, y, X_WIDTH, Y_WIDTH, //bb
            0, 0, 0, //Box colour
            0, 0, 0, //Highlighted Box colour
            255, 255, 255, //Text colour
            buff, 
            FONT,
            NULL,
            0,
            0
        );
        
        x += X_STEP;
        snprintf(buff, 64, "is_TOWNSFOLK_[NIGHT%d]", night);
        int isTownsfolk = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_OUTSIDER_[NIGHT%d]", night);
        int isOutsider = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_MINION_[NIGHT%d]", night);
        int isMinion = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_DEMON_[NIGHT%d]", night);
        int isDemon = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_GOOD_[NIGHT%d]", night);
        int isGood = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_EVIL_[NIGHT%d]", night);
        int isEvil = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_POISONED_[NIGHT%d]", night);
        int isPoisoned = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_NOT_POISONED_[NIGHT%d]", night);
        int isNotPoisoned = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_ALIVE_[NIGHT%d]", night);
        int isAlive = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_DEAD_[NIGHT%d]", night);
        int isDead = isKnownName(kb, "PLAYERS", element, buff); 

        //printTrucatedStr(kb->ELEMENT_NAMES[0][element], 9);
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            //Only print roles in the script
            if (ROLE_IN_SCRIPT[role] == 1)
            {
                int red;
                int green;
                int blue;
                if (strcmp(ROLE_CLASSES[role], "DEMON") == 0)
                {
                    red = 255;
                    green = 0;
                    blue = 0;
                }
                else if (strcmp(ROLE_CLASSES[role], "MINION") == 0)
                {
                    red = 255;
                    green = 255;
                    blue = 0;
                }
                else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0)
                {
                    red = 255;
                    green = 0;
                    blue = 255;
                }
                else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0)
                {
                    red = 0;
                    green = 255;
                    blue = 0;
                }
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isNotRoleCertain = isKnownName(kb, "PLAYERS", element, buff);

                snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isRole = getProbIntPercentageName(probkb, kb, "PLAYERS", element, buff); 
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isNotRole = getProbIntPercentageName(probkb, kb, "PLAYERS", element, buff);
                
                if (isRole == 100) snprintf(buff, 64, " * ");
                else if (isNotRole == 100) snprintf(buff, 64, "   ");
                else snprintf(buff, 64, "%02d%%", isRole);

                if (isNotRoleCertain == 0)
                {
                    if (POSSIBLE_WORLD_GENERATED[element][role] == 1)
                    {
                        addTextBox(
                            x, y, X_WIDTH, Y_WIDTH, //bb
                            25+(isRole*2), 25+(isRole*2), 25+(isRole*2), //Box colour
                            250, 250, 250, //Highlighted Box colour
                            red, green, blue, //Text colour
                            buff, 
                            FONT,
                            viewSoloWorld,
                            role + (NUM_BOTCT_ROLES * element),
                            0
                        );
                    }
                    else
                    {
                        addTextBox(
                            x, y, X_WIDTH, Y_WIDTH, //bb
                            25+(isRole*2), 25+(isRole*2), 25+(isRole*2), //Box colour
                            25+(isRole*2), 25+(isRole*2), 25+(isRole*2), //Highlighted Box colour
                            red, green, blue, //Text colour
                            buff, 
                            FONT,
                            NULL,
                            0,
                            0
                        );
                    }
                }
                x += X_STEP;

            }
        }
        if (isTownsfolk == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "TOWNSFOLK", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isOutsider == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 255, //Text colour
                "OUTSIDER", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isMinion == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 0, //Text colour
                "MINION", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isDemon == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "DEMON", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }

        if (isGood == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "GOOD", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isEvil == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "EVIL", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        for (int poisonedPlayer = 0; poisonedPlayer < kb->SET_SIZES[set]; poisonedPlayer++)
        {
            snprintf(buff, 64, "POISONED_%d_[NIGHT%d]", poisonedPlayer, night);
            int didPoison = isKnownName(kb, "PLAYERS", element, buff); 
            if (didPoison)
            {
                addLine(X_START - X_STEP+20, y+(Y_STEP/2), x+20, 50+(Y_STEP*(poisonedPlayer+2))+(Y_STEP/2), 0);
            }
        }
        if (isPoisoned == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "POISONED", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isNotPoisoned == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "HEALTHY", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        if (isAlive == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "ALIVE", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isDead == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "DEAD", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
    }
}

void makeSingleWorldTable(KnowledgeBase* kb, TTF_Font *FONT, int night)
{
    char buff[STRING_BUFF_SIZE];

    const int X_WIDTH = 40;
    const int Y_WIDTH = 15;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 50;

    int y = 50;
    int x = X_START;

    snprintf(buff, STRING_BUFF_SIZE, "ROLE TABLE [NIGHT%d]", night);

    //printf("buff: %s", buff);

    addTextBox(
        x, y, X_WIDTH*5, Y_WIDTH, //bb
        0, 0, 0, //Box colour
        0, 0, 0, //Highlighted Box colour
        255, 255, 255, //Text colour
        buff, 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP*5;
    addTextBox(
        x, y, X_WIDTH*2, Y_WIDTH, //bb
        50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "PREV NIGHT", 
        FONT,
        event_PrevNight,
        0,
        0
    );
    x += X_STEP*2;
    addTextBox(
        x, y, X_WIDTH*2, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "NEXT NIGHT", 
        FONT,
        event_NextNight,
        0,
        0
    );
    x += X_STEP*2;
    addTextBox(
        x, y, X_WIDTH*2, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "BACK TO PROB", 
        FONT,
        viewProbWorld,
        0,
        0
    );
    x = X_START;
    y += Y_STEP;


    for (int role = 0; role < NUM_BOTCT_ROLES; role++)
    {
        //Only print roles in the script
        if (ROLE_IN_SCRIPT[role] == 1)
        {
            int red;
            int green;
            int blue;
            if (strcmp(ROLE_CLASSES[role], "DEMON") == 0)
            {
                red = 255;
                green = 0;
                blue = 0;
            }
            else if (strcmp(ROLE_CLASSES[role], "MINION") == 0)
            {
                red = 255;
                green = 255;
                blue = 0;
            }
            else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0)
            {
                red = 255;
                green = 0;
                blue = 255;
            }
            else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0)
            {
                red = 0;
                green = 255;
                blue = 0;
            }
            snprintf(buff, STRING_BUFF_SIZE, "%.*s", 3, ROLE_NAMES[role]);
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                red, green, blue, //Text colour
                buff, 
                FONT,
                NULL,
                0,
                0
            );

            x += X_STEP;
        }
    }

    
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "CLASS", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "TEAM", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "POISONED", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "ALIVE", 
        FONT,
        NULL,
        0,
        0
    );
    x += X_STEP;

    int set = 0;
    for (int element = 0; element < kb->SET_SIZES[set]; element++)
    {
        y += Y_STEP;
        x = X_START - X_STEP;
        snprintf(buff, STRING_BUFF_SIZE, "%s", kb->ELEMENT_NAMES[0][element]);
        addTextBox(
            x, y, X_WIDTH, Y_WIDTH, //bb
            0, 0, 0, //Box colour
            0, 0, 0, //Highlighted Box colour
            255, 255, 255, //Text colour
            buff, 
            FONT,
            NULL,
            0,
            0
        );
        
        x += X_STEP;
        snprintf(buff, 64, "is_TOWNSFOLK_[NIGHT%d]", night);
        int isTownsfolk = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_OUTSIDER_[NIGHT%d]", night);
        int isOutsider = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_MINION_[NIGHT%d]", night);
        int isMinion = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_DEMON_[NIGHT%d]", night);
        int isDemon = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_GOOD_[NIGHT%d]", night);
        int isGood = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_EVIL_[NIGHT%d]", night);
        int isEvil = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_POISONED_[NIGHT%d]", night);
        int isPoisoned = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_NOT_POISONED_[NIGHT%d]", night);
        int isNotPoisoned = isKnownName(kb, "PLAYERS", element, buff); 

        snprintf(buff, 64, "is_ALIVE_[NIGHT%d]", night);
        int isAlive = isKnownName(kb, "PLAYERS", element, buff); 
        snprintf(buff, 64, "is_DEAD_[NIGHT%d]", night);
        int isDead = isKnownName(kb, "PLAYERS", element, buff); 

        //printTrucatedStr(kb->ELEMENT_NAMES[0][element], 9);
        for (int role = 0; role < NUM_BOTCT_ROLES; role++)
        {
            //Only print roles in the script
            if (ROLE_IN_SCRIPT[role] == 1)
            {
                int red;
                int green;
                int blue;
                if (strcmp(ROLE_CLASSES[role], "DEMON") == 0)
                {
                    red = 255;
                    green = 0;
                    blue = 0;
                }
                else if (strcmp(ROLE_CLASSES[role], "MINION") == 0)
                {
                    red = 255;
                    green = 255;
                    blue = 0;
                }
                else if (strcmp(ROLE_CLASSES[role], "OUTSIDER") == 0)
                {
                    red = 255;
                    green = 0;
                    blue = 255;
                }
                else if (strcmp(ROLE_CLASSES[role], "TOWNSFOLK") == 0)
                {
                    red = 0;
                    green = 255;
                    blue = 0;
                }
                snprintf(buff, 64, "is_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isRole = isKnownName(kb, "PLAYERS", element, buff);

                if (isRole)
                {
                    addTextBox(
                        x, y, X_WIDTH, Y_WIDTH, //bb
                        50, 50, 150, //Box colour
                        250, 250, 250, //Highlighted Box colour
                        red, green, blue, //Text colour
                        " * ", 
                        FONT,
                        NULL,
                        0,
                        0
                    );
                }
                x += X_STEP;

            }
        }
        if (isTownsfolk == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "TOWNSFOLK", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isOutsider == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 255, //Text colour
                "OUTSIDER", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isMinion == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 0, //Text colour
                "MINION", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isDemon == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "DEMON", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }

        if (isGood == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "GOOD", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isEvil == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "EVIL", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        for (int poisonedPlayer = 0; poisonedPlayer < kb->SET_SIZES[set]; poisonedPlayer++)
        {
            snprintf(buff, 64, "POISONED_%d_[NIGHT%d]", poisonedPlayer, night);
            int didPoison = isKnownName(kb, "PLAYERS", element, buff); 
            if (didPoison)
            {
                addLine(X_START - X_STEP+20, y+(Y_STEP/2), x+20, 50+(Y_STEP*(poisonedPlayer+2))+(Y_STEP/2), 0);
            }
        }
        if (isPoisoned == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "POISONED", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isNotPoisoned == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "HEALTHY", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        if (isAlive == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "ALIVE", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else if (isDead == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 0, 0, //Text colour
                "DEAD", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
        else
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                255, 255, 255, //Text colour
                "?", 
                FONT,
                NULL,
                0,
                0
            );
            x += X_STEP;
        }
    }
}


void updateUITable(KnowledgeBase* kb, ProbKnowledgeBase* probKB, TTF_Font *FONT, int night)
{
    currentNight = night;
    resetScreen(0);
    if (soloWorldPlayer == -1 || soloWorldRole == -1)
    {
        makeTable(kb, probKB, FONT, night);
    }
    else
    {
        makeSingleWorldTable(POSSIBLE_WORLDS_FOR_PROB[soloWorldPlayer][soloWorldRole], FONT, night);
    }
}

void getButtonColours(bool selected, int* r, int* g, int* b, int* sr, int* sg, int* sb)
{
    const int normR = 50;
    const int normG = 50;
    const int normB = 50;

    const int normselectedR = 100;
    const int normselectedG = 100;
    const int normselectedB = 100;

    const int openR = 150;
    const int openG = 150;
    const int openB = 150;

    if (selected)
    {
        *r = openR;
        *g = openG;
        *b = openB;
        *sr = openR;
        *sg = openG;
        *sb = openB;

    }
    else
    {
        *r = normR;
        *g = normG;
        *b = normB;
        *sr = normselectedR;
        *sg = normselectedG;
        *sb = normselectedB;
    }   
}

void resetSubMenu()
{
    subMenuOpen = 0;
}
void resetSubSubMenu()
{
    subSubMenuOpen = 0;
}
void resetSubSubSubMenu()
{
    subSubSubMenuOpen = 0;
}
void resetSubSubSubSubMenu()
{
    for (int i = 0; i < MAX_BUTTON_OPTIONS; i++) subSubSubSubMenuSelected[i] = 0;
}
void resetSubSubSubSubSubMenu()
{
    for (int i = 0; i < MAX_BUTTON_OPTIONS*3; i++) subSubSubSubSubMenuSelected[i] = 0;
}

void openSubMenu(int eventID)
{
    subMenuOpen = eventID;
    reRenderCall = true;

    resetSubSubMenu();
    resetSubSubSubMenu();
    resetSubSubSubSubMenu();
    resetSubSubSubSubSubMenu();
}
void openSubSubMenu(int eventID)
{
    subSubMenuOpen = eventID;
    reRenderCall = true;

    resetSubSubSubMenu();
    resetSubSubSubSubMenu();
    resetSubSubSubSubSubMenu();
}
void openSubSubSubMenu(int eventID)
{
    subSubSubMenuOpen = eventID;
    reRenderCall = true;

    resetSubSubSubSubMenu();
    resetSubSubSubSubSubMenu();
}
void toggleSubSubSubSubMenu(int eventID)
{
    subSubSubSubMenuSelected[eventID] = subSubSubSubMenuSelected[eventID] == 1 ? 0 : 1;
    reRenderCall = true;
    resetSubSubSubSubSubMenu();
}
void selectSubSubSubSubMenu(int eventID)
{
    for (int i = 0; i < MAX_BUTTON_OPTIONS; i++) subSubSubSubMenuSelected[i] = 0;
    subSubSubSubMenuSelected[eventID] = 1;
    reRenderCall = true;
    resetSubSubSubSubSubMenu();
}
void toggleSubSubSubSubSubMenu(int eventID)
{
    subSubSubSubSubMenuSelected[eventID] = subSubSubSubSubMenuSelected[eventID] == 1 ? 0 : 1;
    reRenderCall = true;
}
void selectSubSubSubSubSubMenu(int eventID)
{
    int range = (eventID / MAX_BUTTON_OPTIONS) * MAX_BUTTON_OPTIONS;
    for (int i = range; i < range+MAX_BUTTON_OPTIONS; i++) subSubSubSubSubMenuSelected[i] = 0;
    subSubSubSubSubMenuSelected[eventID] = 1;
    reRenderCall = true;
}

void finish()
{
    int contradiction = inferImplicitFacts(KNOWLEDGE_BASE, RULE_SET, NUM_SOLVE_STEPS, 1);
    
    if (contradiction == 0)
    { //Optimise ruleset only if no contradictions were produced
        //printHeading("OPTIMISE RULESET");
        optimiseRuleset(RULE_SET, KNOWLEDGE_BASE);
        copyTo(REVERT_KB, KNOWLEDGE_BASE);
    }
        

    if (contradiction == 1)
    {
        //printRedHeading("CONTRADICTION FOUND!"); //UI HEADING
        //printf("Rolling back Knowledge base\n");
        //Roll back knowledge base
        copyTo(KNOWLEDGE_BASE, REVERT_KB);
    }

    if (contradiction == 0)
    {
        //Compute total tally
        resetProbKnowledgeBase(WORLD_TALLY);
        WORLD_GENERATION++;

        for (int i = 0; i < MAX_SET_ELEMENTS; i++)
        {
            for (int j = 0; j < NUM_BOTCT_ROLES; j++)
            {
                POSSIBLE_WORLD_GENERATED[i][j] = 0;
            }
        }
    }
    
    //printf("FINISHED CONFIRM!\n");
    reRenderCall = true;
}

void confirm()
{
    int night;
    int playerID;
    int playerIDs[KNOWLEDGE_BASE->SET_SIZES[0]];
    int roleID;
    int count;
    int count2;
    int count3;
    int roleIDs[NUM_BOTCT_ROLES];
    int mode;
    switch(subMenuOpen)
    {
        case 1: //Shown Role
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1)
                {
                    roleID = i-1;
                    break;
                }
            }
            shown_role(KNOWLEDGE_BASE, playerID, roleID, night);
            break;
        case 2: //Player Possibilities
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            count = 0;
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1)
                {
                    roleIDs[count] = i-1;
                    count++;
                }
            }
            noptions(KNOWLEDGE_BASE, playerID, count, roleIDs, night);
            break;
        case 3: //Role not In Game
            night = subSubMenuOpen-1;
            roleID = subSubSubMenuOpen-1;
            roleNotInGame(KNOWLEDGE_BASE, roleID, night);
            break;
        case 4: //PLayer is/is not poisoned
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            if (subSubSubSubMenuSelected[1] == 1)
            {
                poisoned(KNOWLEDGE_BASE, playerID, night);
            }
            if (subSubSubSubMenuSelected[2] == 1)
            {
                notPoisoned(KNOWLEDGE_BASE, playerID, night);
            }
            break;
        case 5: //Redherring 
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            if (subSubSubSubMenuSelected[1] == 1)
            {
                redHerring(KNOWLEDGE_BASE, playerID);
            }

            break;
        case 6: //Num deaths / Ressurections
            night = subSubMenuOpen-1;
            mode = subSubSubMenuOpen;
            count = 0;
            for (int i = 0; i < KNOWLEDGE_BASE->SET_SIZES[0]+1; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1)
                {
                    playerIDs[count] = i-1;
                    count++;
                }
            }
            if (mode == 1) diedInNight(KNOWLEDGE_BASE, count, playerIDs, night);
            else if (mode == 2) nominationDeath(KNOWLEDGE_BASE, count, playerIDs, night);
            else if (mode == 3) hung(KNOWLEDGE_BASE, count, playerIDs, night);
            else if (mode == 4) resurrected(KNOWLEDGE_BASE, count, playerIDs, night);
            break;
        case 7: //Player Ping
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1)
                {
                    mode = i;
                    break;
                }
            }
            count = 0;
            count2 = 0;
            count3 = 0;
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubSubMenuSelected[i] == 1)
                {
                    roleIDs[count] = i-1;
                    count++;
                }
            }
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubSubMenuSelected[i+MAX_BUTTON_OPTIONS] == 1)
                {
                    playerIDs[count2] = i-1;
                    count2++;
                }
            }
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubSubMenuSelected[i+(MAX_BUTTON_OPTIONS*2)] == 1)
                {
                    count3 = i-1;
                    break;
                }
            }
            if (mode == 1)
            { //washerwoman
                printf("washerwoman\n");
                washerWomanPing(playerID, roleIDs[0], playerIDs[0], playerIDs[1], KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 2)
            { //librarian
                printf("librarian\n");
                librarianPing(playerID, roleIDs[0], playerIDs[0], playerIDs[1], KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 3)
            { //investigator
                printf("investigator\n");
                investigatorPing(playerID, roleIDs[0], playerIDs[0], playerIDs[1], KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 4)
            { //chef
                printf("chef\n");
                chefPing(playerID, count3, KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 5)
            { //empath
                printf("empath\n");
                empathPing(playerID, count3, night, KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 6)
            { //fortune teller
                printf("fortune teller\n");
                fortuneTellerPing(playerID, count3, playerIDs[0], playerIDs[1], night, KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 7)
            { //undertaker
                printf("undertaker\n");
                undertakerPing(playerID, roleIDs[0], playerIDs[0], night, KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 8)
            { //monk
                printf("monk\n");
                monkPing(playerID, playerIDs[0], night, KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 9)
            { //ravenkeeper
                printf("ravenkeeper\n");
                ravenkeeperPing(playerID, roleIDs[0], playerIDs[0], night, KNOWLEDGE_BASE, RULE_SET);
            }
            else if (mode == 10)
            { //clockmaker
                printf("clockmaker\n");
            }
            else if (mode == 11)
            { //dreamer
                printf("dreamer\n");
            }
            else if (mode == 12)
            { //snake charmer
                printf("snake charmer\n");
            }
            else if (mode == 13)
            { //mathematician
                printf("mathematician\n");
            }
            else if (mode == 14)
            { //flowergirl
                printf("flowergirl\n");
            }
            else if (mode == 15)
            { //towncrier
                printf("towncrier\n");
            }
            else if (mode == 16)
            { //oracle
                printf("oracle\n");
            }
            else if (mode == 17)
            { //savant
                printf("savant\n");
            }
            else if (mode == 18)
            { //seamstress
                printf("\n");
            }
            else if (mode == 19)
            { //philosopher
                printf("\n");
            }
            else if (mode == 20)
            { //artist
                printf("\n");
            }
            else if (mode == 21)
            { //juggler
                printf("\n");
            }
            else if (mode == 22)
            { //sage
                printf("\n");
            }
            else if (mode == 23)
            { //grandmother
                printf("\n");
            }
            else if (mode == 24)
            { //chambermaid
                printf("\n");
            }
            else if (mode == 25)
            { //exorcist
                printf("\n");
            }
            else if (mode == 26)
            { //innkeeper
                printf("\n");
            }
            else if (mode == 27)
            { //gambler
                printf("\n");
            }
            else if (mode == 28)
            { //gossip
                printf("\n");
            }
            else if (mode == 29)
            { //courtier
                printf("\n");
            }
            else if (mode == 30)
            { //professor
                printf("\n");
            }
            else if (mode == 31)
            { //minstrel
                printf("\n");
            }
            break;
        case 8: //reset Data
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            reset(KNOWLEDGE_BASE, playerID);
            resetMetaData(KNOWLEDGE_BASE);
            break;
        case 9: //kill player
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            count = 0;
            for (int i = 0; i < KNOWLEDGE_BASE->SET_SIZES[0]+1; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1)
                {
                    playerIDs[count] = i-1;
                    count++;
                }
            }
            killedPlayer(KNOWLEDGE_BASE, playerID, playerIDs[0], night);
            break;
        case 10: //poison player
            night = subSubMenuOpen-1;
            playerID = subSubSubMenuOpen-1;
            count = 0;
            for (int i = 0; i < KNOWLEDGE_BASE->SET_SIZES[0]+1; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1)
                {
                    playerIDs[count] = i-1;
                    count++;
                }
            }
            hasPoisoned(KNOWLEDGE_BASE, playerID, playerIDs[0], night);
            break;
        default:
            break;

    }

    finish();
    reRenderCall = true;
}

int canConfirm()
{
    int FAIL = 0;
    int SUCCESS = 1;
    switch(subMenuOpen)
    {
        case 1: //Shown Role
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1) break;
                if (i == MAX_BUTTON_OPTIONS-1) return FAIL;
            }
            return SUCCESS;
        case 2: //Player Possibilities
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1) break;
                if (i == MAX_BUTTON_OPTIONS-1) return FAIL;
            }
            return SUCCESS;
        case 3: //Role not In Game
            return subSubSubMenuOpen != 0;
        case 4: //PLayer is/is not poisoned
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1) break;
                if (i == MAX_BUTTON_OPTIONS-1) return FAIL;
            }
            return SUCCESS;
        case 5: //Redherring 
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1) break;
                if (i == MAX_BUTTON_OPTIONS-1) return FAIL;
            }
            return SUCCESS;
        case 6: //Num deaths / Ressurections
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1) break;
                if (i == MAX_BUTTON_OPTIONS-1) return FAIL;
            }
            return SUCCESS;
        case 7: //Player Ping
            for (int i = 0; i < MAX_BUTTON_OPTIONS*3; i++)
            {
                if (subSubSubSubSubMenuSelected[i] == 1) break;
                if (i+1 == MAX_BUTTON_OPTIONS*3) return FAIL;
            }
            return SUCCESS;
        case 8: //reset Data
            return subSubSubMenuOpen != 0;
        case 9: //Kill player
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1) break;
                if (i == MAX_BUTTON_OPTIONS-1) return FAIL;
            }
            return SUCCESS;
        case 10: //Poison player
            for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
            {
                if (subSubSubSubMenuSelected[i] == 1) break;
                if (i == MAX_BUTTON_OPTIONS-1) return FAIL;
            }
            return SUCCESS;
        default:
            break;

    }
    return FAIL;
}   

void updateConfirmButton(TTF_Font *FONT, KnowledgeBase* kb)
{
    //UI Zone for this menu
    const int MY_UI_ZONE = 6;
    //reset menu UI zone
    resetScreen(MY_UI_ZONE);
    char buff[STRING_BUFF_SIZE];

    const int X_WIDTH = 256;
    const int Y_WIDTH = 40;


    const int X_START = 50;

    int Y_START = 440;

    int y = Y_START;
    int x = X_START;

    int red, green, blue;
    int selectedRed, selectedGreen, selectedBlue;

    int count = 1;

    if (canConfirm())
    {
        addTextBox(
            x, y, X_WIDTH, Y_WIDTH, //bb
            50, 150, 50, //Box colour
            100, 200, 100, //Highlighted Box colour
            255, 255, 255, //Text colour
            "CONFIRM", 
            FONT,
            confirm,
            0,
            MY_UI_ZONE
        );
    }
}

void updateSubSubSubSubMenu(TTF_Font *FONT, KnowledgeBase* kb)
{
    //UI Zone for this menu
    const int MY_UI_ZONE = 5;
    //reset menu UI zone
    resetScreen(MY_UI_ZONE);
    char buff[STRING_BUFF_SIZE];

    //If the previous menu doesn't have a selected menu open
    for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
    {
        if (subSubSubSubMenuSelected[i] == 1) break;
        if (i == MAX_BUTTON_OPTIONS-1) return;
    }

    //printf("DRAWING SUB MENU\n");

    const int X_WIDTH = 128;
    const int Y_WIDTH = 20;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 530+315;

    int Y_START = 500;

    int y = Y_START;
    int x = X_START;

    int red, green, blue;
    int selectedRed, selectedGreen, selectedBlue;

    int count = 1;

    int haveRoles = 0;
    int havePlayers = 0;
    int haveNumbers = 0;
    char* roleDescription = "";
    int mode = 0;
    for (int i = 0; i < MAX_BUTTON_OPTIONS; i++)
    {
        if (subSubSubSubMenuSelected[i] == 1)
        {
            mode = i;
            break;
        }
    }

    if (subMenuOpen == 7)
    {
        if (mode == 1)
        { //washerwoman
            haveRoles = 1;
            havePlayers = 2;
            haveNumbers = 0;
            roleDescription = "You start knowing that 1 of 2 players is a particular Townsfolk.";
        }
        else if (mode == 2)
        { //librarian
            haveRoles = 1;
            havePlayers = 2;
            haveNumbers = 0;
            roleDescription = "You start knowing that 1 of 2 players is a particular Outsider. (Or that zero are in play.)";
        }
        else if (mode == 3)
        { //investigator
            haveRoles = 1;
            havePlayers = 2;
            haveNumbers = 0;
            roleDescription = "You start knowing that 1 of 2 players is a particular Minion.";
        }
        else if (mode == 4)
        { //chef
            haveRoles = 0;
            havePlayers = 0;
            haveNumbers = 4;
            roleDescription = "You start knowing how many pairs of evil players there are.";
        }
        else if (mode == 5)
        { //empath
            haveRoles = 0;
            havePlayers = 0;
            haveNumbers = 2;
            roleDescription = "Each night, you learn how many of your 2 alive neighbors are evil.";
        }
        else if (mode == 6)
        { //fortune teller
            haveRoles = 0;
            havePlayers = 2;
            haveNumbers = 1;
            roleDescription = "Each night, choose 2 players: you learn if either is a Demon. There is a good player that registers as a Demon to you.";
        }
        else if (mode == 7)
        { //undertaker
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "Each night, you learn which character died by execution today.";
        }
        else if (mode == 8)
        { //monk
            haveRoles = 0;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "Each night*, choose a player (not yourself): they are safe from the Demon tonight.";
        }
        else if (mode == 9)
        { //ravenkeeper
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "If you die at night, you are woken to choose a player: you learn their character.";
        }
        else if (mode == 10)
        { //clockmaker
            haveRoles = 0;
            havePlayers = 0;
            haveNumbers = ((kb->SET_SIZES[0])/2)+1;
            roleDescription = "You start knowing how many steps from the Demon to its nearest Minion.";
        }
        else if (mode == 11)
        { //dreamer
            haveRoles = 2;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "Each night, choose a player (not yourself, or Travellers): you learn 1 good character & 1 evil character, 1 of which is correct.";
        }
        else if (mode == 12)
        { //snake charmer
            haveRoles = 0;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "Each night, choose an alive player: a chosen Demon swaps characters & alignments with you & is then poisoned.";
        }
        else if (mode == 13)
        { //mathematician
            haveRoles = 0;
            havePlayers = 0;
            haveNumbers = kb->SET_SIZES[0];
            roleDescription = "Each night, you learn how many players' abilities worked abnormally (since dawn) due to another character's ability.";
        }
        else if (mode == 14)
        { //flowergirl
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "Each night*, you learn if the Demon voted today.";
        }
        else if (mode == 15)
        { //towncrier
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "Each night*, you learn if a Minion nominated today.";
        }
        else if (mode == 16)
        { //oracle
            haveRoles = 0;
            havePlayers = 0;
            haveNumbers = kb->SET_SIZES[0];
            roleDescription = "Each night*, you learn how many dead players are evil.";
        }
        else if (mode == 17)
        { //savant
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "Each day, you may visit the Storyteller to learn 2 things in private: 1 is true & 1 is false.";
        }
        else if (mode == 18)
        { //seamstress
            haveRoles = 0;
            havePlayers = 2;
            haveNumbers = 1;
            roleDescription = "Once per game, at night, choose 2 players (not yourself): you learn if they are the same alignment.";
        }
        else if (mode == 19)
        { //philosopher
            haveRoles = 1;
            havePlayers = 0;
            haveNumbers = 0;
            roleDescription = "Once per game, at night, choose a good character: gain that ability. If this character is in play, they are drunk.";
        }
        else if (mode == 20)
        { //artist
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "Once per game, during the day, privately ask the Storyteller any yes/ no question.";
        }
        else if (mode == 21)
        { //juggler
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "On your 1st day, publicly guess up to 5 players' characters. That night, you learn how many you got correct.";
        }
        else if (mode == 22)
        { //sage
            haveRoles = 0;
            havePlayers = 2;
            haveNumbers = 0;
            roleDescription = "If the Demon kills you, you learn that it is 1 of 2 players.";
        }
        else if (mode == 23)
        { //grandmother
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "You start knowing a good player & their character. If the Demon kills them, you die too.";
        }
        else if (mode == 24)
        { //chambermaid
            haveRoles = 0;
            havePlayers = 2;
            haveNumbers = 2;
            roleDescription = "Each night, choose 2 alive players (not yourself): you learn how many woke tonight due to their ability.";
        }
        else if (mode == 25)
        { //exorcist
            haveRoles = 0;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "Each night*, choose a player (different to last night): the Demon, if chosen, learns who you are & doesn't wake tonight.";
        }
        else if (mode == 26)
        { //innkeeper
            haveRoles = 0;
            havePlayers = 2;
            haveNumbers = 0;
            roleDescription = "Each night*, choose 2 players: they can't die tonight, but 1 is drunk until dusk.";
        }
        else if (mode == 27)
        { //gambler
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "Each night*, choose a player & guess their character: if you guess wrong, you die.";
        }
        else if (mode == 28)
        { //gossip
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "Each day, you may make a public statement. Tonight, if it was true, a player dies.";
        }
        else if (mode == 29)
        { //courtier
            haveRoles = 0;
            havePlayers = 1;
            haveNumbers = 0;
            roleDescription = "Once per game, at night, choose a character: they are drunk for 3 nights & 3 days.";
        }
        else if (mode == 30)
        { //professor
            haveRoles = 0;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "Once per game, at night*, choose a dead player: if they are a Townsfolk, they are resurrected.";
        }
        else if (mode == 31)
        { //minstrel
            haveRoles = 1;
            havePlayers = 1;
            haveNumbers = 1;
            roleDescription = "When a Minion dies by execution, all other players (except Travellers) are drunk until dusk tomorrow.";
        }

        addTextBox(
            x, y-Y_STEP, X_WIDTH*4, Y_WIDTH, //bb
            0, 0, 0, //Box colour
            0, 0, 0, //Highlighted Box colour
            255, 255, 255, //Text colour
            roleDescription, 
            FONT,
            NULL,
            0,
            MY_UI_ZONE
        );

        //printf("DRAWING SUB MENU YAY\n");
        if (haveRoles)
        {
            //Drawing roles
            for (int role = 0; role < NUM_BOTCT_ROLES; role++)
            {
                if (ROLE_IN_SCRIPT[role])
                {
                    getButtonColours(subSubSubSubSubMenuSelected[role+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                    snprintf(buff, STRING_BUFF_SIZE, "Role: %s", ROLE_NAMES[role]);
                    addTextBox(
                        x, y, X_WIDTH, Y_WIDTH, //bb
                        red, green, blue, //Box colour
                        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                        255, 255, 255, //Text colour
                        buff, 
                        FONT,
                        selectSubSubSubSubSubMenu,
                        role+1,
                        MY_UI_ZONE
                    );
                    y += Y_STEP;

                    if (y > HEIGHT-Y_STEP-10)
                    {
                        y = Y_START;
                        x += X_STEP;
                    }
                }
            }
            x += X_STEP;
        }
        y = Y_START;
        if (havePlayers)
        {
            //Drawing players
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubSubSubMenuSelected[MAX_BUTTON_OPTIONS+player+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    toggleSubSubSubSubSubMenu,
                    MAX_BUTTON_OPTIONS+player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            x += X_STEP;
        }
        y = Y_START;
        if (haveNumbers)
        {
            //Drawing count
            for (int count = 0; count <= haveNumbers; count++)
            {
                getButtonColours(subSubSubSubSubMenuSelected[(MAX_BUTTON_OPTIONS*2)+count+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "%d", count);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    selectSubSubSubSubSubMenu,
                    (MAX_BUTTON_OPTIONS*2)+count+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
        }
    }


}

void updateSubSubSubMenu(TTF_Font *FONT, KnowledgeBase* kb)
{
    //UI Zone for this menu
    const int MY_UI_ZONE = 4;
    //reset menu UI zone
    resetScreen(MY_UI_ZONE);
    char buff[STRING_BUFF_SIZE];

    //If the previous menu doesn't have a selected menu open
    if (subSubSubMenuOpen == 0) return;

    const int X_WIDTH = 128;
    const int Y_WIDTH = 20;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 530;

    int Y_START = 500;

    int y = Y_START;
    int x = X_START;

    int red, green, blue;
    int selectedRed, selectedGreen, selectedBlue;

    int count = 1;

    switch(subMenuOpen)
    {
        case 1: //Shown Role
            for (int role = 0; role < NUM_BOTCT_ROLES; role++)
            {
                if (ROLE_IN_SCRIPT[role])
                {
                    getButtonColours(subSubSubSubMenuSelected[role+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                    snprintf(buff, STRING_BUFF_SIZE, "Role: %s", ROLE_NAMES[role]);
                    addTextBox(
                        x, y, X_WIDTH, Y_WIDTH, //bb
                        red, green, blue, //Box colour
                        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                        255, 255, 255, //Text colour
                        buff, 
                        FONT,
                        selectSubSubSubSubMenu,
                        role+1,
                        MY_UI_ZONE
                    );
                    y += Y_STEP;

                    if (y > HEIGHT-Y_STEP-10)
                    {
                        y = Y_START;
                        x += X_STEP;
                    }
                }
            }
            break;
        case 2: //Player Possibilities
            for (int role = 0; role < NUM_BOTCT_ROLES; role++)
            {
                if (ROLE_IN_SCRIPT[role])
                {
                    getButtonColours(subSubSubSubMenuSelected[role+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                    snprintf(buff, STRING_BUFF_SIZE, "Role: %s", ROLE_NAMES[role]);
                    addTextBox(
                        x, y, X_WIDTH, Y_WIDTH, //bb
                        red, green, blue, //Box colour
                        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                        255, 255, 255, //Text colour
                        buff, 
                        FONT,
                        toggleSubSubSubSubMenu,
                        role+1,
                        MY_UI_ZONE
                    );
                    y += Y_STEP;

                    if (y > HEIGHT-Y_STEP-10)
                    {
                        y = Y_START;
                        x += X_STEP;
                    }
                }
            }
            break;
        case 3: //Role not In Game
            //MENU NOT NEEDED
            break;
        case 4: //PLayer is/is not poisoned
            getButtonColours(subSubSubSubMenuSelected[1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Poisoned");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                1,
                MY_UI_ZONE
            );
            y += Y_STEP;
            getButtonColours(subSubSubSubMenuSelected[2] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Healthy");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                2,
                MY_UI_ZONE
            );
            break;
        case 5: //Redherring 
            getButtonColours(subSubSubSubMenuSelected[1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Red Herring");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                1,
                MY_UI_ZONE
            );
            y += Y_STEP;
            getButtonColours(subSubSubSubMenuSelected[2] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Not Red Herring");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                2,
                MY_UI_ZONE
            );
            break;
        case 6: //Num deaths / Ressurections
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubSubMenuSelected[player+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    toggleSubSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            getButtonColours(subSubSubSubMenuSelected[kb->SET_SIZES[0]+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "NO ONE");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                kb->SET_SIZES[0]+1,
                MY_UI_ZONE
            );
            break;
        case 7: //Player Ping

        
            count = 1;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "WASHERWOMAN");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "LIBRARIAN");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "INVESTIGATOR");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "CHEF");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "EMPATH");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "FORTUNE_TELLER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "UNDERTAKER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "MONK");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "RAVENKEEPER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;


            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "CLOCKMAKER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "DREAMER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "SNAKE_CHARMER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "MATHEMATICIAN");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "FLOWERGIRL");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "TOWN_CRIER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y = Y_START;
            x += X_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "ORACLE");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "SAVANT");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "SEAMSTRESS");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "PHILOSOPHER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "ARTIST");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "JUGGLER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "SAGE");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;


            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "GRANDMOTHER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "CHAMBERMAID");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "EXORCIST");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "INNKEEPER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "GAMBLER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "GOSSIP");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "COURTIER");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "PROFESSOR");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            getButtonColours(subSubSubSubMenuSelected[count] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "MINSTREL");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                selectSubSubSubSubMenu,
                count,
                MY_UI_ZONE
            );
            y += Y_STEP;
            count++;
            break;
        case 8: //reset Data
            //NOTHING TO DO HERE
            break;
        case 9: //Kill player
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubSubMenuSelected[player+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    selectSubSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        case 10: //Poison player
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubSubMenuSelected[player+1] == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    selectSubSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        default:
            break;

    }
}

void updateSubSubMenu(TTF_Font *FONT, KnowledgeBase* kb)
{
    //UI Zone for this menu
    const int MY_UI_ZONE = 3;
    //reset menu UI zone
    resetScreen(MY_UI_ZONE);
    char buff[STRING_BUFF_SIZE];

    //If the previous menu doesn't have a selected menu open
    if (subSubMenuOpen == 0) return;

    const int X_WIDTH = 128;
    const int Y_WIDTH = 20;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 370;

    int Y_START = 500;

    int y = Y_START;
    int x = X_START;

    int red, green, blue;
    int selectedRed, selectedGreen, selectedBlue;

    switch(subMenuOpen)
    {
        case 1: //Shown Role
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;

        case 2: //Player Possibilities
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;

        case 3: //Role not In Game
            for (int role = 0; role < NUM_BOTCT_ROLES; role++)
            {
                if (ROLE_IN_SCRIPT[role])
                {
                    getButtonColours(subSubSubMenuOpen == role+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                    snprintf(buff, STRING_BUFF_SIZE, "Role: %s", ROLE_NAMES[role]);
                    addTextBox(
                        x, y, X_WIDTH, Y_WIDTH, //bb
                        red, green, blue, //Box colour
                        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                        255, 255, 255, //Text colour
                        buff, 
                        FONT,
                        openSubSubSubMenu,
                        role+1,
                        MY_UI_ZONE
                    );
                    y += Y_STEP;

                    if (y > HEIGHT-Y_STEP-10)
                    {
                        y = Y_START;
                        x += X_STEP;
                    }
                }
            }
            break;
        case 4: //PLayer is/is not poisoned
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        case 5: //Redherring 
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        case 6: //Num deaths / Ressurections
            getButtonColours(subSubSubMenuOpen == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Night Death");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                openSubSubSubMenu,
                1,
                MY_UI_ZONE
            );
            y += Y_STEP;
            getButtonColours(subSubSubMenuOpen == 2, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Day Death");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                openSubSubSubMenu,
                2,
                MY_UI_ZONE
            );
            y += Y_STEP;
            getButtonColours(subSubSubMenuOpen == 3, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Hanging Death");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                openSubSubSubMenu,
                3,
                MY_UI_ZONE
            );
            y += Y_STEP;
            getButtonColours(subSubSubMenuOpen == 4, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
            snprintf(buff, STRING_BUFF_SIZE, "Resurrection");
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                red, green, blue, //Box colour
                selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                255, 255, 255, //Text colour
                buff, 
                FONT,
                openSubSubSubMenu,
                4,
                MY_UI_ZONE
            );
            break;
        case 7: //Player Ping
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        case 8: //reset Data
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        case 9: //kill player
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        case 10: //poison player
            for (int player = 0; player < kb->SET_SIZES[0]; player++)
            {
                getButtonColours(subSubSubMenuOpen == player+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
                snprintf(buff, STRING_BUFF_SIZE, "Player: %s", kb->ELEMENT_NAMES[0][player]);
                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    red, green, blue, //Box colour
                    selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
                    255, 255, 255, //Text colour
                    buff, 
                    FONT,
                    openSubSubSubMenu,
                    player+1,
                    MY_UI_ZONE
                );
                y += Y_STEP;
            }
            break;
        default:
            break;

    }
}

void updateSubMenu(TTF_Font *FONT, KnowledgeBase* kb)
{
    
    //UI Zone for this menu
    const int MY_UI_ZONE = 2;
    //reset menu UI zone
    resetScreen(MY_UI_ZONE);
    char buff[STRING_BUFF_SIZE];

    //If the previous menu doesn't have a selected menu open
    if (subMenuOpen == 0) return;

    const int X_WIDTH = 128;
    const int Y_WIDTH = 20;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 210;

    int Y_START = 500;

    int y = Y_START;
    int x = X_START;

    int red, green, blue;
    int selectedRed, selectedGreen, selectedBlue;

    for (int night = 0; night < NUM_DAYS; night++)
    {
        getButtonColours(subSubMenuOpen == night+1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
        snprintf(buff, STRING_BUFF_SIZE, "Night: %d", night);
        addTextBox(
            x, y, X_WIDTH, Y_WIDTH, //bb
            red, green, blue, //Box colour
            selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
            255, 255, 255, //Text colour
            buff, 
            FONT,
            openSubSubMenu,
            night+1,
            MY_UI_ZONE
        );
        y += Y_STEP;
    }
}

void updateFirstMenu(TTF_Font *FONT)
{
    //UI Zone for this menu
    const int MY_UI_ZONE = 1;
    //reset menu UI zone
    resetScreen(MY_UI_ZONE);
    char buff[STRING_BUFF_SIZE];

    const int X_WIDTH = 128;
    const int Y_WIDTH = 20;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 50;

    int y = 500;
    int x = X_START;

    int red, green, blue;
    int selectedRed, selectedGreen, selectedBlue;

    

    getButtonColours(subMenuOpen == 1, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "SHOWN ROLE", 
        FONT,
        openSubMenu,
        1,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 2, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "PLAYER POSSIBBILITIES", 
        FONT,
        openSubMenu,
        2,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 3, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "ROLE NOT IN GAME", 
        FONT,
        openSubMenu,
        3,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 4, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "POISONED/HEALTHY", 
        FONT,
        openSubMenu,
        4,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 5, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "RED HERRING", 
        FONT,
        openSubMenu,
        5,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 6, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "DEATHS / RES", 
        FONT,
        openSubMenu,
        6,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 7, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "PLAYER PINGS", 
        FONT,
        openSubMenu,
        7,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 8, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "RESET DATA", 
        FONT,
        openSubMenu,
        8,
        MY_UI_ZONE
    );
    y += Y_STEP;
    
    getButtonColours(subMenuOpen == 9, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "KILL PLAYER", 
        FONT,
        openSubMenu,
        9,
        MY_UI_ZONE
    );
    y += Y_STEP;

    getButtonColours(subMenuOpen == 10, &red, &green, &blue, &selectedRed, &selectedGreen, &selectedBlue);
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        red, green, blue, //Box colour
        selectedRed, selectedGreen, selectedBlue, //Highlighted Box colour
        255, 255, 255, //Text colour
        "POISON PLAYER", 
        FONT,
        openSubMenu,
        10,
        MY_UI_ZONE
    );
    y += Y_STEP;
    
}



//gcc -I/opt/homebrew/include uitest.c knowledge.c rules.c scripts.c solver.c ui.c util.c  -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf
int main() {
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
    

    initScript(&RULE_SET, &KNOWLEDGE_BASE, SCRIPT, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    REVERT_KB = initKB(NUM_PLAYERS); //For backup incase of contradictions
    WORLD_TALLY = initProbKB();

    copyTo(REVERT_KB, KNOWLEDGE_BASE);

    getNames(KNOWLEDGE_BASE->ELEMENT_NAMES, NUM_PLAYERS);

    //Init threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        possibleWorldKB[i] = initKB(NUM_PLAYERS);
        threadTallies[i] = initProbKB();
        possibleWorldTempKB[i] = (KnowledgeBase****)malloc(NUM_DAYS * sizeof(KnowledgeBase****));
        //Create arguments in strctures to pass into new thread
        threadArgs[i] = (struct getProbApproxArgs*) malloc(sizeof(struct getProbApproxArgs));
        if (possibleWorldTempKB[i] == NULL)
        {
            printf("MALLOC FAILED!\n");
            return 1;
        }
        for (int j = 0; j < NUM_DAYS; j++)
        {
            possibleWorldTempKB[i][j] = (KnowledgeBase***)malloc(MAX_SET_ELEMENTS * sizeof(KnowledgeBase***));
            if (possibleWorldTempKB[i][j] == NULL)
            {
                printf("MALLOC FAILED!\n");
                return 1;
            }
            
            for (int k = 0; k < MAX_SET_ELEMENTS; k++)
            {
                //possibleWorldTempKB[i][j][k] = (KnowledgeBase*)malloc(NUM_DAYS * sizeof(KnowledgeBase*));
                
                possibleWorldTempKB[i][j][k] = (KnowledgeBase**)malloc(3 * sizeof(KnowledgeBase**));
                if (possibleWorldTempKB[i][j][k] == NULL)
                {
                    printf("MALLOC FAILED!\n");
                    return 1;
                }
                for (int l = 0; l < 3; l++)
                {
                    possibleWorldTempKB[i][j][k][l] = initKB(NUM_PLAYERS);
                    if (possibleWorldTempKB[i][j][k][l] == NULL)
                    {
                        printf("MALLOC FAILED!\n");
                        return 1;
                    }
                }
            }
        }
    }

    //Init zone to store data
    for (int i = 0; i < MAX_SET_ELEMENTS; i++)
    {
        for (int j = 0; j < NUM_BOTCT_ROLES; j++)
        {
            POSSIBLE_WORLDS_FOR_PROB[i][j] = initKB(NUM_PLAYERS);
            copyTo(POSSIBLE_WORLDS_FOR_PROB[i][j], KNOWLEDGE_BASE);
            POSSIBLE_WORLD_GENERATED[i][j] = 0;
        }
    }

    //Generate NUM_THREADS thread arguments
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadArgs[i]->kb = KNOWLEDGE_BASE; //We MUST promise to never touch this in the thread
        threadArgs[i]->possibleWorldKB = possibleWorldKB[i]; //Working block of memory
        threadArgs[i]->possibleWorldRevertKB = possibleWorldTempKB[i]; //Working block of memory
        threadArgs[i]->determinedInNWorlds = threadTallies[i]; //The output tallies
        threadArgs[i]->worldTally = WORLD_TALLY;
        threadArgs[i]->POSSIBLE_WORLDS_FOR_PROB=&POSSIBLE_WORLDS_FOR_PROB;
        threadArgs[i]->POSSIBLE_WORLD_GENERATED=&POSSIBLE_WORLD_GENERATED;
        threadArgs[i]->worldGeneration = &WORLD_GENERATION;
        threadArgs[i]->reRenderCall = &reRenderCall;
        threadArgs[i]->rs = RULE_SET;
        threadArgs[i]->numIterations = NUM_ITERATIONS;
        
    }
    //Set off NUM_THREADS-1 threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, &getProbApproxContinuous, (void *) threadArgs[i]);
    }
    
    //Don't print rules
    //printRules(rs, kb);

    //solve(kb, rs, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Blood on the Clocktower Solver V1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    
    TTF_Font *ARIAL_FONT = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 12);
    if (!ARIAL_FONT) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }
        

    bool running = true;
    SDL_Event event;

    reRenderCall = true;
    
    updateFirstMenu(ARIAL_FONT);
   

    while (running) {
        if (reRenderCall) 
        {
            //printf("RE RENDER!\n");
            reRenderCall = false;
            //printf("-TABLE!\n");
            
            updateUITable(KNOWLEDGE_BASE, WORLD_TALLY, ARIAL_FONT, currentNight);

            //printf("-FIRST MENU!\n");
            updateFirstMenu(ARIAL_FONT);
            //printf("-SUB MENU!\n");
            updateSubMenu(ARIAL_FONT, KNOWLEDGE_BASE);
            //printf("-SUB SUB MENU!\n");
            updateSubSubMenu(ARIAL_FONT, KNOWLEDGE_BASE);
            //printf("-SUB SUB SUB MENU!\n");
            updateSubSubSubMenu(ARIAL_FONT, KNOWLEDGE_BASE);
            //printf("-SUB SUB SUB SUB MENU!\n");
            updateSubSubSubSubMenu(ARIAL_FONT, KNOWLEDGE_BASE);
            //printf("-CONFIRM MENU!\n");
            updateConfirmButton(ARIAL_FONT, KNOWLEDGE_BASE);
        }
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        updatedButtonInBounds(mouseX, mouseY);
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                runButtonInBounds();
            }
        }

        
        drawUIElements(renderer);  
        SDL_Delay(8);
    }

    TTF_CloseFont(ARIAL_FONT);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}