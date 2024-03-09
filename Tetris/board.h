#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define BOARD_WIDTH 5
#define BOARD_HEIGHT 20
#define BOARD_EXTRA_ROWS 4
#define BOARD_HEIGHT_ACTUAL (BOARD_HEIGHT + BOARD_EXTRA_ROWS)
#define PIECE_MAX_XY 4

typedef enum {
    CELL_EMPTY = 0,
    CELL_COLOR1 = 1,
    CELL_COLOR2 = 2,
    CELL_COLOR3 = 3,
    CELL_COLOR4 = 4
} cell_t;

typedef enum {
    DIRECTION_DOWN = 0,
    DIRECTION_LEFT = 1,
    DIRECTION_UP= 2,
    DIRECTION_RIGHT = 3
} direction_t;

typedef enum {
    PIECE_LANDED = 0,
    PIECE_FALLING = 1
} result_t;

typedef enum {
    PIECE_I = 0,
    PIECE_J = 1,
    PIECE_T = 2,
    PIECE_L = 3,
    PIECE_S = 4,
    PIECE_Z = 5,
    PIECE_NONE = 6
} piece_type_t;

typedef struct {
    int score;
    int level;
    int shift_period;
    cell_t board[BOARD_WIDTH][BOARD_HEIGHT_ACTUAL];
} game_t;

typedef struct {
    int pos_x;
    int pos_y;
    int width;
    int height;
    int frames_since_shift;
    direction_t rotation;
    piece_type_t type;
    cell_t piece[PIECE_MAX_XY][PIECE_MAX_XY];
} piece_t;


result_t board_update(piece_t *active_piece, game_t *game);
void new_piece(piece_t *piece);
void init_board(game_t *game);

#endif