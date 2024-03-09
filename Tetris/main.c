#include "board.h"

#include <stdio.h>
#include <Windows.h>

void draw_board(game_t *game, piece_t *active_piece);

int main(void){
    cell_t board[BOARD_WIDTH][BOARD_HEIGHT + BOARD_EXTRA_ROWS];
    game_t game;
    game.shift_period = 5;
    piece_t active_piece;
    new_piece(&active_piece);
    int score = 0;

    init_board(&game);

    while(1){
        board_update(&active_piece, &game);
        draw_board(&game, &active_piece);
        Sleep(100);
    }
}

void draw_board(game_t *game, piece_t *active_piece){
    printf("\033c");

    for(int j = (BOARD_HEIGHT-1); j >= 0; j--){
        for(int i = 0; i < BOARD_WIDTH; i++){
            char c = ' ';

            if(game->board[i][j] != CELL_EMPTY){
                c = '#';
            } else if(i >= active_piece->pos_x && i < active_piece->pos_x + piece_width(active_piece)
                   && j >= active_piece->pos_y && j < active_piece->pos_y + piece_height(active_piece)) {
                if(piece_cell(active_piece, i-active_piece->pos_x, j-active_piece->pos_y)){
                    c = '#';
                }
            }
            printf("%c", c);
        }
        printf("\n");
    }
}
