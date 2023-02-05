#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int isOk2Format(char *path)
{
    FILE *text = fopen(path, "r");

    int openBrackets = 0, closeBrackets = 0;
    char buff;

    while (1)
    {
        buff = fgetc(text);
        if (buff == EOF)
            break;
        if (buff == '{')
            openBrackets++;
        if (buff == '}')
            closeBrackets++;
    }

    fclose(text);

    if (closeBrackets == openBrackets)
        return 1;

    return 0;
}

void formatter(char *path)
{
    int bytesRead = 0;
    int bytesWrite = 0;

    char buff;
    int tabcounter = 0;

    FILE *textFile = fopen(path, "r");

    while (1)
    {
        buff = fgetc(textFile);

        if (buff == EOF)
            break;

        if (buff == '{')
        {
            char *str2Write = calloc(1000, sizeof(char));

            strcpy(str2Write, "\n");

            tabcounter++;

            for (int i = 1; i < tabcounter; i++)
            {
                strcat(str2Write, "\t");
            }

            insertWithBytes(path, "\n", bytesWrite);

            bytesWrite += 1;

            insertWithBytes(path, str2Write, bytesWrite);

            bytesWrite += tabcounter;
        }

        if (buff == '}')
        {
            char *str2Write = calloc(1000, sizeof(char));

            strcpy(str2Write, "\n");

            for (int i = 1; i < tabcounter; i++)
            {
                strcat(str2Write, "\t");
            }

            insertWithBytes(path, "\n", bytesWrite);

            bytesWrite += 1;

            insertWithBytes(path, str2Write, bytesWrite);

            bytesWrite += tabcounter;

            tabcounter--;
        }

        bytesRead++;
        bytesWrite++;
    }

    fclose(textFile);

    textFile = fopen(path, "r");

    bytesRead = 0;
    bytesWrite = 0;
    tabcounter = 0;

    while (1)
    {
        buff = fgetc(textFile);

        if (buff == EOF)
            break;

        if (buff == '\n')
        {
            while (1)
            {
                buff = fgetc(textFile);

                if (buff == '\n')
                {
                    bytesRead++;
                    bytesWrite++;
                    break;
                }

                if (buff == '{')
                    tabcounter++;
                if (buff == '}')
                    tabcounter--;

                if (buff != ' ' && buff != '\t' && buff != '}' && buff != '{')
                {
                    char *str2Write = calloc(1000, sizeof(char));
                    bytesRead++;
                    bytesWrite++;

                    strcpy(str2Write, "\n");

                    for (int i = 1; i <= tabcounter; i++)
                    {
                        strcat(str2Write, "\t");
                    }

                    insertWithBytes(path, str2Write, bytesWrite);

                    bytesWrite += tabcounter + 1;
                    bytesRead++;

                    break;
                }

                bytesRead++;
                bytesWrite++;
            }
        }

        if (buff == '{')
        {
            tabcounter++;

            while (1)
            {
                buff = fgetc(textFile);

                if (buff == '\n')
                {
                    bytesRead++;
                    bytesWrite++;
                    break;
                }

                if (buff != ' ' && buff != '\t')
                {
                    char *str2Write = calloc(1000, sizeof(char));
                    bytesRead++;
                    bytesWrite++;

                    strcpy(str2Write, "\n");

                    for (int i = 1; i <= tabcounter; i++)
                    {
                        strcat(str2Write, "\t");
                    }

                    insertWithBytes(path, str2Write, bytesWrite);

                    bytesWrite += tabcounter + 1;
                    bytesRead++;

                    break;
                }

                bytesRead++;
                bytesWrite++;
            }
        }

        if (buff == '}')
        {
            tabcounter--;

            while (1)
            {
                buff = fgetc(textFile);

                if (buff == '\n')
                {
                    bytesRead++;
                    bytesWrite++;
                    break;
                }

                if (buff != ' ' && buff != '\t')
                {
                    char *str2Write = calloc(1000, sizeof(char));
                    bytesRead++;
                    bytesWrite++;

                    strcpy(str2Write, "\n");

                    for (int i = 1; i <= tabcounter; i++)
                    {
                        strcat(str2Write, "\t");
                    }

                    insertWithBytes(path, str2Write, bytesWrite);

                    bytesWrite += tabcounter + 1;
                    bytesRead++;

                    break;
                }

                bytesRead++;
                bytesWrite++;
            }
        }

        bytesRead++;
        bytesWrite++;
    }

    fclose(textFile);

    textFile = fopen(path, "r");

    bytesRead = 0;
    int lineRead = 1;
    int lineWrite = 1;

    while (1)
    {
        buff = fgetc(textFile);

        if (buff == EOF)
            break;

        if (buff == '\n')
        {
            lineRead++;
            lineWrite++;

            while (1)
            {
                buff = fgetc(textFile);

                if (buff == '\n')
                {
                    lineRead++;
                    lineWrite++;

                    removeStr(path, lineWrite, 0, 1, -1);

                    lineWrite--;
                }
                else
                    break;
            }
        }
    }

    fclose(textFile);

    textFile = fopen(path, "r");

    while (1)
    {
        buff = fgetc(textFile);

        if (buff == ' ' || buff == '\t')
            removeStr(path, 1, 1, 1, -1);
        else
            break;
    }

    fclose(textFile);
}

int handleAutoIndent(char *command, size_t commandLen)
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

    if (!isOk2Format(path))
        return -1;

    saveCurrentStatus(path);
    formatter(path);
}