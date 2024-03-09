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
        // result = move_piece(active_piece, board);

        // if(result == PIECE_LANDED){
        //     integrate_piece(active_piece, board);
        //     if(topped_out(board)){
        //         return 0; // end game
        //     }
        //     score += clear_rows(board);
        //     active_piece = new_piece();
        // }
    }
}

void draw_board(game_t *game, piece_t *active_piece){
    printf("\033c");
    for(int j = (BOARD_HEIGHT-1); j >= 0; j--){
        for(int i = 0; i < BOARD_WIDTH; i++){
            char c = ' ';

            if(game->board[i][j] != CELL_EMPTY){
                c = '#';
            } else if(i >= active_piece->pos_x && i < active_piece->pos_x + active_piece->width 
                   && j >= active_piece->pos_y && j < active_piece->pos_y + active_piece->height) {
                if(active_piece->piece[i-active_piece->pos_x][j-active_piece->pos_y]){
                    c = '#';
                }
            }
            printf("%c", c);
        }
        printf("\n");
    }
}
