
#include <stdio.h>
#include <stdlib.h>

void Panic(char * s)
{
    printf("\n Error: %s\n", s);
    exit(1);
}
