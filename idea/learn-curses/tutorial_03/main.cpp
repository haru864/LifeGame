#include <iostream>
#include <ncurses.h>

using namespace std;

int main()
{
    initscr();

    int height, width, start_y, start_x;
    height = 10;
    width = 20;
    start_x = start_y = 10;

    WINDOW *win = newwin(height, width, start_y, start_x);
    refresh();

    // box(win, 97, 98);
    int left, right, top, bottom, tlc, trc, blc, brc;
    left = right = 103;
    top = bottom = 104;
    tlc = trc = blc = brc = (int)'c';
    wborder(win, left, right, top, bottom, tlc, trc, blc, brc);
    wrefresh(win);

    int c = getch();

    endwin();

    return EXIT_SUCCESS;
}