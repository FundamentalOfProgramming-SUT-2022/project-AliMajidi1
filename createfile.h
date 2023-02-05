#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int dirExists(char *dir)
{
    FILE *dirToCheck = fopen(dir, "r");
    if (dirToCheck == NULL)
        return 0;
    return 1;
}

void creatDir(char *dir)
{
    mkdir(dir, ACCESSPERMS);
}

int fileExists(char *path)
{
    if (!access(path, F_OK))
        return 1;
    return 0;
}

int tokenizePath(char *path, size_t pathLen)
{
    char *pathToCheck = calloc(pathLen, sizeof(char));
    char *token = strdup(path);

    token = strtok(token, "/");
    strcat(pathToCheck, token);

    while (1)
    {
        if (strlen(pathToCheck) == pathLen)
        {
            if (fileExists(pathToCheck))
                return -1;
            else
            {
                fopen(pathToCheck, "w");
            }
        }

        if (!dirExists(pathToCheck))
            creatDir(pathToCheck);

        token = strtok(NULL, "/");

        if (token == NULL)
            break;

        strcat(pathToCheck, "/");
        strcat(pathToCheck, token);
    }

    return 0;
}

int handleCreteFile(char *command, size_t commandLen)
{

    char *path = calloc(commandLen, sizeof(char));
    int start = 18, pathIndex = 0;

    if (command[start] == '"')
    {
        if (command[start + 1] == '/')
            start += 2;
        else
            start++;

        while (command[start] != '"')
        {
            path[pathIndex++] = command[start++];
        }

        if (path[pathIndex - 1] == '/')
            path[pathIndex - 1] = '\0';
    }
    else
    {
        if (command[start] == '/')
            start++;

        while (command[start] != ' ' && start != commandLen)
        {
            path[pathIndex++] = command[start++];
        }

        if (path[pathIndex - 1] == '/')
            path[pathIndex - 1] = '\0';
    }

    return tokenizePath(path, strlen(path));
}
