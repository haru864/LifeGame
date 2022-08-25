#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <vector>

using namespace std;

int GAME_SCREEN_WIDTH;
int GAME_SCREEN_HEIGHT;
int cursorX = 0;
int cursorY = 0;
vector<vector<int>> CELLS;
const int COMMENT_ROW = 4;
string MESSAGES[COMMENT_ROW] = {"Push 'W', 'A', 'S', or 'D' to move your cursor.",
                                "Push SPACE to revive current cell.",
                                "If you want to quit game, please push 'Q'.",
                                "> "};

// 生存 -> #
// 死亡 -> .
// カーソル -> o

bool moveCursor(char c, int y, int x);
void checkCoordinate();
void initCells();

int main()
{
    initscr();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    GAME_SCREEN_HEIGHT = yMax - COMMENT_ROW;
    GAME_SCREEN_WIDTH = xMax;

    initCells();
    mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 30, "%s", to_string(yMax).c_str());
    mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 25, "%s", to_string(xMax).c_str());
    mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 20, "%s", to_string(GAME_SCREEN_HEIGHT).c_str());
    mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 15, "%s", to_string(GAME_SCREEN_WIDTH).c_str());
    mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 10, "%s", to_string(CELLS.size()).c_str());
    mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 5, "%s", to_string(CELLS.at(0).size()).c_str());

    while (true)
    {
        resize_term(yMax, xMax);

        int y, x;
        for (y = 0; y < GAME_SCREEN_HEIGHT; y++)
        {
            for (x = 0; x < GAME_SCREEN_WIDTH; x++)
            {
                if (x == cursorX && y == cursorY)
                {
                    mvprintw(y, x, "o");
                }
                else if (CELLS.at(y).at(x) == 1)
                {
                    mvprintw(y, x, "#");
                }
                else
                {
                    mvprintw(y, x, ".");
                }
            }
        }
        refresh();

        for (int i = 0; i < COMMENT_ROW; i++)
        {
            char *cstr = new char[MESSAGES[i].size() + 1];
            cstr = strcpy(cstr, MESSAGES[i].c_str());
            mvprintw(GAME_SCREEN_HEIGHT + i, 0, "%s", cstr);
        }

        if (moveCursor((char)getch(), cursorY, cursorX) == false)
        {
            break;
        }
    }

    endwin();
    printf("Thank you for playing!\n");

    return EXIT_SUCCESS;
}

bool moveCursor(char c, int y, int x)
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
    case ' ':
        mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 40, "y:%s x:%s", to_string(y).c_str(), to_string(x).c_str());
        CELLS.at(y).at(x) = 1;
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

void initCells()
{
    CELLS.resize(GAME_SCREEN_HEIGHT, vector<int>(GAME_SCREEN_WIDTH, 0));
}