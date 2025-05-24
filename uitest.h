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

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"

extern pthread_mutex_t problock; // Mutex to protect shared data
extern pthread_mutex_t exampleworldlock; // Mutex to protect shared data
extern pthread_mutex_t cacheworldlock;

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