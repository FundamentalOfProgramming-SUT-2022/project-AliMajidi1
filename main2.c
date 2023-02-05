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
#include "autoindent.h"
#include "tree.h"
#include "grep.h"
#include "compare.h"
#include "undo.h"
#include "find2.h"

int maxX, maxY, cursorX, cursorY, shift = 0;
char mode = 'n';
char *path;
int *lineLimit;
int lineCount = 0;
int firstCaseY, firstCaseX;

void printLineNo(int start, int end);
void printMode(char mode);
void printPath(char *path, int saved);
void commandBar();
void commandMode(int ch);
void undoPhase2();
void checkFileDetails();
void printFile();
void cleanEditor();
void clearAllSelected();
void insertMode();

int main()
{
    char ch;
    path = calloc(1000, sizeof(char));
    lineLimit = calloc(100000000, sizeof(char));
    int selectedCharsCount = 0;
    int isForward = 1;

    initscr();
    setlocale(LC_ALL, "");
    raw();
    keypad(stdscr, TRUE);
    noecho();

    move(0, 6);
    getmaxyx(stdscr, maxY, maxX);
    getyx(stdscr, cursorY, cursorX);

    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    commandBar();
    printMode(mode);
    strcpy(path, "root/welcome.txt");
    printPath(path, 1);
    checkFileDetails();
    printFile(1, (int)fmin(maxY - 2, lineCount));
    printLineNo(1, (int)fmin(maxY - 2, lineCount));
    int cursorYBV, cursorXBV;
    while (1)
    {
        ch = getch();
        getyx(stdscr, cursorY, cursorX);

        if (ch != 'n' || mode != 'n')
            clearAllSelected();

        // exit from visual or insert if normal exit

        if (ch == 27)
        {
            if (mode == 'n')
            {
                endwin();
                return 0;
            }

            mode = 'n';
            printMode(mode);
        }

        // up

        if (ch == 'k')
        {
            if (cursorX - 6 >= lineLimit[cursorY + shift - 1])
                move(cursorY - 1, lineLimit[cursorY + shift - 1] + 6);
            else
                move(cursorY - 1, cursorX);

            if (cursorY - 5 < 0 && shift > 0)
            {
                shift--;
                cleanEditor();
                printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
                printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
            }
        }

        // down

        if (ch == 'j')
        {
            if (cursorY + shift + 2 <= lineCount)
            {
                if (cursorX - 6 >= lineLimit[cursorY + shift + 1])
                    move(cursorY + 1, lineLimit[cursorY + shift + 1] + 6);
                else
                    move(cursorY + 1, cursorX);
            }

            if (cursorY > maxY - 8 && cursorY + shift + 2 <= lineCount)
            {
                shift++;
                cleanEditor();
                printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
                printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
            }
        }

        // left

        if (ch == 'h')
        {
            if (cursorX > 6)
                move(cursorY, cursorX - 1);
        }

        // right

        if (ch == 'l')
        {
            if (cursorX - 6 < lineLimit[cursorY + shift])
                move(cursorY, cursorX + 1);
        }

        // visual mode

        if (ch == 'v' && mode != 'v')
        {
            mode = 'v';
            printMode(mode);

            cursorXBV = cursorX;
            cursorYBV = cursorY;
        }

        if (mode == 'v')
        {

            getyx(stdscr, cursorY, cursorX);

            selectedCharsCount = 0;
            isForward = 1;

            if (cursorY > cursorYBV)
            {
                int cursorYBVTmp = cursorYBV;

                mvchgat(cursorYBV, cursorXBV, lineLimit[cursorYBV + shift] - cursorXBV + 7, A_BOLD, 3, NULL);

                selectedCharsCount += lineLimit[cursorYBV + shift] - cursorXBV + 7;

                while (1)
                {
                    if (cursorYBVTmp < cursorY - 1)
                    {
                        cursorYBVTmp++;

                        mvchgat(cursorYBVTmp, 6, lineLimit[cursorYBVTmp + shift] + 1, A_BOLD, 3, NULL);

                        selectedCharsCount += lineLimit[cursorYBVTmp + shift] + 1;
                    }
                    else
                        break;
                }

                mvchgat(cursorY, 6, cursorX - 6, A_BOLD, 3, NULL);

                selectedCharsCount += cursorX - 6;

                move(cursorY, cursorX);
            }

            else if (cursorY < cursorYBV)
            {
                int cursorYBVTmp = cursorYBV;
                isForward = -1;

                mvchgat(cursorYBV, 6, cursorXBV - 6, A_BOLD, 3, NULL);

                selectedCharsCount += cursorXBV - 6;

                while (1)
                {
                    if (cursorYBVTmp > cursorY + 1)
                    {
                        cursorYBVTmp--;

                        mvchgat(cursorYBVTmp, 6, lineLimit[cursorYBVTmp + shift] + 1, A_BOLD, 3, NULL);

                        selectedCharsCount += lineLimit[cursorYBVTmp + shift] + 1;
                    }
                    else
                        break;
                }

                mvchgat(cursorY, cursorX + 1, lineLimit[cursorY + shift] - cursorX + 6, A_BOLD, 3, NULL);

                selectedCharsCount += lineLimit[cursorY + shift] - cursorX + 6;

                move(cursorY, cursorX);
            }
            else
            {
                if (cursorX > cursorXBV)
                {
                    mvchgat(cursorY, cursorXBV, cursorX - cursorXBV, A_BOLD, 3, NULL);

                    selectedCharsCount += cursorX - cursorXBV;
                }

                if (cursorX < cursorXBV)
                {
                    mvchgat(cursorY, cursorX + 1, cursorXBV - cursorX - 1, A_BOLD, 3, NULL);

                    selectedCharsCount += cursorXBV - cursorX - 1;

                    isForward = -1;
                }

                move(cursorY, cursorX);
            }
        }

        // copy

        if (ch == 'y' && mode == 'v')
        {
            char *command = calloc(10000, sizeof(char));

            sprintf(command, "copystr --file %c%s%c --pos %d:%d -size %d -%c", '"', path, '"', cursorYBV + shift + 1, cursorXBV - 6, selectedCharsCount, isForward == 1 ? 'f' : 'b');
            handleCopyStr(command, strlen(command));
            clearAllSelected();

            mode = 'n';

            printMode(mode);
        }

        // cut

        if (ch == 'd' && mode == 'v')
        {
            char *command = calloc(10000, sizeof(char));

            sprintf(command, "cutstr --file %c%s%c --pos %d:%d -size %d -%c", '"', path, '"', cursorYBV + shift + 1, cursorXBV - 6, selectedCharsCount, isForward == 1 ? 'f' : 'b');
            handleCutStr(command, strlen(command));
            clearAllSelected();

            mode = 'n';

            printMode(mode);
            cleanEditor();
            checkFileDetails();
            printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
            printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
        }

        // paste

        if (ch == 'p' && mode == 'n')
        {
            char *command = calloc(10000, sizeof(char));

            sprintf(command, "pastestr --file %c%s%c --pos %d:%d", '"', path, '"', cursorY + shift + 1, cursorX - 6);
            handlePasteStr(command, strlen(command));
            clearAllSelected();
            cleanEditor();
            checkFileDetails();
            printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
            printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
        }

        // auto-indent

        if (ch == '=' && mode == 'n')
        {
            char *command = calloc(10000, sizeof(char));

            sprintf(command, "auto-indent %c%s%c", '"', path, '"');
            handleAutoIndent(command, strlen(command));
            clearAllSelected();
            cleanEditor();
            checkFileDetails();
            printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
            printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
        }

        if (ch == 'i' && mode == 'n')
        {
            mode = 'i';

            printMode(mode);
            insertMode();

            mode = 'n';

            printMode(mode);
        }

        if ((ch == ':' || ch == '/') && mode == 'n')
        {
            commandBar();
            commandMode(ch);
        }

        if (ch == 'u' && mode == 'n')
            undoPhase2();

        if (ch == 'n' && mode == 'n')
        {
            move(firstCaseY, firstCaseX);
        }

        refresh();
    }

    refresh();
    endwin();

    return 0;
}

