#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int handleFind(char *command, size_t commandLen)
{
    // index 0 -> count index 1 -> at index 2 -> byword index 3 -> all
    char options[4] = "bbbb";
    int startStr = 11, strIndex = 0, isWildCard = 0, startPath = 11, pathIndex = 0, at = 0;
    char *str = calloc(commandLen, sizeof(char));
    char *path = calloc(commandLen, sizeof(char));

    if (command[startStr] == '"')
        for (int i = startStr + 1; command[i] != '"'; ++i, startPath = i + 1)
        {
            if (command[i] == 92 && command[i + 1] == '*')
            {
                str[strIndex++] = command[i + 1];
                i++;
            }
            else
            {
                str[strIndex++] = command[i];
                if (command[i] == '*')
                    isWildCard = 1;
            }
        }
    else
        for (int i = startStr; command[i] != ' '; ++i, startPath = i)
        {
            if (command[i] == 92 && command[i + 1] == '*')
            {
                str[strIndex++] = command[i + 1];
                i++;
            }
            else
            {
                str[strIndex++] = command[i];
                if (command[i] == '*')
                    isWildCard = 1;
            }
        }

    startPath += 8;

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

        startPath++;
    }
    else
    {
        if (command[startPath] == '/')
            startPath += 1;

        while (startPath < commandLen && command[startPath] != ' ')
        {
            path[pathIndex++] = command[startPath++];
        }

        if (path[pathIndex - 1] == '/')
            path[pathIndex - 1] = '\0';
    }

    if (pathErrorHandle(path))
        return pathErrorHandle(path);

    char *optionStr = calloc(commandLen, sizeof(char));

    strcpy(optionStr, command + startPath + 1);

    char *tok = strtok(optionStr, "-");
    while (tok != NULL)
    {
        if (strncmp(tok, "count", 5) == 0)
            options[0] = 97;

        if (strncmp(tok, "byword", 6) == 0)
            options[2] = 97;

        if (strncmp(tok, "all", 3) == 0)
            options[3] = 97;

        if (strncmp(tok, "at", 2) == 0)
        {
            options[1] = 97;
            at = atoi(tok + 3);
        }

        tok = strtok(NULL, "-");
    }

    if (options[0] == 'a' && strcmp(options, "abbb") != 0)
        return -1;

    if (options[3] == 97 && options[1] == 97)
        return -1;

    FILE *textFile = fopen(path, "r");

    char *line = calloc(100000, sizeof(char));

    for (int i = 0; i < 100000; i++)
    {
        char buff = fgetc(textFile);

        if (buff != EOF)
            line[i] = buff;
        else
            break;
    }

    if (isWildCard == 0)
    {
        int lenLine = strlen(line);
        int lenStr = strlen(str);
        int caseCounter = 0;
        int wordCounter = 1;

        for (int i = 0; i <= lenLine - lenStr; i++)
        {
            if (strncmp(str, line + i, lenStr) == 0)
            {
                caseCounter++;

                if (strcmp(options, "bbbb") == 0)
                {
                    printf("%d\n", i);
                    return 0;
                }

                if (strcmp(options, "bbba") == 0)
                    printf("%d ", i);

                if (strcmp(options, "bbab") == 0)
                {
                    for (int j = 0; j < i; j++)
                        if (line[j] == ' ')
                            wordCounter++;

                    printf("%d\n", wordCounter);

                    return 0;
                }

                if (strcmp(options, "babb") == 0)
                {
                    if (caseCounter == at)
                    {
                        printf("%d\n", i);
                        return 0;
                    }
                }

                if (strcmp(options, "bbaa") == 0)
                {
                    for (int j = 0; j < i; j++)
                        if (line[j] == ' ')
                            wordCounter++;

                    printf("%d ", wordCounter);
                }

                if (strcmp(options, "baab") == 0)
                {
                    if (caseCounter == at)
                    {
                        for (int j = 0; j < i; j++)
                            if (line[j] == ' ')
                                wordCounter++;

                        printf("%d\n", wordCounter);

                        return 0;
                    }
                }
            }
        }

        if (caseCounter == 0 || at > caseCounter)
        {
            printf("-1\n");
            return 0;
        }

        if (strcmp(options, "abbb") == 0)
            printf("%d\n", caseCounter);

        return 0;
    }

    int spaceCount = 0;

    for (int i = 0; i < strlen(str); i++)
        if (str[i] == ' ')
            spaceCount++;

    if (isWildCard == 1 && spaceCount == 0)
    {
        int lenLine = strlen(line);
        int lenStr = strlen(str);
        int caseCounter = 0;
        int wordCounter = 0;
        int isAtFirst = 0;
        int index = 0;

        if (str[0] == '*')
        {
            isAtFirst = 1;
            strcpy(str, str + 1);
            lenStr--;
        }
        else
        {
            str[lenStr - 1] = '\0';
            lenStr--;
        }

        if (isAtFirst == 1)
        {
            char *tok = strtok(line, " ");

            while (tok != NULL)
            {
                wordCounter++;

                for (int i = strlen(tok) - strlen(str); i >= 0; i--)
                {
                    if (strncmp(str, tok + i, strlen(str)) == 0)
                    {
                        caseCounter++;

                        if (strcmp(options, "bbbb") == 0)
                        {
                            printf("%d\n", index);
                            return 0;
                        }

                        if (strcmp(options, "bbba") == 0)
                            printf("%d ", index);

                        if (strcmp(options, "bbab") == 0)
                        {
                            printf("%d\n", wordCounter);
                            return 0;
                        }

                        if (strcmp(options, "babb") == 0)
                        {
                            if (caseCounter == at)
                            {
                                printf("%d\n", index);
                                return 0;
                            }
                        }

                        if (strcmp(options, "bbaa") == 0)
                        {
                            printf("%d ", wordCounter);
                        }

                        if (strcmp(options, "baab") == 0)
                        {
                            if (caseCounter == at)
                            {
                                printf("%d\n", wordCounter);

                                return 0;
                            }
                        }

                        break;
                    }
                }

                index += strlen(tok) + 1;
                tok = strtok(NULL, " ");
            }

            if (caseCounter == 0 || at > caseCounter)
            {
                printf("-1\n");
                return 0;
            }

            if (strcmp(options, "abbb") == 0)
                printf("%d\n", caseCounter);

            return 0;
        }

        if (isAtFirst == 0)
        {
            char *tok = strtok(line, " ");

            while (tok != NULL)
            {
                wordCounter++;

                for (int i = 0; i <= strlen(tok) - strlen(str); i++)
                {
                    if (strncmp(str, tok + i, strlen(str)) == 0)
                    {
                        caseCounter++;

                        if (strcmp(options, "bbbb") == 0)
                        {
                            printf("%d\n", index + i);
                            return 0;
                        }

                        if (strcmp(options, "bbba") == 0)
                            printf("%d ", index + i);

                        if (strcmp(options, "bbab") == 0)
                        {
                            printf("%d\n", wordCounter);
                            return 0;
                        }

                        if (strcmp(options, "babb") == 0)
                        {
                            if (caseCounter == at)
                            {
                                printf("%d\n", index + i);
                                return 0;
                            }
                        }

                        if (strcmp(options, "bbaa") == 0)
                        {
                            printf("%d ", wordCounter);
                        }

                        if (strcmp(options, "baab") == 0)
                        {
                            if (caseCounter == at)
                            {
                                printf("%d\n", wordCounter);

                                return 0;
                            }
                        }

                        break;
                    }
                }

                index += strlen(tok) + 1;
                tok = strtok(NULL, " ");
            }

            if (caseCounter == 0 || at > caseCounter)
            {
                printf("-1\n");
                return 0;
            }

            if (strcmp(options, "abbb") == 0)
                printf("%d\n", caseCounter);

            return 0;
        }
    }
}