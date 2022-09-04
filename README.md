# lifegame

## what is lifegame?
&emsp; lifegame \- Conway's Game of Life on Linux Terminal

## SYNOPSIS
&emsp; lifegame [ \-h height ] [ \-w width ]

## DESCRIPTION
lifegame realize Conway's Game of Life on Linux Terminal Emulator.

Player can specify height and width of cell grid within defaule terminal height and width.

\-h and \-w are valid, other option is invalid.

Highly recommended to set above 20 both height and width.

Game has two mode, operation mode and cycle mode.

First, the game begin with operation mode, and when player push 'B' key,
then cycle mode beginning.

In operation mode, player can move cursor by pushing 'W', 'A', 'S' or 'D', 
and make any cells come alive by pushing SPACE.
(All cell is dead at the beginning point)

In cycle mode, cells are increasing or decreasing according to rule of Game of Life.

In both operation, player can quit game immediately by pushing 'Q'.

## OPERATION MODE
![Screenshot from 2022-09-04 22-48-55](https://user-images.githubusercontent.com/45516420/188317228-0737da7a-4462-4336-9e59-bf2c2050034d.png)

## CYCLE MODE
![Screenshot from 2022-09-04 22-50-33](https://user-images.githubusercontent.com/45516420/188317205-3ad5e6a0-2c2a-4860-8490-48b8d896a693.png)