void printLineNo(int start, int end)
{
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);

    for (int i = start; i <= end; i++)
    {
        mvprintw(i - start, 2, "%d", i);
        mvprintw(i - start, 0, "%c", '~');
        mvchgat(i - start, 0, 6, A_DIM, 1, NULL);
    }

    move(cursorY, cursorX);

    refresh();
}

void printMode(char mode)
{

    start_color();
    init_pair(2, COLOR_BLACK, COLOR_CYAN);

    if (mode == 'n')
        mvprintw(maxY - 2, 1, "--NORMAL--");
    if (mode == 'i')
        mvprintw(maxY - 2, 1, "--INSERT--");
    if (mode == 'v')
        mvprintw(maxY - 2, 1, "--VISUAL--");

    mvchgat(maxY - 2, 0, 12, A_BOLD, 2, NULL);
    move(cursorY, cursorX);
    refresh();
}

void printPath(char *path, int saved)
{

    start_color();

    move(maxY - 2, 12);
    clrtoeol();

    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    mvprintw(maxY - 2, 14, "%s", path);

    if (saved == -1)
        mvprintw(maxY - 2, 13, "+");

    mvchgat(maxY - 2, 12, -1, A_BOLD, 3, NULL);
    move(cursorY, cursorX);
    refresh();
}

