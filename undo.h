#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int handleUndo(char *command, size_t commandLen)
{
    char *path = calloc(commandLen, sizeof(char));
    int startPath = 12, pathIndex = 0;

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

    return restoreLastStatus(path);
}