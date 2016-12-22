#ifndef _GAME_H
#define _GAME_H

#define SHIFT_CURSOR_RIGHT 0x14
#define SHIFT_CURSOR_LEFT 0x12

int playerPlace;
int easy;
int place;

// moves the cursor up, down, left, or right on the screen
int moveUp(int);
int moveDown(int);
int moveLeft(int);
int moveRight(int);

#endif // _GAME_H_

