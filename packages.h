#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define INCLUDE_CHECK 1

int dirExists(char *dir)
{
    FILE *dirToCheck = fopen(dir, "r");
    if (dirToCheck == NULL)
        return 0;
    return 1;
}

void creatDir(char *dir)
{
    mkdir(dir,ACCESSPERMS);
}

int fileExists(char *file)
{
    if (!access(file, F_OK))
        return 1;
    return 0;
}

int saveCurrentStatus(char *path)
{
    if (!fileExists(path))
        return -1;
        

    FILE *undoFile = fopen("root/.tmp/.undo.txt", "w");
    FILE *currentFile = fopen(path, "r");

    char buff;

    while (1)
    {
        buff = getc(currentFile);
        if (buff == EOF)
            break;
        putc(buff, undoFile);
    }

    fclose(currentFile);
    fclose(undoFile);
}