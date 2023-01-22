#ifndef INCLUDE_CHECK
#include "packages.h"
#endif
int insertStr(char *path, char *str, int linePos, int charPos)
{
    if (!fileExists(path))
        return -1;

    FILE *textFile = fopen(path, "r");
    FILE *tempFile = fopen("root/.tmp/.temp.txt", "w+");

    int currentLine = 1;
    int currentChar = 0;
    char buff;

    while (currentLine != linePos)
    {
        buff = fgetc(textFile);
        fputc(buff, tempFile);
        if (buff == '\n')
            currentLine++;
    }

    while (currentChar < charPos)
    {
        buff = fgetc(textFile);
        fputc(buff, tempFile);
        currentChar++;
    }
    fprintf(tempFile, "%s", str);

    while (1)
    {
        buff = fgetc(textFile);

        if (buff == EOF)
            break;
        fputc(buff, tempFile);
    }

    fclose(textFile);
    fclose(tempFile);

    FILE *textFileToEdit = fopen(path, "w");
    FILE *tempFileRead = fopen("root/.tmp/.temp.txt", "r");

    while (1)
    {
        buff = getc(tempFileRead);
        if (buff == EOF)
            break;
        putc(buff, textFileToEdit);
    }

    fclose(textFileToEdit);
    fclose(tempFileRead);
    remove("root/.tmp/.temp.txt");
}
