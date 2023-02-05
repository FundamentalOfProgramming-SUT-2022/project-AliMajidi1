#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int **handleFind2(char *str, char *path)
{
    FILE *textFile = fopen(path, "r");
    char *line = calloc(100000, sizeof(char));
    int **result = calloc(100000, sizeof(int *));
    int lineCount = 0;
    int cases = 0;
    int casesInSameLine = 0;
    char *returnFgets;

    while (1)
    {

        returnFgets = fgets(line, 100000, textFile);
        casesInSameLine = 0;

        if (returnFgets == NULL && cases != 0)
            return result;

        if (returnFgets == NULL && cases == 0)
            return NULL;

        for (int i = 0; i <= strlen(line) - strlen(str); i++)
        {
            if (strncmp(line + i, str, strlen(str)) == 0)
            {
                cases++;

                if (casesInSameLine == 0)
                    result[lineCount] = calloc(100000, sizeof(int));

                result[lineCount][casesInSameLine++] = i + 1;
            }
        }

        lineCount++;
    }
}