void commandBar()
{
    move(maxY - 1, 0);
    clrtoeol();
    start_color();
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    mvchgat(maxY - 1, 0, -1, A_BOLD, 4, NULL);
    move(cursorY, cursorX);
}

void commandMode(int ch)
{
    int commandIndex = 0;
    char commandChar;
    char *command = calloc(10000, sizeof(char));

    start_color();
    attron(A_BOLD | COLOR_PAIR(4));

    mvprintw(maxY - 1, 0, "%c", ch);

    while (1)
    {
        commandChar = getch();

        if (commandChar == '\n')
        {
            move(cursorY, cursorX);
            break;
        }

        if (commandChar == 27)
        {
            commandBar();
            return;
        }

        const char *check = unctrl(commandChar);

        if (strlen(check) != 1)
            continue;

        command[commandIndex++] = commandChar;

        addch(commandChar);
        refresh();
    }

    attroff(A_BOLD | COLOR_PAIR(4));

    if (strcmp(command, "undo") == 0 && ch == ':')
    {
        undoPhase2();
    }

    if (strncmp(command, "saveas", 6) == 0 && ch == ':')
    {
        char *newPath = calloc(strlen(command), sizeof(char));
        int startPath = 7, pathIndex = 0;

        if (command[startPath] == '"')
        {
            if (command[startPath + 1] == '/')
                startPath += 2;
            else
                startPath++;

            while (command[startPath] != '"')
            {
                newPath[pathIndex++] = command[startPath++];
            }

            if (newPath[pathIndex - 1] == '/')
                newPath[pathIndex - 1] = '\0';
        }
        else
        {
            if (command[startPath] == '/')
                startPath++;

            while (command[startPath] != ' ' && startPath != strlen(command))
            {
                newPath[pathIndex++] = command[startPath++];
            }

            if (newPath[pathIndex - 1] == '/')
                newPath[pathIndex - 1] = '\0';
        }

        FILE *newFile = fopen(newPath, "w");
        fclose(newFile);

        copyFile(path, newPath);
        remove(path);
        strcpy(path, newPath);
        printPath(path, 1);
    }

    if (strncmp(command, "open", 4) == 0 && ch == ':')
    {
        char *newPath = calloc(strlen(command), sizeof(char));
        int startPath = 5, pathIndex = 0;

        if (command[startPath] == '"')
        {
            if (command[startPath + 1] == '/')
                startPath += 2;
            else
                startPath++;

            while (command[startPath] != '"')
            {
                newPath[pathIndex++] = command[startPath++];
            }

            if (newPath[pathIndex - 1] == '/')
                newPath[pathIndex - 1] = '\0';
        }
        else
        {
            if (command[startPath] == '/')
                startPath++;

            while (command[startPath] != ' ' && startPath != strlen(command))
            {
                newPath[pathIndex++] = command[startPath++];
            }

            if (newPath[pathIndex - 1] == '/')
                newPath[pathIndex - 1] = '\0';
        }

        FILE *newFile = fopen(newPath, "a+");

        fclose(newFile);
        strcpy(path, newPath);
        printPath(path, 1);
        cleanEditor();
        checkFileDetails();
        printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
        printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
    }

    if (ch == '/')
    {
        int **result = handleFind2(command, path);
        int cases = 0;

        if (result == NULL)
        {
            attron(A_BOLD | COLOR_PAIR(4));

            mvprintw(maxY - 1, 0, " Not found");

            attroff(A_BOLD | COLOR_PAIR(4));
            move(cursorY, cursorX);

            return;
        }

        for (int i = 0; i < 100000; i++)
        {
            if (result[i] != NULL)
                for (int j = 0; result[i][j] != 0; j++)
                {
                    cases++;

                    if (cases == 1)
                    {
                        firstCaseX = 6 + result[i][j] - 1;
                        firstCaseY = i + shift;
                    }

                    mvchgat(i + shift, 6 + result[i][j] - 1, strlen(command), A_BOLD, 3, NULL);
                    move(cursorY, cursorX);
                }
        }
    }
}

