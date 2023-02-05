#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

void catFile(char *path)
{

    FILE *textFile = fopen(path, "r");
    char buff;

    while (1)
    {
        buff = fgetc(textFile);
        if (buff == EOF)
            break;
        printf("%c", buff);
    }

    printf("\n");
}

int handleCatFile(char *command, size_t commandLen)
{
    char *path = calloc(commandLen, sizeof(char));
    int startPath = 11, pathIndex = 0;

    if (command[startPath] == '"')
    {
        if (command[startPath + 1] == '/')
            startPath += 2;
        else
            startPath++;

        while (command[startPath] != '"')
        {
            path[pathIndex++] = command[startPath++];
        }

        if (path[pathIndex - 1] == '/')
            path[pathIndex - 1] = '\0';
    }
    else
    {
        if (command[startPath] == '/')
            startPath++;

        while (command[startPath] != ' ' && startPath != commandLen)
        {
            path[pathIndex++] = command[startPath++];
        }

        if (path[pathIndex - 1] == '/')
            path[pathIndex - 1] = '\0';
    }

    if (pathErrorHandle(path))
        return pathErrorHandle(path);

    catFile(path);

    return 0;
}