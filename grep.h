#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int handleGrep(char *command, size_t commandLen)
{
    int startStr = 11, startPaths = 0;
    char option = '\0';
    char *str = calloc(commandLen, sizeof(char));

    if (command[5] == '-' && command[6] == 'c')
        option = 'c';
    if (command[5] == '-' && command[6] == 'l')
        option = 'l';
    if (option != '\0')
        startStr += 3;

    if (command[startStr] == '"')
    {
        startPaths = startStr + 2;
        for (int i = startStr + 1; command[i] != '"'; i++, startPaths++)
            str[i - startStr - 1] = command[i];
    }
    else
    {
        startPaths = startStr;
        for (int i = startStr; command[i] != ' '; i++, startPaths++)
            str[i - startStr] = command[i];
    }

    startPaths += 9;
    int pathsCount = 0;
    char **paths = calloc(100, sizeof(char *));

    while (1)
    {
        int pathIndex = 0;
        paths[pathsCount] = calloc(commandLen, sizeof(char));

        if (command[startPaths] == '"')
        {
            if (command[startPaths + 1] == '/')
                startPaths += 2;
            else
                startPaths += 1;

            while (command[startPaths] != '"')
                paths[pathsCount][pathIndex++] = command[startPaths++];

            if (paths[pathsCount][pathIndex - 1] == '/')
                paths[pathsCount][pathIndex - 1] = '\0';

            startPaths += 2;
        }
        else
        {
            if (command[startPaths] == '/')
                startPaths += 1;

            while (command[startPaths] != ' ')
            {
                paths[pathsCount][pathIndex++] = command[startPaths++];

                if (startPaths == commandLen)
                    break;
            }

            if (paths[pathsCount][pathIndex - 1] == '/')
                paths[pathsCount][pathIndex - 1] = '\0';

            startPaths++;
        }

        pathsCount++;

        if (startPaths >= commandLen)
            break;
    }

    FILE *files[pathsCount];

    for (int i = 0; i < pathsCount; i++)
    {
        if (pathErrorHandle(paths[i]))
            return pathErrorHandle(paths[i]);

        files[i] = fopen(paths[i], "r");
    }

    char *line = calloc(100000, sizeof(char));
    char *buff = calloc(strlen(str), sizeof(char));
    int cOptionResult = 0;

    for (int i = 0; i < pathsCount; i++)
    {
        while (1)
        {

            if (fgets(line, 100000, files[i]) == NULL)
                break;

            for (int j = 0; j < strlen(line) - strlen(str); j++)
            {
                for (int k = 0; k < strlen(str); k++)
                    buff[k] = line[j + k];

                if (strcmp(str, buff) == 0)
                {
                    if (option == 'c')
                    {
                        cOptionResult++;
                        break;
                    }
                    else if (option == 'l')
                    {
                        printf("%s\n", paths[i]);
                        goto whenFind;
                    }
                    else
                    {
                        printf("%s: %s", paths[i], line);
                        break;
                    }
                }
            }
        }
    whenFind:
    }

    if (option == 'c')
        printf("%d\n", cOptionResult);
}
