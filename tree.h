#ifndef INCLUDE_CHECK
#include "packages.h"
#endif

int depth, i = 0, j = -1;
char *path;

int printTree()
{
    DIR *currDir;
    struct dirent *dir;
    int count = 0;
    char *pathTemp = calloc(1000, sizeof(char));
    currDir = opendir(path);

    if (currDir)
    {
        while ((dir = readdir(currDir)) != NULL)
        {
            if ((dir->d_type == DT_DIR && dir->d_name[0] != '.') || (dir->d_type == DT_REG))
                count++;
        }

        closedir(currDir);
    }

    currDir = opendir(path);

    if (currDir)
    {
        while ((dir = readdir(currDir)) != NULL)
        {
            if (dir->d_type == DT_DIR && dir->d_name[0] != '.')
            {
                --count;

                if (i > 0)
                    printf(" │");

                if (count == 0)
                    printf("%*c└── %s (DIR)\n", 3 * i + j, ' ', dir->d_name);
                else
                    printf("%*c├── %s (DIR)\n", 3 * i + j, ' ', dir->d_name);

                strcpy(pathTemp, path);

                path = strcat(path, "/");
                path = strcat(path, dir->d_name);

                if (depth > 0)
                {
                    i++;
                    j++;
                    depth--;
                    printTree();
                    strcpy(path, pathTemp);
                    i--;
                    j--;
                    depth++;
                }
            }
            if (dir->d_type == DT_REG)
            {
                --count;

                if (i > 0)
                    printf(" │");

                if (count == 0)
                    printf("%*c└── %s\n", 3 * i + j, ' ', dir->d_name);
                else
                    printf("%*c├── %s\n", 3 * i + j, ' ', dir->d_name);
            }
        }

        closedir(currDir);
    }
}

int handleTree(char *command, size_t commandLen)
{
    char *depthStr = calloc(10, sizeof(char));
    path = calloc(1000, sizeof(char));
    path[0] = '.';

    for (int i = 5; command[i] != ' ' && i != commandLen; i++)
    {
        depthStr[i - 5] = command[i];
    }

    depth = atoi(depthStr);
    if (depth == -1)
        depth = 2147483647;

    else if (depth < -1)
        return -1;

    printf(" .\n");
    printTree();
}