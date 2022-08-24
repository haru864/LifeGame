#include <ncurses.h>

using namespace std;

int main()
{
    initscr();

    int y, x, yBeg, xBeg, yMax, xMax;

    WINDOW *win = newwin(10, 20, 10, 10);
    box(win, 103, 104);
    refresh();
    wrefresh(win);

    getyx(stdscr, y, x);
    getbegyx(stdscr, yBeg, xBeg);
    getmaxyx(stdscr, yMax, xMax);

    printw("%d %d %d %d %d %d", y, x, yBeg, xBeg, yMax, xMax);
    mvprintw(yMax / 2, xMax / 2, "hello");

    getch();
    endwin();

    return 0;
}