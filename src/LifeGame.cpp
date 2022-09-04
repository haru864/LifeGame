#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include <unistd.h>
#include <thread>

#define OPERATION_MODE 0
#define CYCLE_MODE 1
#define ALLOWED_OPTIONS "h:w:"

bool GAME_MODE; // 0: initial sequence, player select living cells
                // 1: simulation sequence, player can start life-cycle and push 'q' to quit game.
int GAME_SCREEN_WIDTH;
int GAME_SCREEN_HEIGHT;
int CURSOR_X = 0;
int CURSOR_Y = 0;
std::vector<std::vector<int>> CELLS;
const int MAX_COMMENT_ROW = 4;
std::string MESSAGES_OPERATION[4] = {"Push 'W', 'A', 'S', or 'D' to move your cursor.",
                                     "Push SPACE to revive cell, 'B' to start life-cycle.",
                                     "If you want to quit game, please push 'Q'.",
                                     "> "};
std::string MESSAGES_CYCLE[3] = {"Game of Life started, cells increase or decrease automatically.",
                                 "If you want to quit game, then push 'Q'.",
                                 "> "};

// 生存 -> #
// 死亡 -> .
// カーソル -> o

void init(int, char **);
void printScreen();
void printUserGuide();
void printCells();
void readKey();
void getKeyInput();
void movePosition(unsigned char);
void checkCoordinate();
void initCells();
void judgeDeadOrAlive();
int getAdjacentLivesCount(int, int);

int main(int argc, char *argv[])
{
    // intialize game screen size
    init(argc, argv);

    // multi threading, one thread is printting cells and guidance,
    // another thread is processing keyboard inout
    std::thread printScreenWorker(printScreen);
    std::thread readKeyWorker(readKey);
    printScreenWorker.join();
    readKeyWorker.join();

    // close screen
    endwin();
    printf("Thank you for playing!\n");

    return EXIT_SUCCESS;
}

void init(int argc, char *argv[])
{
    // open screen
    initscr();

    // initialize game mode
    GAME_MODE = OPERATION_MODE;

    // get default tty screen size
    int maxHeight, maxWidth;
    getmaxyx(stdscr, maxHeight, maxWidth);
    maxHeight -= MAX_COMMENT_ROW;

    // get screen size that player specified through command-line option
    int height, width;
    height = width = 0;
    int opt;
    while ((opt = getopt(argc, argv, ALLOWED_OPTIONS)) != -1)
    {
        switch (opt)
        {
        case 'h':
            height = atoi(optarg);
            break;
        case 'w':
            width = atoi(optarg);
            break;
        case '?':
            endwin();
            exit(1);
        }
    }

    // determine screen size
    GAME_SCREEN_HEIGHT = height > 0 ? std::min(height, maxHeight) : maxHeight;
    GAME_SCREEN_WIDTH = width > 0 ? std::min(width, maxWidth) : maxWidth;

    initCells();
}

void printScreen()
{
    while (true)
    {
        // when GAME_MODE id CYCLE_MODE, then making cells alive or dead
        if (GAME_MODE == CYCLE_MODE)
        {
            judgeDeadOrAlive();

            // pause for 0.5 sec, to make it easy to see how cells are increasing/decreasing
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        // print alive or dead cell
        printCells();

        // print how to play
        printUserGuide();
    }
}

void printCells()
{
    for (int y = 0; y < GAME_SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < GAME_SCREEN_WIDTH; x++)
        {
            if (GAME_MODE == OPERATION_MODE && x == CURSOR_X && y == CURSOR_Y)
            {
                mvprintw(y, x, "o");
            }
            else if (CELLS.at(y).at(x) > 0)
            {
                mvprintw(y, x, "#");
            }
            else
            {
                mvprintw(y, x, ".");
            }
            // if ((x == CURSOR_X && y == CURSOR_Y) && (GAME_MODE == OPERATION_MODE))
            // {
            //     mvprintw(y, x, "*");
            // }
            // else
            // {
            //     mvprintw(y, x, "%s", to_string(CELLS.at(y).at(x)).c_str());
            // }
        }
    }
    refresh();
}

void readKey()
{
    while (true)
    {
        // wait and reflect keyboard input
        getKeyInput();
    }
}

void printUserGuide()
{
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
    refresh();
}

void getKeyInput()
{
    unsigned char keyInput = getchar();

    if (keyInput == 'q')
    {
        // close screen
        endwin();
        printf("Thank you for playing!\n");
        fflush(stdout);
        exit(0);
    }

    if (GAME_MODE == OPERATION_MODE)
    {
        // move cursor within range
        movePosition(keyInput);
    }
}

void movePosition(unsigned char key)
{
    switch (key)
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
        CELLS.at(CURSOR_Y).at(CURSOR_X) = 1;
        break;
    case 'b':
        GAME_MODE = CYCLE_MODE;
        break;
    default:
        break;
    }

    checkCoordinate();
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
    CELLS.resize(GAME_SCREEN_HEIGHT, std::vector<int>(GAME_SCREEN_WIDTH, 0));
}

void judgeDeadOrAlive()
{
    std::vector<std::vector<int>> tempVec;
    tempVec.resize(GAME_SCREEN_HEIGHT, std::vector<int>(GAME_SCREEN_WIDTH, 0));

    for (int curr_y = 0; curr_y < GAME_SCREEN_HEIGHT; curr_y++)
    {
        for (int curr_x = 0; curr_x < GAME_SCREEN_WIDTH; curr_x++)
        {
            // 誕生：死んでいるセルに隣接する生きたセルがちょうど3つあれば、次の世代が誕生する。
            if ((CELLS.at(curr_y).at(curr_x) == 0) && (getAdjacentLivesCount(curr_y, curr_x) == 3))
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
        }
    }
    CELLS = tempVec;
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