#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

#define MAX_DIR_LEN 1000

int createFile(char *dir, char *file)
{

    char *token = strdup(dir);
    char *directoryToCheck = (char *)calloc(MAX_DIR_LEN, sizeof(char));

    token = strtok(token, "/");

    while (token != NULL)
    {
        strcat(directoryToCheck, token);
        strcat(directoryToCheck, "/");

        if (!dirExists(directoryToCheck))
        {
            creatDir(directoryToCheck);
        }

        token = strtok(NULL, "/");
    }

    strcat(directoryToCheck, file);

    if (fileExists(directoryToCheck))
        return -1;

    FILE *newFile = fopen(directoryToCheck, "w");

    if (newFile != NULL)
        return 1;
}