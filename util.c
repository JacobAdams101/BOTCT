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
#include <time.h>
#include <ctype.h>

#include "util.h"
#include "constants.h"

/**
 * getRandInt() - returns a random integer between two values
 *
 * @min - min number (inclusive)
 * @max - max number (exclusive)
 * 
 * @return min <= rand < max
*/
int getRandInt(int min, int max)
{
    return (rand() % (max - min)) + min;
}

/**
 * getRandIntNotIn() - returns a random integer between 0 and len(avaliable)
 *
 * @avaliable - is number avaliable
 * @numAvaliable - num avaliable
 * 
 * @return min <= rand < max
*/
int getRandIntNotIn(int avaliable[], int numAvaliable)
{
    int rand = getRandInt(0, numAvaliable);
        
    int selected = 0;
    //Find avaliable role 
    while(avaliable[selected] == 0 || rand > 0)
    {
        rand = avaliable[selected] == 1 ? rand - 1 : rand;
        selected++;
    }
    return selected;
}

/**
 * initRand() - Initilize the random number generate
 * 
 * 
*/
void initRand()
{
    srand(time(NULL));
}


char* toLowerCase(char* str)
{
    for (int i = 0; str[i] != '\0'; i++) str[i] = tolower(str[i]);
    return str;
}

char* toUpperCase(char* str)
{
    for (int i = 0; str[i] != '\0'; i++) str[i] = toupper(str[i]);
    return str;
}

