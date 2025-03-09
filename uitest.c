#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "uitest.h"

#include "rules.h"
#include "knowledge.h"
#include "constants.h"
#include "scripts.h"
#include "ui.h"
#include "util.h"
#include "solver.h"

#define WIDTH 1500
#define HEIGHT 750

#define MAX_UI_ELEMENTS 1000

TextBox UI_ELEMENTS[MAX_UI_ELEMENTS];
int COUNT = 0;
int currentNight = 0;
bool reRenderCall = false;

void event_PrevNight()
{
    currentNight = currentNight - 1;
    if (currentNight < 0) currentNight += NUM_DAYS;
    reRenderCall = true;
    printf("PREV NIGHT CLICKED!\n");
}
void event_NextNight()
{
    currentNight = currentNight + 1;
    if (currentNight >= NUM_DAYS) currentNight -= NUM_DAYS;
    reRenderCall = true;
    printf("Next NIGHT CLICKED!\n");
}

void renderTextBox(SDL_Renderer *renderer, TextBox* tb)
{
    //Draw box
    if (tb->highlighted) SDL_SetRenderDrawColor(renderer, tb->highlightColor.r, tb->highlightColor.g, tb->highlightColor.b, tb->highlightColor.a);
    else SDL_SetRenderDrawColor(renderer, tb->boxColor.r, tb->boxColor.g, tb->boxColor.b, tb->boxColor.a);
    
    SDL_RenderFillRect(renderer, &tb->box);

    //Draw text
    SDL_Surface *surface = TTF_RenderText_Solid(tb->font, &(tb->text[0]), tb->textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {tb->box.x, tb->box.y, surface->w, surface->h};
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
    for (int uiElement = 0; uiElement < COUNT; uiElement++)
    {
        TextBox* tb = &UI_ELEMENTS[uiElement];
        renderTextBox(renderer, tb);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

void resetScreen()
{
    COUNT = 0;
}

int addTextBox(
    int x, int y, int width, int height, 
    Uint8 boxr, Uint8 boxg, Uint8 boxb, 
    Uint8 highlightboxr, Uint8 highlightboxg, Uint8 highlightboxb, 
    Uint8 textr, Uint8 textg, Uint8 textb, 
    const char * text, TTF_Font *FONT,
    EventFunction clickEvent
)
{
    if (COUNT >= MAX_UI_ELEMENTS) return -1;

    TextBox* tb = &UI_ELEMENTS[COUNT];
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
    COUNT++;
    return 1;
}

int isInBounds(TextBox* tb, int x, int y)
{
    return (tb->box.x <= x) && (x <= tb->box.x+tb->box.w) && (tb->box.y <= y) && (y <= tb->box.y+tb->box.h);
}

void updatedButtonInBounds(int mx, int my)
{
    for (int uiElement = 0; uiElement < COUNT; uiElement++)
    {
        TextBox* tb = &UI_ELEMENTS[uiElement];
        tb->highlighted = isInBounds(tb, mx, my);
    }
}

void runButtonInBounds()
{
    for (int uiElement = 0; uiElement < COUNT; uiElement++)
    {
        TextBox* tb = &UI_ELEMENTS[uiElement];
        if (tb->highlighted && tb->clickEventFunction != NULL)
        {
            tb->clickEventFunction();
        }
    }
}



void makeTable(KnowledgeBase* kb, TTF_Font *FONT, int night)
{
    char buff[STRING_BUFF_SIZE];

    const int X_WIDTH = 40;
    const int Y_WIDTH = 20;
    const int X_STEP = X_WIDTH+5;
    const int Y_STEP = Y_WIDTH+5;

    const int X_START = 50;

    int y = 50;
    int x = X_START;

    snprintf(buff, STRING_BUFF_SIZE, "ROLE TABLE [NIGHT%d]", night);

    printf("buff: %s", buff);

    addTextBox(
        x, y, X_WIDTH*5, Y_WIDTH, //bb
        0, 0, 0, //Box colour
        0, 0, 0, //Highlighted Box colour
        255, 255, 255, //Text colour
        buff, 
        FONT,
        NULL
    );
    x += X_STEP*5;
    addTextBox(
        x, y, X_WIDTH*2, Y_WIDTH, //bb
        50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "PREV NIGHT", 
        FONT,
        event_PrevNight
    );
    x += X_STEP*2;
    addTextBox(
        x, y, X_WIDTH*2, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "NEXT NIGHT", 
        FONT,
        event_NextNight
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

            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                red, green, blue, //Text colour
                ROLE_NAMES[role], 
                FONT,
                NULL
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
        NULL
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "TEAM", 
        FONT,
        NULL
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "POISONED", 
        FONT,
        NULL
    );
    x += X_STEP;
    addTextBox(
        x, y, X_WIDTH, Y_WIDTH, //bb
        50, 50, 50, //Box colour
        100, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "ALIVE", 
        FONT,
        NULL
    );
    x += X_STEP;


    int set = 0;
    for (int element = 0; element < kb->SET_SIZES[set]; element++)
    {
        y += Y_STEP;
        x = X_START;
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
                snprintf(buff, 64, "is_NOT_%s_[NIGHT%d]", ROLE_NAMES[role], night);
                int isNotRole = isKnownName(kb, "PLAYERS", element, buff);
                char* text;
                if (isRole == 1) text = "*";
                else if (isNotRole == 1) text = "";
                else text = "?";

                addTextBox(
                    x, y, X_WIDTH, Y_WIDTH, //bb
                    50, 50, 50, //Box colour
                    100, 100, 100, //Highlighted Box colour
                    red, green, blue, //Text colour
                    text, 
                    FONT,
                    NULL
                );
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
                NULL
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
                NULL
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
                NULL
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
                NULL
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
                NULL
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
                NULL
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
                NULL
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
                NULL
            );
            x += X_STEP;
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
                NULL
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
                NULL
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
                NULL
            );
            x += X_STEP;
        }
        printf("|");
        if (isAlive == 1)
        {
            addTextBox(
                x, y, X_WIDTH, Y_WIDTH, //bb
                50, 50, 50, //Box colour
                100, 100, 100, //Highlighted Box colour
                0, 255, 0, //Text colour
                "ALIVE", 
                FONT,
                NULL
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
                NULL
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
                NULL
            );
            x += X_STEP;
        }
    }
}

void updateUI(KnowledgeBase* kb, TTF_Font *FONT, int night)
{
    currentNight = night;
    resetScreen();
    makeTable(kb, FONT, night);
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
    //Make knowlegde base
    KnowledgeBase* kb = NULL;
    RuleSet* rs = NULL;

    initScript(&rs, &kb, SCRIPT, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    getNames(kb->ELEMENT_NAMES, NUM_PLAYERS);
    
    //Don't print rules
    //printRules(rs, kb);

    //solve(kb, rs, NUM_PLAYERS, NUM_MINIONS, NUM_DEMONS, BASE_OUTSIDERS);

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    
    TTF_Font *ARIAL_FONT = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 12);
    if (!ARIAL_FONT) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }
        

    bool running = true;
    SDL_Event event;

    reRenderCall = true;
    
    
   

    while (running) {
        if (reRenderCall) 
        {
            printf("RE RENDER\n");
            reRenderCall = false;
            updateUI(kb, ARIAL_FONT, currentNight);
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
    }

    TTF_CloseFont(ARIAL_FONT);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}