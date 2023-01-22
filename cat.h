#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int catFile(char *path)
{

    if (!fileExists(path))
        return -1;

    FILE *textFile = fopen(path, "r");
    char buff;

    while (1)
    {
        buff = fgetc(textFile);
        if (buff == EOF)
            break;
        printf("%c", buff);
    }
}