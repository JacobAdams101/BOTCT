#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"

// Define a function type
typedef void (*EventFunction)(int);

typedef struct {
    SDL_Rect box;
    SDL_Color boxColor;
    SDL_Color highlightColor;
    SDL_Color textColor; 
    TTF_Font *font;
    char text[STRING_BUFF_SIZE]; 
    int highlighted;
    EventFunction clickEventFunction;
    int eventID;
} TextBox;