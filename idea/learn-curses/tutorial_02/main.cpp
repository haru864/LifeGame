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

    box(win, 0, 0);
    wrefresh(win);

    int c = getch();

    endwin();

    return EXIT_SUCCESS;
}