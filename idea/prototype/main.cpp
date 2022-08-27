#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <vector>

using namespace std;

#define OPERATION_MODE 0
#define CYCLE_MODE 1

bool GAME_MODE; // 0: initial sequence, player select living cells
                // 1: simulation sequence, player can start life-cycle and push 'q' to quit game.
int GAME_SCREEN_WIDTH;
int GAME_SCREEN_HEIGHT;
int CURSOR_X = 0;
int CURSOR_Y = 0;
vector<vector<int>> CELLS;
const int MAX_COMMENT_ROW = 4;
string MESSAGES_OPERATION[4] = {"Push 'W', 'A', 'S', or 'D' to move your cursor.",
                                "Push SPACE to revive cell, 'B' to start life-cycle.",
                                "If you want to quit game, please push 'Q'.",
                                "> "};
string MESSAGES_CYCLE[3] = {"Game of Life started.",
                            "If you want to quit game, then push 'Q'.",
                            "> "};

// 生存 -> #
// 死亡 -> .
// カーソル -> o

bool moveCursor(char c, int y, int x);
void checkCoordinate();
void initCells();
void initCells(int y, int x);
int getAdjacentLivesCount(int y, int x);

int main()
{
    initscr();
    GAME_MODE = OPERATION_MODE;

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    // GAME_SCREEN_HEIGHT = yMax - MAX_COMMENT_ROW;
    // GAME_SCREEN_WIDTH = xMax;
    GAME_SCREEN_HEIGHT = 5;
    GAME_SCREEN_WIDTH = 5;

    initCells();
    // mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 30, "%s", to_string(yMax).c_str());
    // mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 25, "%s", to_string(xMax).c_str());
    // mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 20, "%s", to_string(GAME_SCREEN_HEIGHT).c_str());
    // mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 15, "%s", to_string(GAME_SCREEN_WIDTH).c_str());
    // mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 10, "%s", to_string(CELLS.size()).c_str());
    // mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 5, "%s", to_string(CELLS.at(0).size()).c_str());

    while (true)
    {
        resize_term(yMax, xMax);

        // print cells on screen
        for (int y = 0; y < GAME_SCREEN_HEIGHT; y++)
        {
            for (int x = 0; x < GAME_SCREEN_WIDTH; x++)
            {
                // if (x == CURSOR_X && y == CURSOR_Y)
                // {
                //     mvprintw(y, x, "o");
                // }
                // else if (CELLS.at(y).at(x) > 0)
                // {
                //     mvprintw(y, x, "#");
                // }
                // else
                // {
                //     mvprintw(y, x, ".");
                // }
                if ((x == CURSOR_X && y == CURSOR_Y) && (GAME_MODE == OPERATION_MODE))
                {
                    mvprintw(y, x, "*");
                }
                else
                {
                    mvprintw(y, x, "%s", to_string(CELLS.at(y).at(x)).c_str());
                }
            }
        }

        // print explanation
        int pos_y, pos_x;
        for (int i = 0; i < MAX_COMMENT_ROW; i++)
        {
            auto messages = (GAME_MODE == OPERATION_MODE ? MESSAGES_OPERATION : MESSAGES_CYCLE);
            move(GAME_SCREEN_HEIGHT + i, 0);
            clrtoeol();
            if (messages[i].c_str() != NULL)
            {
                printw("%s", messages[i].c_str());
                getyx(stdscr, pos_y, pos_x);
            }
        }
        move(pos_y, pos_x);

        // when detect key pushed, move cursor or revive cell
        if (moveCursor((char)getch(), CURSOR_Y, CURSOR_X) == false)
        {
            break;
        }

        // if life-cycle started, draw cells slowly
        if (GAME_MODE == CYCLE_MODE)
        {
            system("sleep 1 > /dev/null &");
        }
    }

    // close screen
    endwin();
    printf("Thank you for playing!\n");

    return EXIT_SUCCESS;
}

bool moveCursor(char c, int y, int x)
{
    if ((GAME_MODE != OPERATION_MODE) && (c != 'q'))
        return true;

    switch (c)
    {
    case 'w':
        CURSOR_Y--;
        break;
    case 'a':
        CURSOR_X--;
        break;
    case 's':
        CURSOR_Y++;
        break;
    case 'd':
        CURSOR_X++;
        break;
    case ' ':
        // mvprintw(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH - 40, "y:%s x:%s", to_string(y).c_str(), to_string(x).c_str());
        CELLS.at(y).at(x) = 1;
        break;
    case 'q':
        return false;
        break;
    case 'b':
        GAME_MODE = CYCLE_MODE;
        return true;
        break;
    default:
        break;
    }

    checkCoordinate();
    initCells(CURSOR_Y, CURSOR_X);

    return true;
}

