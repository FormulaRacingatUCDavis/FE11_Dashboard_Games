#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include "pieces.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BOARD_EXTRA_ROWS 4
#define BOARD_HEIGHT_ACTUAL (BOARD_HEIGHT + BOARD_EXTRA_ROWS)


typedef enum {
    PIECE_LANDED = 0,
    PIECE_FALLING = 1
} result_t;

typedef struct {
    int score;
    int level;
    int shift_period;
    cell_t board[BOARD_WIDTH][BOARD_HEIGHT_ACTUAL];
} game_t;

result_t board_update(piece_t *active_piece, game_t *game);
void new_piece(piece_t *piece);
void init_board(game_t *game);
cell_t piece_cell(piece_t *piece, int x, int y);
cell_t board_cell(game_t *game, int x, int y);
bool topped_out(game_t *game);

#endif