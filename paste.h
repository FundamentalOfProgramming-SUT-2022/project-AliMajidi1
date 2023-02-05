#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

#define MAX_CLIPBORD_LEN 1000000000
int handlePasteStr(char *command, size_t commandLen)
{
    char *path = calloc(commandLen, sizeof(char));
    char *pos = calloc(commandLen, sizeof(char));
    char *str = calloc(MAX_CLIPBORD_LEN, sizeof(char));
    int startPath = 16, pathIndex = 0, posIndex;

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

        posIndex = startPath + 8;
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

        posIndex = startPath + 7;
    }

    if (pathErrorHandle(path))
        return pathErrorHandle(path);

    for (int i = posIndex; command[i] != ' ' && i != commandLen; i++)
    {
        pos[i - posIndex] = command[i];
    }

    int linePos, charPos, sizeCount, sizeIndex = posIndex + strlen(pos) + 7;

    linePos = atoi(strtok(pos, ":"));
    charPos = atoi(strtok(NULL, ":"));

    FILE *clipBoard = fopen("root/.tmp/.clipboard.txt", "r");
    int count = 0;

    while (1)
    {
        char buff = fgetc(clipBoard);

        if (buff == EOF)
            break;

        str[count++] = buff;
    }

    fclose(clipBoard);
    saveCurrentStatus(path);
    insertStr(path, str, linePos, charPos);
}