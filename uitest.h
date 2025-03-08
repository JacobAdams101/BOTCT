#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect box;
    SDL_Color boxColor;
    SDL_Color highlightColor;
    SDL_Color textColor; 
    TTF_Font *font;
    const char* text; 
    int highlighted;
} TextBox;