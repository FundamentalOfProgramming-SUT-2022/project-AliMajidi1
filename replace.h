#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int handleReplace(char *command, size_t commandLen)
{
    int startStr = 15, strIndex = 0, isWildCard = 0, startPath = 11, pathIndex = 0, at = 0;
    char *str1 = calloc(commandLen, sizeof(char));
    char *str2 = calloc(commandLen, sizeof(char));
    char *path = calloc(commandLen, sizeof(char));

    if (command[startStr] == '"')
        for (int i = startStr + 1; command[i] != '"'; ++i, startStr = i + 1)
        {
            if (command[i] == 92 && command[i + 1] == '*')
            {
                str1[strIndex++] = command[i + 1];
                i++;
            }
            else
            {
                str1[strIndex++] = command[i];
                if (command[i] == '*')
                    isWildCard = 1;
            }
        }
    else
        for (int i = startStr; command[i] != ' '; ++i, startStr = i)
        {
            if (command[i] == 92 && command[i + 1] == '*')
            {
                str1[strIndex++] = command[i + 1];
                i++;
            }
            else
            {
                str1[strIndex++] = command[i];
                if (command[i] == '*')
                    isWildCard = 1;
            }
        }

    startStr += 8;
    strIndex = 0;

    if (command[startStr] == '"')
        for (int i = startStr + 1; command[i] != '"'; ++i, startPath = i + 1)
            str2[strIndex++] = command[i];

    else
        for (int i = startStr; command[i] != ' '; ++i, startPath = i)
            str2[strIndex++] = command[i];

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
    int hasAt = 0, hasAll = 0;

    char *tok = strtok(optionStr, "-");
    while (tok != NULL)
    {

        if (strncmp(tok, "all", 3) == 0)
            hasAll = 1;

        if (strncmp(tok, "at", 2) == 0)
        {
            hasAt = 1;
            at = atoi(tok + 3);
        }

        tok = strtok(NULL, "-");
    }

    if (hasAt == 1 && hasAll == 1)
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
        int lenStr = strlen(str1);
        int caseCounter = 0;
        char *commandRemove = calloc(10000, sizeof(char));
        char *commandInsert = calloc(10000, sizeof(char));
        char *commandAll = calloc(10000, sizeof(char));

        for (int i = 0; i <= lenLine - lenStr; i++)
        {
            if (strncmp(str1, line + i, lenStr) == 0)
            {
                caseCounter++;

                if ((caseCounter == at || hasAt == 0) && hasAll == 0)
                {
                    sprintf(commandRemove, "removestr --file %c%s%c --pos 1:%d -size %d -f", '"', path, '"', i, (int)strlen(str1));
                    handleRemoveStr(commandRemove, strlen(commandRemove));
                    sprintf(commandInsert, "insertstr --file %c%s%c --str %s --pos 1:%d", '"', path, '"', str2, i);
                    handleInsertStr(commandInsert, strlen(commandInsert));

                    return 0;
                }
            }
        }

        if (caseCounter == 0 || at > caseCounter)
        {
            printf("-1\n");

            return 0;
        }

        if (hasAll == 1)
        {
            for (int i = 0; i < caseCounter; i++)
            {
                sprintf(commandAll, "replace --str1 %c%s%c --str2 %c%s%c --file %c%s%c", '"', str1, '"', '"', str2, '"', '"', path, '"');
                handleReplace(commandAll, strlen(commandAll));
            }
        }

        return 0;
    }

    int spaceCount = 0;

    for (int i = 0; i < strlen(str1); i++)
        if (str1[i] == ' ')
            spaceCount++;

    if (isWildCard == 1 && spaceCount == 0)
    {
        int lenLine = strlen(line);
        int lenStr = strlen(str1);
        int caseCounter = 0;
        int wordCounter = 0;
        int isAtFirst = 0;
        int index = 0;
        char *commandRemove = calloc(10000, sizeof(char));
        char *commandInsert = calloc(10000, sizeof(char));
        char *commandAll = calloc(10000, sizeof(char));

        if (str1[0] == '*')
        {
            isAtFirst = 1;
            strcpy(str1, str1 + 1);
            lenStr--;
        }
        else
        {
            str1[lenStr - 1] = '\0';
            lenStr--;
        }

        if (isAtFirst == 1)
        {
            char *tok = strtok(line, " ");

            while (tok != NULL)
            {
                wordCounter++;

                for (int i = strlen(tok) - strlen(str1); i >= 0; i--)
                {
                    if (strncmp(str1, tok + i, strlen(str1)) == 0)
                    {
                        caseCounter++;

                        if ((caseCounter == at || hasAt == 0) && hasAll == 0)
                        {
                            sprintf(commandRemove, "removestr --file %c%s%c --pos 1:%d -size %d -f", '"', path, '"', index, i + (int)strlen(str1));
                            handleRemoveStr(commandRemove, strlen(commandRemove));
                            sprintf(commandInsert, "insertstr --file %c%s%c --str %s --pos 1:%d", '"', path, '"', str2, index);
                            handleInsertStr(commandInsert, strlen(commandInsert));

                            return 0;
                        }
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

            if (hasAll == 1)
            {
                for (int i = 0; i < caseCounter; i++)
                {
                    sprintf(commandAll, "replace --str1 %c%c%s%c --str2 %c%s%c --file %c%s%c", '"', '*', str1, '"', '"', str2, '"', '"', path, '"');
                    handleReplace(commandAll, strlen(commandAll));
                }
            }

            return 0;
        }

        if (isAtFirst == 0)
        {
            char *tok = strtok(line, " ");

            while (tok != NULL)
            {
                wordCounter++;

                for (int i = 0; i <= strlen(tok) - strlen(str1); i++)
                {
                    if (strncmp(str1, tok + i, strlen(str1)) == 0)
                    {
                        caseCounter++;

                        if ((caseCounter == at || hasAt == 0) && hasAll == 0)
                        {
                            sprintf(commandRemove, "removestr --file %c%s%c --pos 1:%d -size %d -f", '"', path, '"', index + i, (int)strlen(tok) - i);
                            handleRemoveStr(commandRemove, strlen(commandRemove));
                            sprintf(commandInsert, "insertstr --file %c%s%c --str %s --pos 1:%d", '"', path, '"', str2, index + i);
                            handleInsertStr(commandInsert, strlen(commandInsert));

                            return 0;
                        }
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

            if (hasAll == 1)
            {
                for (int i = 0; i < caseCounter; i++)
                {
                    sprintf(commandAll, "replace --str1 %c%s%c%c --str2 %c%s%c --file %c%s%c", '"', str1, '*', '"', '"', str2, '"', '"', path, '"');
                    handleReplace(commandAll, strlen(commandAll));
                }
            }

            return 0;
        }
    }
}