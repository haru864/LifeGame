#include <stdio.h>
#include <stdlib.h>
// #include <ncurses.h>

#define FIELD_WIDTH 24
#define FIELD_HEIGHT 24

int cursorX, cursorY;

// 生存 -> #
// 死亡 -> .

int main()
{
    // 別のターミナル画面を開いて、そこにゲーム画面を表示させたい
    // system("gnome-terminal");

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

    // getch();
    printf("If you want to quit game, please push RETURN button.");
    int USER_INPUT = getchar();
    printf("Thank you for playing!\n");

    return 0;
}