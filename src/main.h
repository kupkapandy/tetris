#pragma once
#include <stdint.h>


struct tetromino{
  uint32_t x;
  uint32_t y;
};

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 1000;
const int columns = 10;
const int rows = 22;
const int horizontalSpacing = SCREEN_WIDTH / columns;
const int verticalSpacing = SCREEN_HEIGHT / (rows - 2);

void drawGrid(uint32_t grid[rows][columns], struct tetromino curr[]);
void clearLine(uint32_t grid[rows][columns], uint32_t indexY);
bool move(uint32_t grid[rows][columns], struct tetromino curr[], uint32_t direction);
void newTetromino(struct tetromino curr[]);
void copyToGrid(uint32_t grid[rows][columns], struct tetromino curr[]);

const struct tetromino I[4] = {{5, 0},{6, 0},{7, 0},{8, 0}};
const struct tetromino O[4] = {{5, 0},{6, 0},{5, 1},{6, 1}};
const struct tetromino T[4] = {{4, 0},{5, 0},{6, 0},{5, 1}};
const struct tetromino J[4] = {{5, 0},{5, 1},{4, 2},{5, 2}};
const struct tetromino L[4] = {{5, 0},{5, 1},{5, 2},{6, 2}};
const struct tetromino S[4] = {{4, 0},{5, 1},{5, 0},{6, 0}};
const struct tetromino Z[4] = {{4, 0},{5, 0},{5, 1},{6, 1}};

