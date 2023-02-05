#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int handleCompare(char *command, size_t commandLen)
{
    int startPaths = 8;
    int pathsCount = 0;
    char **paths = calloc(2, sizeof(char *));

    while (pathsCount != 2)
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
    }

    FILE *files[2];
    char **lines = calloc(2, sizeof(char *));
    char **linesTmp = calloc(2, sizeof(char *));

    for (int i = 0; i < pathsCount; i++)
    {
        if (pathErrorHandle(paths[i]))
            return pathErrorHandle(paths[i]);

        files[i] = fopen(paths[i], "r");
        lines[i] = calloc(100000, sizeof(char));
        linesTmp[i] = calloc(100000, sizeof(char));
    }

    int lineCounter = 0;
    char *returnFgets[2];

    while (1)
    {
        returnFgets[0] = fgets(lines[0], 100000, files[0]);
        returnFgets[1] = fgets(lines[1], 100000, files[1]);
        linesTmp[0] = strdup(lines[0]);
        linesTmp[1] = strdup(lines[1]);

        if (returnFgets[0] == NULL && returnFgets[1] == NULL)
            break;

        if (returnFgets[0] != NULL && returnFgets[1] != NULL)
        {
            lineCounter++;

            if (strcmp(lines[0], lines[1]) != 0)
            {
                int spaceCount[2] = {0};

                printf("============ #%d ============\n", lineCounter);

                for (int i = 0; lines[0][i] != '\0'; i++)
                {
                    if (lines[0][i] == ' ')
                        spaceCount[0]++;
                    if (lines[0][i] == '\n')
                        lines[0][i] = '\0';
                }

                for (int i = 0; lines[1][i] != '\0'; i++)
                {
                    if (lines[1][i] == ' ')
                        spaceCount[1]++;
                    if (lines[1][i] == '\n')
                        lines[1][i] = '\0';
                }

                if (spaceCount[0] != spaceCount[1])
                {
                    printf("%s\n%s\n", lines[0], lines[1]);
                    continue;
                }
                else
                {
                    int countTok = 0, difCount = 0, difIndex = 0;

                    char ***words = calloc(2, sizeof(char **));
                    char *tok;
                    words[0] = calloc(2000, sizeof(char *));
                    words[1] = calloc(2000, sizeof(char *));

                    tok = strtok(linesTmp[0], " ");

                    while (tok != NULL)
                    {
                        words[0][countTok] = calloc(1000, sizeof(char));

                        strcpy(words[0][countTok], tok);
                        tok = strtok(NULL, " ");

                        countTok++;
                    }

                    countTok = 0;

                    tok = strtok(linesTmp[1], " ");

                    while (tok != NULL)
                    {
                        words[1][countTok] = calloc(1000, sizeof(char));

                        strcpy(words[1][countTok], tok);
                        tok = strtok(NULL, " ");

                        countTok++;
                    }

                    for (int i = 0; i < countTok; i++)
                    {

                        if (strcmp(words[0][i], words[1][i]) != 0)
                        {
                            difCount++;
                            difIndex = i;
                        }
                        if (difCount > 1)
                            break;
                    }

                    if (difCount == 1)
                    {
                        for (int i = 0; i < countTok; i++)
                        {
                            if (i == difIndex)
                                printf(">>%s<< ", words[0][i]);
                            else
                                printf("%s ", words[0][i]);
                        }

                        printf("\n");

                        for (int i = 0; i < countTok; i++)
                        {
                            if (i == difIndex)
                                printf(">>%s<< ", words[1][i]);
                            else
                                printf("%s ", words[1][i]);
                        }

                        printf("\n");
                    }
                    else
                    {
                        printf("%s\n%s\n", lines[0], lines[1]);
                        continue;
                    }
                }
            }
        }

        if (returnFgets[0] != NULL && returnFgets[1] == NULL)
        {
            int firstLine = lineCounter;

            printf(">>>>>>>>>>>> File No.1 >>>>>>>>>>>>\n");

            while (1)
            {
                printf("%s", lines[0]);

                returnFgets[0] = fgets(lines[0], 100000, files[0]);

                if (returnFgets[0] == NULL)
                    break;

                lineCounter++;
            }

            printf("\n>>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", firstLine, lineCounter);

            break;
        }

        if (returnFgets[0] == NULL && returnFgets[1] != NULL)
        {
            int firstLine = lineCounter;

            printf(">>>>>>>>>>>> File No.2 >>>>>>>>>>>>\n");

            while (1)
            {
                printf("%s", lines[1]);

                returnFgets[1] = fgets(lines[1], 100000, files[1]);

                if (returnFgets[1] == NULL)
                    break;

                lineCounter++;
            }

            printf("\n>>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", firstLine, lineCounter);

            break;
        }
    }
}