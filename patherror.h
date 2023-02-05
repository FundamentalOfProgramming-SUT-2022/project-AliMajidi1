#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int pathErrorHandle(char *path)
{
    char *pathToCheck = calloc(strlen(path), sizeof(char));
    char *token = strdup(path);

    token = strtok(token, "/");
    strcat(pathToCheck, token);

    while (1)
    {
        if (strlen(pathToCheck) == strlen(path))
            if (!fileExists(pathToCheck))
                return -2;

        if (!dirExists(pathToCheck))
            return -3;

        token = strtok(NULL, "/");

        if (token == NULL)
            break;

        strcat(pathToCheck, "/");
        strcat(pathToCheck, token);
    }

    return 0;
}