void checkCoordinate()
{
    if (CURSOR_X < 0)
        CURSOR_X = 0;
    if (CURSOR_X >= GAME_SCREEN_WIDTH)
        CURSOR_X = GAME_SCREEN_WIDTH - 1;
    if (CURSOR_Y < 0)
        CURSOR_Y = 0;
    if (CURSOR_Y >= GAME_SCREEN_HEIGHT)
        CURSOR_Y = GAME_SCREEN_HEIGHT - 1;
    return;
}

void initCells()
{
    CELLS.resize(GAME_SCREEN_HEIGHT, vector<int>(GAME_SCREEN_WIDTH, 0));
}

void initCells(int y, int x)
{
    vector<vector<int>> tempVec;
    tempVec.resize(GAME_SCREEN_HEIGHT, vector<int>(GAME_SCREEN_WIDTH, 0));

    for (int curr_y = 0; curr_y < GAME_SCREEN_HEIGHT; curr_y++)
    {
        for (int curr_x = 0; curr_x < GAME_SCREEN_WIDTH; curr_x++)
        {
            if ((curr_y == y) && (curr_x == x))
            {
                tempVec.at(curr_y).at(curr_x) = CELLS.at(curr_y).at(curr_x);
            }
            // 誕生：死んでいるセルに隣接する生きたセルがちょうど3つあれば、次の世代が誕生する。
            else if ((CELLS.at(curr_y).at(curr_x) == 0) && (getAdjacentLivesCount(curr_y, curr_x) == 3))
            {
                tempVec.at(curr_y).at(curr_x) = 1;
            }
            // 生存：生きているセルに隣接する生きたセルが2つか3つならば、次の世代でも生存する。
            else if ((CELLS.at(curr_y).at(curr_x) != 0) && ((getAdjacentLivesCount(curr_y, curr_x) == 2) || (getAdjacentLivesCount(curr_y, curr_x) == 3)))
            {
                tempVec.at(curr_y).at(curr_x) = CELLS.at(curr_y).at(curr_x);
            }
            // 過疎：生きているセルに隣接する生きたセルが1つ以下ならば、過疎により死滅する。
            else if ((CELLS.at(curr_y).at(curr_x) != 0) && (getAdjacentLivesCount(curr_y, curr_x) <= 1))
            {
                tempVec.at(curr_y).at(curr_x) = 0;
            }
            // 過密：生きているセルに隣接する生きたセルが4つ以上ならば、過密により死滅する。
            else if ((CELLS.at(curr_y).at(curr_x) != 0) && (getAdjacentLivesCount(curr_y, curr_x) >= 4))
            {
                tempVec.at(curr_y).at(curr_x) = 0;
            }
            else
            {
                tempVec.at(curr_y).at(curr_x) = CELLS.at(curr_y).at(curr_x);
            }
        }
    }
    for (int i = 0; i < GAME_SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < GAME_SCREEN_WIDTH; j++)
        {
            mvprintw(i, j + 10, "%s", to_string(CELLS.at(i).at(j)).c_str());
            mvprintw(i, j + 20, "%s", to_string(tempVec.at(i).at(j)).c_str());
        }
    }
    CELLS = tempVec;
    for (int i = 0; i < GAME_SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < GAME_SCREEN_WIDTH; j++)
        {
            mvprintw(i, j + 30, "%s", to_string(CELLS.at(i).at(j)).c_str());
        }
    }
}

int getAdjacentLivesCount(int y, int x)
{
    int count = 0;
    for (int Y = -1; Y <= 1; Y++)
    {
        for (int X = -1; X <= 1; X++)
        {
            if ((Y == 0) && (X == 0))
                continue;
            int currY = (y + Y + GAME_SCREEN_HEIGHT) % GAME_SCREEN_HEIGHT;
            int currX = (x + X + GAME_SCREEN_WIDTH) % GAME_SCREEN_WIDTH;
            if (CELLS.at(currY).at(currX) > 0)
                count++;
        }
    }
    return count;
}