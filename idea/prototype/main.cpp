#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int SRC_WIDTH;
int SRC_HEIGHT;
int cursorX = 0;
int cursorY = 0;
int COMMENT_ROW = 5;

// 生存 -> #
// 死亡 -> .

bool moveCursor(char c);
void checkCoordinate();

int main()
{
    initscr();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    SRC_HEIGHT = yMax - COMMENT_ROW;
    SRC_WIDTH = xMax;

    refresh();

    for (int y = 0; y < SRC_HEIGHT; y++)
    {
        for (int x = 0; x < SRC_WIDTH; x++)
        {
            if (x == cursorX && y == cursorY)
            {
                mvprintw(y, x, "#");
            }
            else
            {
                mvprintw(y, x, ".");
            }
        }
    }
    getch();

    // while (true)
    // {
    //     for (int y = 0; y < SRC_HEIGHT; y++)
    //     {
    //         for (int x = 0; x < SRC_WIDTH; x++)
    //         {
    //             move(y, x);
    //             if (x == cursorX && y == cursorY)
    //             {
    //                 printw("#");
    //             }
    //             else
    //             {
    //                 printw(".");
    //             }
    //         }
    //         printf("\n");
    //     }
    //     printf("Push 'W', 'A', 'S', or 'D' to move your cursor.\n");
    //     printf("If you want to quit game, please push 'Q'.\n");
    //     printf("> ");
    //     if (moveCursor((char)getchar()) == false)
    //     {
    //         break;
    //     }
    // }

    printf("Thank you for playing!\n");

    endwin();

    return EXIT_SUCCESS;
}

bool moveCursor(char c)
{
    switch (c)
    {
    case 'w':
        cursorY--;
        break;
    case 'a':
        cursorX--;
        break;
    case 's':
        cursorY++;
        break;
    case 'd':
        cursorX++;
        break;
    case 'q':
        return false;
        break;
    default:
        printf("Push 'W', 'A', 'S', 'D', or 'Q'.\n");
        break;
    }

    checkCoordinate();

    return true;
}

void checkCoordinate()
{
    if (cursorX < 0)
        cursorX = 0;
    if (cursorX > SRC_WIDTH)
        cursorX = SRC_WIDTH;
    if (cursorY < 0)
        cursorY = 0;
    if (cursorY > SRC_HEIGHT)
        cursorY = SRC_HEIGHT;
    return;
}