#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define SUBPOSITIONS_PER_CELL 3
#define SUBPOSITION_CENTER 0
#define NUM_CELL_TYPES 4

typedef enum {
  CELL_BLOCKED = 0,
  CELL_EMPTY = 1,
  CELL_SMALL_FOOD = 2,
  CELL_BIG_FOOD = 3,
  CELL_BOARD_EDGE = 4
} cell_t;

typedef enum {
  DIRECTION_UP = 0,
  DIRECTION_RIGHT = 1,
  DIRECTION_DOWN = 2,
  DIRECTION_LEFT = 3,
  DIRECTION_NONE = 4
} direction_t;

void generate_board(cell_t board[BOARD_WIDTH][BOARD_HEIGHT]);
cell_t *get_cell_relative(cell_t board[BOARD_WIDTH][BOARD_HEIGHT], int x, int y, direction_t direction);
cell_t *get_cell(cell_t board[BOARD_WIDTH][BOARD_HEIGHT], int x, int y);
//cell_t *get_cell_at_pos(cell_t board[BOARD_WIDTH][BOARD_HEIGHT], int pos_x, int pos_y);
bool cell_open(cell_t * cell);
bool relative_cell_open(cell_t board[BOARD_WIDTH][BOARD_HEIGHT], int x, int y, direction_t direction);
bool direction_perpendicular(direction_t dir1, direction_t dir2);
direction_t opposite_direction(direction_t direction);

#endif