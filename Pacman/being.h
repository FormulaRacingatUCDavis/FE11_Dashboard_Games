#ifndef BEING_H
#define BEING_H

#include "board.h"
#include <stdbool.h>

#define SMALL_FOOD_SCORE 1
#define BIG_FOOD_SCORE 5

typedef enum { BEING_PACMAN = 0, BEING_ENEMY = 1 } being_type;

typedef struct {
  direction_t direction;
  direction_t next_direction;
  int cell_x;
  int cell_y;
  int sub_pos_x;
  int sub_pos_y;
  being_type type;
} being_t;

void init_being(being_t *being, being_type type, int cell_x, int cell_y, direction_t direction, int speed);
bool move_being(being_t * being, cell_t board[BOARD_WIDTH][BOARD_HEIGHT], direction_t direction);
bool move_possible(being_t * being, cell_t board[BOARD_WIDTH][BOARD_HEIGHT], direction_t direction);
bool check_inputs(being_t *pacman);
void move_pacman(being_t *pacman, cell_t board[BOARD_WIDTH][BOARD_HEIGHT]);
void move_ghost(being_t *ghost, cell_t board[BOARD_WIDTH][BOARD_HEIGHT]);
bool beings_coliding(being_t *being1, being_t *being2);

#endif