void undoPhase2()
{
    char *command = calloc(1000, sizeof(char));
    int returnVal;

    strcpy(command, "undo --file ");

    command[12] = '"';

    strcat(command, path);

    command[strlen(command)] = '"';

    returnVal = handleUndo(command, strlen(command));

    commandBar();
    start_color();
    attron(A_BOLD | COLOR_PAIR(4));

    if (returnVal == -1)
        mvprintw(maxY - 1, 1, "No History");

    if (returnVal == -2)
        mvprintw(maxY - 1, 1, "File not found");

    if (returnVal == -3)
        mvprintw(maxY - 1, 1, "Wrong directory");

    attroff(A_BOLD | COLOR_PAIR(4));

    if (returnVal == 0)
    {
        cleanEditor();
        checkFileDetails();
        printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
        printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
    }

    move(cursorY, cursorX);
}

void checkFileDetails()
{
    FILE *textFile = fopen(path, "r");
    char *returnFgets;
    char *line = calloc(1000000, sizeof(char));
    lineCount = 0;

    while (1)
    {
        returnFgets = fgets(line, 10000, textFile);

        if (returnFgets == NULL && line[strlen(line) - 1] != '\n')
            break;

        if (returnFgets == NULL && line[strlen(line) - 1] == '\n')
        {
            lineLimit[lineCount] = 0;
            lineCount++;
            break;
        }

        lineLimit[lineCount] = (int)strlen(line) - 1;
        lineCount++;
    }

    move(cursorY, cursorX);

    fclose(textFile);
}

void printFile(int firstLine, int endLine)
{
    FILE *textFile = fopen(path, "r");
    char *returnFgets;
    char *line = calloc(1000000, sizeof(char));
    int lineNum = 0;
    int yPos = 0;

    while (1)
    {
        returnFgets = fgets(line, 10000, textFile);

        if (returnFgets == NULL)
            break;

        lineNum++;

        if (lineNum <= endLine && lineNum >= firstLine)
            mvprintw(yPos++, 6, "%s", line);
    }

    move(cursorY, cursorX);
    fclose(textFile);
}

void cleanEditor()
{
    for (int i = 0; i < maxY - 2; i++)
    {
        move(i, 0);
        clrtoeol();
        move(cursorY, cursorX);
    }
}

void clearAllSelected()
{
    for (int i = 0; i < maxY - 2; i++)
        mvchgat(i, 6, -1, A_NORMAL, 0, NULL);

    move(cursorY, cursorX);
}

void insertMode()
{
    while (1)
    {
        int insertChar = getch();
        char *commandInsert = calloc(1000, sizeof(char));

        getyx(stdscr, cursorY, cursorX);

        if (insertChar == 27)
            break;

        if (insertChar == '\n')
        {
            move(cursorY + 1, 6);

            sprintf(commandInsert, "insertstr --file %c%s%c --str %c%c --pos %d:%d", '"', path, '"', 92, 'n', cursorY + shift + 1, cursorX - 6);

            getyx(stdscr, cursorY, cursorX);
        }
        else
        {
            addch(insertChar);
            getyx(stdscr, cursorY, cursorX);

            if (insertChar == ' ')
                sprintf(commandInsert, "insertstr --file %c%s%c --str %c%c%c --pos %d:%d", '"', path, '"', '"', insertChar, '"', cursorY + shift + 1, cursorX - 7);

            else
                sprintf(commandInsert, "insertstr --file %c%s%c --str %c --pos %d:%d", '"', path, '"', insertChar, cursorY + shift + 1, cursorX - 7);
        }

        handleInsertStr(commandInsert, strlen(commandInsert));
        cleanEditor();
        checkFileDetails();
        printFile(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);
        printLineNo(1 + shift, (int)fmin(maxY - 2, lineCount) + shift);

        move(cursorY, cursorX);
    }
}