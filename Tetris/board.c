#include "board.h"
#include "Windows.h"
#include "conio.h"
#include "stdio.h"

#define ROW_NONE -1

bool pos_possible(piece_t *piece, game_t *game);
bool integrate_piece(piece_t *piece, game_t *game);
bool move_piece(piece_t *piece, game_t *game, direction_t direction);
void player_move(game_t *game, piece_t *piece);
void _collapse_board(game_t *game, int first_row);
void collapse_board(game_t *game);


// cell_t get_cell(game_t *game, int x, int y){
//     return &game->board[x][y];
// }

// special functions to handle rotations
cell_t piece_cell(piece_t *piece, int x, int y){
    int x_raw = x;
    int y_raw = y;
    switch (piece->rotation){
        case DIRECTION_UP:
            // default
            break;
        case DIRECTION_DOWN:
            x_raw = (piece->width_raw - 1) - x;
            y_raw = (piece->height_raw - 1) - y;
            break;
        case DIRECTION_RIGHT:
            x_raw = y;
            y_raw = x;
            break;
        case DIRECTION_LEFT:
            x_raw = (piece->width_raw - 1) - y;
            y_raw = (piece->height_raw - 1) - x;       
            break;
    }
    //printf("XY: %i %i\n", x, y);
    //printf("raw XY: %i %i\n", x_raw, y_raw);
    return piece->cell[x_raw][y_raw];
}

int piece_width(piece_t *piece){
    switch (piece->rotation){
        case DIRECTION_UP:
        case DIRECTION_DOWN:
            return piece->width_raw;
            break;
        case DIRECTION_RIGHT:
        case DIRECTION_LEFT:
            return piece->height_raw;
            break;
    }
    return 0;
}

int piece_height(piece_t *piece){
    switch (piece->rotation){
        case DIRECTION_UP:
        case DIRECTION_DOWN:
            return piece->height_raw;
            break;
        case DIRECTION_RIGHT:
        case DIRECTION_LEFT:
            return piece->width_raw;
            break;
    }
    return 0;
}

void init_board(game_t *game){
    for(int i = 0; i < BOARD_WIDTH; i++){
        for(int j = 0; j < BOARD_HEIGHT_ACTUAL; j++){
            game->board[i][j] = CELL_EMPTY;
        }
    }
}

bool pos_possible(piece_t *piece, game_t *game){
    for(int i = 0; i < piece_width(piece); i++){
        for(int j = 0; j < piece_height(piece); j++){
            int board_x = piece->pos_x + i;
            int board_y = piece->pos_y + j;

            // check board boundaries
            if(board_y < 0 || board_y >= BOARD_HEIGHT_ACTUAL) {
                return false;
            }
            if(board_x < 0 || board_x >= BOARD_WIDTH) {
                return false;
            }

            // check for cell collisions
            if(game->board[board_x][board_y] != CELL_EMPTY && piece_cell(piece, i, j) != CELL_EMPTY){
                return false;
            }
        }
    }

    // position is possible
    return true;
}

// bool move_possible(piece_t *piece, game_t *game, direction_t direction){
//     switch(direction){
//         case DIRECTION_DOWN:
//             if()
//     }
// }

bool integrate_piece(piece_t *piece, game_t *game){
    // confirm that piece position is valid
    if(!pos_possible(piece, game)){
        return false;
    }

    for(int i = 0; i < piece_width(piece); i++){
        for(int j = 0; j < piece_height(piece); j++){
            int board_x = piece->pos_x + i;
            int board_y = piece->pos_y + j;

            if(piece_cell(piece, i, j) != CELL_EMPTY){
                game->board[board_x][board_y] = piece_cell(piece, i, j);
            }
        }
    }

    return true;
}

bool move_piece(piece_t *piece, game_t *game, direction_t direction){
    int original_x = piece->pos_x;
    int original_y = piece->pos_y;

    switch(direction){
        case DIRECTION_DOWN:
            piece->pos_y--;
            break;
        case DIRECTION_LEFT:
            piece->pos_x--;
            break;
        case DIRECTION_RIGHT:
            piece->pos_x++;
            break;
    }

    // check if move is possible
    if(pos_possible(piece, game)){
        return true;
    }

    // move is blocked, cancel move
    piece->pos_x = original_x;
    piece->pos_y = original_y;

    return false;
}

