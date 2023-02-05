#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

void insertWithBytes(char *path, char *str, int bytes)
{
    FILE *textFile = fopen(path, "r");
    FILE *tempFile = fopen("root/.tmp/.temp.txt", "w+");
    int byteCounter = 0;
    char buff;

    while (byteCounter < bytes)
    {
        buff = fgetc(textFile);
        fputc(buff, tempFile);
        byteCounter++;
    }

    fprintf(tempFile, "%s", str);

    while (1)
    {
        buff = fgetc(textFile);

        if (buff == EOF)
            break;

        fputc(buff, tempFile);
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
void insertStr(char *path, char *str, int linePos, int charPos)
{

    FILE *textFile = fopen(path, "r");
    FILE *tempFile = fopen("root/.tmp/.temp.txt", "w+");

    int currentLine = 1;
    int currentChar = 0;
    char buff;

    while (currentLine != linePos && buff != EOF)
    {
        buff = fgetc(textFile);
        fputc(buff, tempFile);

        if (buff == EOF)
            break;
        if (buff == '\n')
            currentLine++;
    }

    // if (currentLine != linePos)
    // {
    //     printf("out of range\n");
    //     fclose(textFile);
    //     fclose(tempFile);
    //     remove("root/.tmp/.temp.txt");
    //     return;
    // }

    while (currentChar < charPos)
    {
        buff = fgetc(textFile);
        fputc(buff, tempFile);

        if (buff == EOF)
            break;

        currentChar++;
    }

    // if (currentChar != charPos)
    // {
    //     printf("out of range\n");
    //     fclose(textFile);
    //     fclose(tempFile);
    //     remove("root/.tmp/.temp.txt");
    //     return;
    // }

    fprintf(tempFile, "%s", str);

    while (1)
    {
        buff = fgetc(textFile);

        if (buff == EOF)
            break;

        fputc(buff, tempFile);
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

int handleInsertStr(char *command, size_t commandLen)
{
    int startPath = 17, startStr;
    int pathIndex = 0, strIndex = 0;
    char *path = calloc(commandLen, sizeof(char));
    char *str = calloc(commandLen, sizeof(char));
    char *pos = calloc(commandLen, sizeof(char));

    if (command[startPath] == '"')
    {
        if (command[startPath + 1] == '/')
            startPath += 2;
        else
            startPath += 1;

        while (command[startPath] != '"')
        {
            path[pathIndex++] = command[startPath++];
        }

        if (path[pathIndex - 1] == '/')
            path[pathIndex - 1] = '\0';

        startStr = startPath + 8;
    }
    else
    {
        if (command[startPath] == '/')
            startPath += 1;

        while (command[startPath] != ' ')
        {
            path[pathIndex++] = command[startPath++];
        }

        if (path[pathIndex - 1] == '/')
            path[pathIndex - 1] = '\0';

        startStr = startPath + 7;
    }

    if (pathErrorHandle(path))
        return pathErrorHandle(path);

    int posIndex;

    if (command[startStr] == '"')
    {
        int lastDoubleQuote;

        for (int i = startStr; i < commandLen; i++)
        {
            if (command[i] == '"')
                lastDoubleQuote = i;
        }

        posIndex = lastDoubleQuote + 8;

        for (int i = startStr + 1; i < lastDoubleQuote; i++)
        {
            if (command[i] == 92 /* ascii backslash */)
            {
                if (command[i + 1] == 'n')
                {
                    str[strIndex++] = '\n';
                    ++i;
                }
                else if (command[i + 1] == 92)
                {
                    str[strIndex++] = 92;
                    str[strIndex++] = command[i + 2];
                    i += 2;
                }
            }
            else
            {
                str[strIndex++] = command[i];
            }
        }
    }
    else
    {
        for (int i = startStr; command[i] != ' '; i++, posIndex = i)
        {
            if (command[i] == 92 /* ascii backslash */)
            {
                if (command[i + 1] == 'n')
                {
                    str[strIndex++] = '\n';
                    ++i;
                }
                else if (command[i + 1] == '"')
                {
                    str[strIndex++] = '"';
                    ++i;
                }
                else if (command[i + 1] == 92)
                {
                    str[strIndex++] = 92;
                    str[strIndex++] = command[i + 2];
                    i += 2;
                }
            }
            else
            {
                str[strIndex++] = command[i];
            }
        }

        posIndex += 7;
    }

    for (int i = posIndex; command[i] != ' ' && i != commandLen; i++)
    {
        pos[i - posIndex] = command[i];
    }

    int linePos, charPos;

    linePos = atoi(strtok(pos, ":"));
    charPos = atoi(strtok(NULL, ":"));

    saveCurrentStatus(path);
    insertStr(path, str, linePos, charPos);

    return 0;
}