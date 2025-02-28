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

#define INT_LENGTH 64
#define STRING_BUFF_SIZE 256

#define NUM_SETS 3
#define MAX_SET_ELEMENTS 16
#define FUNCTION_RESULT_SIZE 24
#define MAX_VARS_IN_RULE 16
#define MAX_NUM_RULES 65536

//UI Constants
#define RED_COLOUR_START "\033[31m"
#define YELLOW_COLOUR_START "\033[33m"
#define GREEN_COLOUR_START "\033[32m"
#define PURPLE_COLOUR_START "\033[35m"
#define TITLE_TEXT_START "\033[0;1m"
#define COLOUR_END "\033[0m"

#define PRINT_RED printf(RED_COLOUR_START); 
#define PRINT_YELLOW printf(YELLOW_COLOUR_START); 
#define PRINT_GREEN printf(GREEN_COLOUR_START); 
#define PRINT_PURPLE printf(PURPLE_COLOUR_START); 

#define PRINT_TITLE printf(TITLE_TEXT_START); 

#define PRINT_END printf(COLOUR_END); 