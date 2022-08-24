#include <ncurses.h>

using namespace std;

int main()
{
    // start_ncurses(true, true);
    initscr();

    if (!has_colors())
    {
        printw("Terminal does not support color.\n");
        getch();
        return -1;
    }
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_WHITE);

    attron(COLOR_PAIR(1));
    printw("This is some text.\n");
    attroff(COLOR_PAIR(1));

    getch();

    endwin();

    return 0;
}