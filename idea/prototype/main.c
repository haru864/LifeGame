#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <ncurses.h>

#define FIELD_WIDTH 24
#define FIELD_HEIGHT 24

int cursorX = 0;
int cursorY = 0;

// 生存 -> #
// 死亡 -> .

bool moveCursor(char c);
void checkCoordinate();

int main()
{
    // manコマンドみたいにターミナル画面を占有したい

    while (true)
    {
        for (int y = 0; y < FIELD_HEIGHT; y++)
        {
            for (int x = 0; x < FIELD_WIDTH; x++)
            {
                if (x == cursorX && y == cursorY)
                {
                    printf("#");
                }
                else
                {
                    printf(".");
                }
            }
            printf("\n");
        }

        printf("Push 'W', 'A', 'S', or 'D' to move your cursor.\n");
        printf("If you want to quit game, please push 'Q'.\n");
        printf("> ");
        if (moveCursor((char)getchar()) == false)
        {
            break;
        }
    }

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
        printf("Push 'W', 'A', 'S', 'D', or 'Q'.\n");
        break;
    }

    checkCoordinate();

    return true;
}

void checkCoordinate()
{
    if (cursorX < 0)
        cursorX = 0;
    if (cursorX > FIELD_WIDTH)
        cursorX = FIELD_WIDTH;
    if (cursorY < 0)
        cursorY = 0;
    if (cursorY > FIELD_HEIGHT)
        cursorY = FIELD_HEIGHT;
    return;
}