bool rotate_piece(piece_t *piece, game_t *game, bool clockwise){
    int original_rotation = piece->rotation;

    // update piece rotation
    if(clockwise){
        piece->rotation++;
    } else {
        piece->rotation--;
    }

    // handle overflow
    if(piece->rotation < 0) {
        piece->rotation = 3;
    } else if (piece->rotation > 3){
        piece->rotation = 0;
    }

    // check if move is possible
    if(pos_possible(piece, game)){
        return true;
    }

    // move is blocked, cancel move
    piece->rotation = original_rotation;

    return false;
}


void player_move(game_t *game, piece_t *piece){
    if(!_kbhit()){
        return;
    }

    switch(_getch()){
        case 'a':
            move_piece(piece, game, DIRECTION_LEFT);
            break;
        case 'd':
            move_piece(piece, game, DIRECTION_RIGHT);
            break;
        case 's':
            move_piece(piece, game, DIRECTION_DOWN);
            break;
        case 'w':
            rotate_piece(piece, game, true);
            break;
        default:
            // do nothing
    }   
}

result_t board_update(piece_t *active_piece, game_t *game){
    active_piece->frames_since_shift++;

    player_move(game, active_piece);

    if(active_piece->frames_since_shift >= game->shift_period){
        if(move_piece(active_piece, game, DIRECTION_DOWN)){
            active_piece->frames_since_shift = 0;
            return PIECE_FALLING;
        } else {
            integrate_piece(active_piece, game);
            collapse_board(game);
            new_piece(active_piece);
            return PIECE_LANDED;
        }
        
    }
}

void new_piece(piece_t *piece){
    static piece_type_t last_piece_type = PIECE_NONE;

    piece->pos_x = 5;
    piece->pos_y = 15;
    piece->frames_since_shift = 0;
    piece->height_raw = 4;
    piece->width_raw = 1;
    piece->cell[0][0] = 1;
    piece->cell[0][1] = 1;
    piece->cell[0][2] = 1;
    piece->cell[0][3] = 1;
    piece->rotation = DIRECTION_LEFT;
}

void clear_rows(game_t *game, int row_start, int row_end){
    //printf("Start: %i, End: %i\n", row_start, row_end);

    if(row_start < 0 || row_start >= BOARD_HEIGHT){
        return;
    }
    if(row_end < 0 || row_end >= BOARD_HEIGHT){
        return;
    }
    if(row_start >= row_end){
        return;
    }

    // TODO: animation to show row clears

    // move down rows above clear
    for(int i = 0; i < (BOARD_HEIGHT - row_end); i++){
        int source_row = row_end + i;
        int dest_row = row_start + i;
        for(int j = 0; j < BOARD_WIDTH; j++){
           game->board[j][dest_row] = game->board[j][source_row]; 
        }
    }

    // clear out top rows
    for(int i = 0; i < (row_end - row_start); i++){
        int row = (BOARD_HEIGHT - 1) - i;
        for(int j = 0; j < BOARD_WIDTH; j++){
           game->board[j][row] = CELL_EMPTY;
        }
    }
}

void collapse_board(game_t *game){
    int row_full_start = ROW_NONE;
    int row = 0;

    while(row < BOARD_HEIGHT){
        for(int i = 0; i < BOARD_WIDTH; i++){

            // if any cell is empty, row is not complete
            if(game->board[i][row] == CELL_EMPTY){
                //rintf("Cell empty: %i %i\n", i, row);
                // if previous rows were full but this row is empty, call clear rows and set current row back
                if(row_full_start != ROW_NONE){
                    //printf("Clearing rows %i to %i\n", row_full_start, row);
                    clear_rows(game, row_full_start, row);

                    row = row_full_start;
                    row_full_start = ROW_NONE;
                }

                break;
            }

            // check if we've reached end of row without finding empty cell
            // and record row number if this is the first full row
            if((row_full_start == ROW_NONE) && (i + 1 == BOARD_WIDTH)) {
                row_full_start = row;
            }
        }

        row++;
    }
}