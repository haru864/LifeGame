#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

using namespace std;

int GAME_SCREEN_WIDTH;
int GAME_SCREEN_HEIGHT;
int cursorX = 0;
int cursorY = 0;
int COMMENT_ROW = 3;

// 生存 -> #
// 死亡 -> .

bool moveCursor(char c);
void checkCoordinate();

int main()
{
    initscr();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    GAME_SCREEN_HEIGHT = yMax - COMMENT_ROW;
    GAME_SCREEN_WIDTH = xMax;

    refresh();

    while (true)
    {
        int y, x;
        for (y = 0; y < GAME_SCREEN_HEIGHT; y++)
        {
            for (x = 0; x < GAME_SCREEN_WIDTH; x++)
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
        mvprintw(GAME_SCREEN_HEIGHT + 0, 0, "Push 'W', 'A', 'S', or 'D' to move your cursor.");
        mvprintw(GAME_SCREEN_HEIGHT + 1, 0, "If you want to quit game, please push 'Q'.");
        mvprintw(GAME_SCREEN_HEIGHT + 2, 0, "> ");
        if (moveCursor((char)getch()) == false)
        {
            break;
        }
    }

    endwin();
    printf("Thank you for playing!\n");

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
        // printf("Push 'W', 'A', 'S', 'D', or 'Q'.\n");
        break;
    }

    checkCoordinate();

    return true;
}

void checkCoordinate()
{
    if (cursorX < 0)
        cursorX = 0;
    if (cursorX >= GAME_SCREEN_WIDTH)
        cursorX = GAME_SCREEN_WIDTH - 1;
    if (cursorY < 0)
        cursorY = 0;
    if (cursorY >= GAME_SCREEN_HEIGHT)
        cursorY = GAME_SCREEN_HEIGHT - 1;
    return;
}