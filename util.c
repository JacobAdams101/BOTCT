#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"

int getRandInt(int min, int max)
{
    return (rand() % (max - min)) + min;
}

void initRand()
{
    srand(time(NULL));
}