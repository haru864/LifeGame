#include <iostream>
#include <ncurses.h>

using namespace std;

int main()
{
    initscr();

    printw("Hello World!\n");

    refresh();

    int n = getchar();
    printw("%c", (char)n);

    getch();

    endwin();

    return EXIT_SUCCESS;
}
