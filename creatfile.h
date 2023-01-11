#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int dirExists(char *dir)
{
    FILE *dirToCheck = fopen(dir, "r");
    if (dirToCheck == NULL)
        return 0;
    return 1;
}

void creatDir(char *dir)
{
    mkdir(dir);
}

int fileExists(char *file)
{
    if (!access(file, F_OK))
        return 1;
    return 0;
}
int creatFile(int len, char *dir,char *file)
{
    char *token;
    char *directoryToCheck = calloc(len, sizeof(char));
    token = strtok(dir, "/");
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
    if(fileExists(directoryToCheck))
        return -1;
    FILE *newFile = fopen(directoryToCheck, "w");
    if(newFile != NULL)
        return 1;
}