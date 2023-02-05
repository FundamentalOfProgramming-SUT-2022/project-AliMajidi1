#include "createfile.h"
#include "savecurrent.h"
#include "patherror.h"
#include "insertstr.h"
#include "cat.h"
#include "removestr.h"
#include "copy.h"
#include "cutstr.h"
#include "paste.h"
#include "find.h"
#include "replace.h"
#include "autoindent.h"
#include "tree.h"
#include "grep.h"
#include "compare.h"
#include "undo.h"

int main()
{
    while (1)
    {
        size_t commandMaxLen = 50000, commandLen;
        char *command = calloc(commandMaxLen, sizeof(char));
        int returnVal;

        fgets(command, commandMaxLen, stdin);

        commandLen = strlen(command) - 1;
        command[commandLen] = '\0';

        if (commandLen == 0)
        {
            free(command);
            printf("invalid command\n");

            continue;
        }

        char *commandType = strdup(command);
        commandType = strtok(commandType, " ");

        // start createfile, syntax = doc

        if (strcmp(commandType, "createfile") == 0)
        {
            returnVal = handleCreteFile(command, commandLen);

            if (returnVal == -1)
                printf("File found\n");
        }

        // start insertstr, syntax = doc

        else if (strcmp(commandType, "insertstr") == 0)
        {
            returnVal = handleInsertStr(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start cat, syntax = doc

        else if (strcmp(commandType, "cat") == 0)
        {
            returnVal = handleCatFile(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start removestr, syntax = doc

        else if (strcmp(commandType, "removestr") == 0)
        {
            returnVal = handleRemoveStr(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start copystr, syntax = doc

        else if (strcmp(commandType, "copystr") == 0)
        {
            returnVal = handleCopyStr(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start cutstr, syntax = doc

        else if (strcmp(commandType, "cutstr") == 0)
        {
            returnVal = handleCutStr(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start pastestr, syntax = doc

        else if (strcmp(commandType, "pastestr") == 0)
        {
            returnVal = handlePasteStr(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start find, syntax = doc

        else if (strcmp(commandType, "find") == 0)
        {
            returnVal = handleFind(command, commandLen);

            if (returnVal == -1)
                printf("Interference occurred\n\n");

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start replace, syntax = doc

        else if (strcmp(commandType, "replace") == 0)
        {
            returnVal = handleReplace(command, commandLen);

            if (returnVal == -1)
                printf("Interference occurred\n");

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start grep, syntax = doc

        else if (strcmp(commandType, "grep") == 0)
        {
            returnVal = handleGrep(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start undo, syntax = doc

        else if (strcmp(commandType, "undo") == 0)
        {
            returnVal = handleUndo(command, commandLen);

            if (returnVal == -1)
                printf("No History\n");

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start auto-indent, syntax = doc

        else if (strcmp(commandType, "auto-indent") == 0)
        {
            returnVal = handleAutoIndent(command, commandLen);

            if (returnVal == -1)
                printf("file is not compatible to format\n");

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start compare, syntax = doc

        else if (strcmp(commandType, "compare") == 0)
        {
            returnVal = handleCompare(command, commandLen);

            if (returnVal == -2)
                printf("File not found\n");

            if (returnVal == -3)
                printf("Wrong directory\n");
        }

        // start tree, syntax = doc

        else if (strcmp(commandType, "tree") == 0)
        {
            returnVal = handleTree(command, commandLen);

            if (returnVal == -1)
                printf("invalid depth\n");
        }

        else
            printf("invalid command\n");
    }
}
