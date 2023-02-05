#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

void removeStr(char *path, int linePos, int charPos, int count, int isForward)
{

    FILE *textFile = fopen(path, "r");
    FILE *tempFile = fopen("root/.tmp/.temp.txt", "w+");

    int currentLine = 1;
    int currentChar = 0;
    char buff;

    while (currentLine != linePos)
    {
        buff = fgetc(textFile);
        fputc(buff, tempFile);

        if (buff == EOF)
            break;
        if (buff == '\n')
            currentLine++;
    }

    if (currentLine != linePos)
    {
        printf("out of range\n");
        fclose(textFile);
        fclose(tempFile);
        remove("root/.tmp/.temp.txt");
        return;
    }

    while (currentChar < charPos)
    {
        buff = fgetc(textFile);
        fputc(buff, tempFile);

        if (buff == EOF)
            break;

        currentChar++;
    }

    if (currentChar != charPos)
    {
        printf("out of range\n");
        fclose(textFile);
        fclose(tempFile);
        remove("root/.tmp/.temp.txt");
        return;
    }

    if (isForward == 1)
        for (int i = 0; i < count; i++)
            buff = fgetc(textFile);
    else
        fseek(tempFile, -count, SEEK_CUR);

    while (1)
    {
        buff = fgetc(textFile);

        fputc(buff, tempFile);

        if (buff == EOF)
            break;
    }

    fclose(textFile);
    fclose(tempFile);

    FILE *textFileToEdit = fopen(path, "w");
    FILE *tempFileRead = fopen("root/.tmp/.temp.txt", "r");

    while (1)
    {
        buff = getc(tempFileRead);
        if (buff == EOF)
            break;
        putc(buff, textFileToEdit);
    }

    fclose(textFileToEdit);
    fclose(tempFileRead);
    remove("root/.tmp/.temp.txt");
}

int handleRemoveStr(char *command, size_t commandLen)
{
    char *path = calloc(commandLen, sizeof(char));
    char *pos = calloc(commandLen, sizeof(char));
    char *size = calloc(commandLen, sizeof(char));
    int startPath = 17, pathIndex = 0, posIndex;

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

    saveCurrentStatus(path);
    removeStr(path, linePos, charPos, sizeCount, isForward);

    return 0;
}
