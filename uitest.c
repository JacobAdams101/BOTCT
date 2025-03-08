#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "uitest.h"

#define WIDTH 1024
#define HEIGHT 1024

void renderTextBox(SDL_Renderer *renderer, TextBox* tb)
{
    //Draw box
    if (tb->highlighted) SDL_SetRenderDrawColor(renderer, tb->highlightColor.r, tb->highlightColor.g, tb->highlightColor.b, tb->highlightColor.a);
    else SDL_SetRenderDrawColor(renderer, tb->boxColor.r, tb->boxColor.g, tb->boxColor.b, tb->boxColor.a);
    
    SDL_RenderFillRect(renderer, &tb->box);

    //Draw text
    SDL_Surface *surface = TTF_RenderText_Solid(tb->font, tb->text, tb->textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {tb->box.x, tb->box.y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    //Free memory
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

#define MAX_UI_ELEMENTS 100

TextBox UI_ELEMENTS[MAX_UI_ELEMENTS];
int COUNT = 0;

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

int addTextBox(int x, int y, int width, int height, Uint8 boxr, Uint8 boxg, Uint8 boxb, Uint8 highlightboxr, Uint8 highlightboxg, Uint8 highlightboxb, Uint8 textr, Uint8 textg, Uint8 textb, const char * text, TTF_Font *FONT)
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
    tb->text = text; 
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
        if (tb->highlighted)
        {
            printf("Clicked!\n");
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    
    TTF_Font *ARIAL_FONT = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 24);
    if (!ARIAL_FONT) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }
        

    bool running = true;
    bool textChanged = false;
    SDL_Event event;

    addTextBox(
        100, 100, 200, 100, //bb
        255, 0, 0, //Box colour
        255, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "Hello SDL!", 
        ARIAL_FONT
    );

    addTextBox(
        300, 100, 200, 100, //bb
        255, 0, 0, //Box colour
        255, 100, 100, //Highlighted Box colour
        255, 255, 255, //Text colour
        "Hello SDL!", 
        ARIAL_FONT
    );

    /*
    TextBox box2;
    TextBox* button = box2;
    button->box= {100, 50, 100, 100};
    button->boxColor = {255, 255, 0, 255};
    button->textColor = {255, 255, 255, 255}; 
    button->font = ARIAL_FONT;
    char* text = "Button"; 
    */

    while (running) {
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
