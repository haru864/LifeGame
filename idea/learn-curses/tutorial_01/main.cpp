#include <iostream>
#include <ncurses.h>

using namespace std;

int main(int argc, char *argv[])
{
    initscr();

    int ROW = 0, COLUMN = 0;

    char *r, *c;
    if (argc >= 2)
    {
        r = argv[1];
        ROW = atoi(r);
    }
    if (argc >= 3)
    {
        c = argv[2];
        COLUMN = atoi(c);
    }

    move(ROW, COLUMN);

    printw("Hello World!");

    getch();

    endwin();

    return EXIT_SUCCESS;
}