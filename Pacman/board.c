#include "board.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_CELL_GEN_OPTIONS 2
#define CELL_GEN_OPTIONS {CELL_SMALL_FOOD, CELL_BLOCKED}

void generate_board(cell_t board[BOARD_HEIGHT][BOARD_WIDTH]) {
  cell_t cell_options[NUM_CELL_GEN_OPTIONS] = CELL_GEN_OPTIONS;

  for (int i = 0; i < BOARD_WIDTH; i++) {
    for (int j = 0; j < BOARD_HEIGHT; j++) {
      board[j][i] = cell_options[rand() % NUM_CELL_GEN_OPTIONS];
    }
  }
}

cell_t *get_cell_relative(cell_t board[BOARD_HEIGHT][BOARD_WIDTH], int x, int y, direction_t direction) {
  switch (direction) {
  case DIRECTION_UP:
    return get_cell(board, x, y - 1);
    break;
  case DIRECTION_DOWN:
    return get_cell(board, x, y + 1);
    break;
  case DIRECTION_LEFT:
    return get_cell(board, x - 1, y);
    break;
  case DIRECTION_RIGHT:
    return get_cell(board, x + 1, y);
    break;
  default:
    return get_cell(board, x, y);
  }
}

cell_t *get_cell(cell_t board[BOARD_HEIGHT][BOARD_WIDTH], int x, int y) {
  static cell_t board_edge = CELL_BOARD_EDGE;
  if (y < 0 || y >= BOARD_HEIGHT || x < 0 || x >= BOARD_WIDTH) {
    return &board_edge;
  }
  return &board[y][x];
}

//cell_t *get_cell_at_pos(cell_t board[BOARD_WIDTH][BOARD_HEIGHT], int pos_x, int pos_y) {
//  return get_cell(board, pos_x / POSITIONS_PER_CELL,
//                  pos_y / POSITIONS_PER_CELL);
//}

bool cell_open(cell_t * cell) {
  if (*cell == CELL_BLOCKED || *cell == CELL_BOARD_EDGE){
    return false;
  }
       
  return true;
}

bool relative_cell_open(cell_t board[BOARD_HEIGHT][BOARD_WIDTH], int x, int y, direction_t direction) {
  cell_t *cell = get_cell_relative(board, x, y, direction);
  return cell_open(cell);
}

bool direction_perpendicular(direction_t dir1, direction_t dir2){
  if(dir1 == DIRECTION_UP || dir1 == DIRECTION_DOWN){
    return (dir2 == DIRECTION_RIGHT || dir2 == DIRECTION_LEFT);
  }

  return (dir2 == DIRECTION_UP || dir2 == DIRECTION_DOWN);
}

direction_t opposite_direction(direction_t direction){
  switch(direction){
    case DIRECTION_DOWN:
      return DIRECTION_UP;
    case DIRECTION_UP:
      return DIRECTION_DOWN;
    case DIRECTION_LEFT:
      return DIRECTION_RIGHT;
    case DIRECTION_RIGHT:
      return DIRECTION_LEFT;
  }

  return DIRECTION_NONE;
}