#include "board.h"
#include "Windows.h"
#include "conio.h"

#define ROW_NONE -1


bool pos_possible(piece_t *piece, game_t *game);
bool integrate_piece(piece_t *piece, game_t *game);
bool move_piece(piece_t *piece, game_t *game, direction_t direction);
void player_move(game_t *game, piece_t *piece);
void _collapse_board(game_t *game, int first_row);
void collapse_board(game_t *game);
cell_t piece_cell(piece_t *piece, int x, int y);
int piece_width(piece_t *piece);
int piece_height(piece_t *piece);


// cell_t get_cell(game_t *game, int x, int y){
//     return &game->board[x][y];
// }

// special functions to handle rotations
cell_t piece_cell(piece_t *piece, int x, int y){
    switch (piece->rotation){
        case DIRECTION_UP:
            return piece->piece[x][y];
            break;
        case DIRECTION_DOWN:
            return piece->piece[piece->width-x-1][piece->height-y-1];
            break;
        case DIRECTION_RIGHT:
            return piece->piece[y][x];
            break;
        case DIRECTION_LEFT:
            return piece->piece[piece->height-y-1][piece->width-x-1];
            break;
    }
    return CELL_EMPTY;
}

int piece_width(piece_t *piece){
    switch (piece->rotation){
        case DIRECTION_UP:
        case DIRECTION_DOWN:
            return piece->width;
            break;
        case DIRECTION_RIGHT:
        case DIRECTION_LEFT:
            return piece->height;
            break;
    }
    return 0;
}

int piece_height(piece_t *piece){
    switch (piece->rotation){
        case DIRECTION_UP:
        case DIRECTION_DOWN:
            return piece->height;
            break;
        case DIRECTION_RIGHT:
        case DIRECTION_LEFT:
            return piece->width;
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
    for(int i = 0; i < piece->width; i++){
        for(int j = 0; j < piece->height; j++){
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
            if(game->board[board_x][board_y] != CELL_EMPTY && piece->piece[i][j] != CELL_EMPTY){
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

    for(int i = 0; i < piece->width; i++){
        for(int j = 0; j < piece->height; j++){
            int board_x = piece->pos_x + i;
            int board_y = piece->pos_y + j;

            if(piece->piece[i][j] != CELL_EMPTY){
                game->board[board_x][board_y] = piece->piece[i][j];
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

    piece->pos_x = 2;
    piece->pos_y = 15;
    piece->frames_since_shift = 0;
    piece->height = 4;
    piece->width = 1;
    piece->piece[0][0] = 1;
    piece->piece[0][1] = 1;
    piece->piece[0][2] = 1;
    piece->piece[0][3] = 1;
}

void clear_rows(game_t *game, int row_start, int row_end){
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

    for(int i = 0; i < (BOARD_HEIGHT - row_end); i++){
        int source_row = row_end + i;
        int dest_row = row_start + i;
        for(int j = 0; j < BOARD_WIDTH; j++){
           game->board[j][dest_row] = game->board[j][source_row]; 
        }
    }

    for(int i = 0; i < (row_end - row_start); i++){
        int row = (BOARD_HEIGHT - 1) - i;
        for(int j = 0; j < BOARD_WIDTH; j++){
           game->board[j][row] = CELL_EMPTY;
        }
    }
}

void collapse_board(game_t *game){
    _collapse_board(game, 0);
}

void _collapse_board(game_t *game, int first_row){
    int row_start = ROW_NONE;
    int row_end = ROW_NONE;

    for(int j = 0; j < BOARD_HEIGHT; j++){
        for(int i = 0; i < BOARD_WIDTH; i++){
            if(game->board[i][j] == CELL_EMPTY){
                break;
            }

            if(i == (BOARD_WIDTH - 1)){ // entire row is clear
                if(row_start == ROW_NONE){
                    row_start = j;
                } 
                else if(row_end == ROW_NONE){
                    row_end = j;
                    clear_rows(game, row_start, row_end);
                    _collapse_board(game, row_start);
                    return;
                }
            }
        }
    }
}