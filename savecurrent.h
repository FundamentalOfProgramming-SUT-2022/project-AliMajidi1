#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

void copyFile(char *src, char *dest)
{
    FILE *srcFile = fopen(src, "r");
    FILE *destFile = fopen(dest, "w");

    char buff;

    while (1)
    {
        buff = getc(srcFile);
        if (buff == EOF)
            break;
        putc(buff, destFile);
    }

    fclose(srcFile);
    fclose(destFile);
}

int restoreLastStatus(char *path)
{

    char *undoFileDir = calloc(10000, sizeof(char));
    char *undoFilePath = calloc(10000, sizeof(char));
    char *extension = calloc(1000, sizeof(char));

    int lenCopy;
    int indexExtension;

    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '/')
        {
            lenCopy = i;
            break;
        }
    }

    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '.')
        {
            indexExtension = i;
            break;
        }
    }

    strncpy(undoFileDir, path, lenCopy);
    strcat(undoFileDir, "/.");
    strncat(undoFileDir, path + lenCopy + 1, strlen(path) - lenCopy - 1);
    strncpy(extension, path + indexExtension + 1, strlen(path) - indexExtension);

    if (!dirExists(undoFileDir))
        return -1;

    int lastHistory;

    for (int i = 1; i < 100000; i++)
    {
        char *temp = calloc(1000, sizeof(char));
        sprintf(temp, "/.%d.%s", i, extension);

        strcpy(undoFilePath, undoFileDir);
        strcat(undoFilePath, temp);

        if (!fileExists(undoFilePath))
        {
            lastHistory = i - 1;

            sprintf(temp, "/.%d.%s", i - 1, extension);

            strcpy(undoFilePath, undoFileDir);
            strcat(undoFilePath, temp);

            break;
        }
    }

    if (lastHistory == 0)
    {
        rmdir(undoFileDir);
        return -1;
    }

    copyFile(undoFilePath, path);

    remove(undoFilePath);

    if (lastHistory == 1)
        rmdir(undoFileDir);

    return 0;
}

void saveCurrentStatus(char *path)
{
    if (!fileExists(path))
        return;

    char *undoFileDir = calloc(10000, sizeof(char));
    char *undoFilePath = calloc(10000, sizeof(char));
    char *extension = calloc(1000, sizeof(char));

    int lenCopy;
    int indexExtension;
    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '/')
        {
            lenCopy = i;
            break;
        }
    }

    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '.')
        {
            indexExtension = i;
            break;
        }
    }

    strncpy(undoFileDir, path, lenCopy);
    strcat(undoFileDir, "/.");
    strncat(undoFileDir, path + lenCopy + 1, strlen(path) - lenCopy - 1);
    strncpy(extension, path + indexExtension + 1, strlen(path) - indexExtension);

    if (!dirExists(undoFileDir))
        creatDir(undoFileDir);

    for (int i = 1; i < 100000; i++)
    {
        char *temp = calloc(1000, sizeof(char));
        sprintf(temp, "/.%d.%s", i, extension);

        strcpy(undoFilePath, undoFileDir);
        strcat(undoFilePath, temp);

        if (!fileExists(undoFilePath))
        {
            FILE *undoFile = fopen(undoFilePath, "w");
            fclose(undoFile);
            copyFile(path, undoFilePath);
            break;
        }
        else
            continue;
    }
}