#include "debug.h"

int IS_DEBUG_ENABLE = 1;

void printDebug(char* text) 
{
    if (IS_DEBUG_ENABLE) 
    {
        puts(text);
        puts("\n");
    }
}