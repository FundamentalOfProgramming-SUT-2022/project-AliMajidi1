#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int copyStr(char *path, int linePos, int charPos, int count, int isForward)
{

    if (!fileExists(path))
        return -1;

    FILE *textFile = fopen(path, "r");
    FILE *clipBoard = fopen("root/.tmp/.clipboard.txt", "w+");

    int currentLine = 1;
    int currentChar = 0;
    char buff;

    while (currentLine != linePos)
    {
        buff = fgetc(textFile);

        if (buff == '\n')
            currentLine++;
    }

    while (currentChar < charPos)
    {
        buff = fgetc(textFile);

        currentChar++;
    }

    if (isForward == 1)
        for (int i = 0; i < count; i++)
        {
            buff = fgetc(textFile);
            fputc(buff, clipBoard);
        }
    else
    {
        fseek(textFile, -count, SEEK_CUR);

        for (int i = 0; i < count; i++)
        {
            buff = fgetc(textFile);
            fputc(buff, clipBoard);
        }
    }

    fclose(textFile);
    fclose(clipBoard);
}

int handleCopyStr(char *command, size_t commandLen)
{
    char *path = calloc(commandLen, sizeof(char));
    char *pos = calloc(commandLen, sizeof(char));
    char *size = calloc(commandLen, sizeof(char));
    int startPath = 15, pathIndex = 0, posIndex;

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

    for (int i = sizeIndex; command[i] != ' '; i++)
    {
        size[i - sizeIndex] = command[i];
    }

    sizeCount = atoi(size);

    int isForward = 1;

    if (command[sizeIndex + strlen(size) + 2] == 'b')
        isForward = -1;

    copyStr(path, linePos, charPos, sizeCount, isForward);

    return 0;
}
