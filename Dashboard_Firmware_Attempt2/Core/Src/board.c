#include "board.h"
//#include "Windows.h"
//#include "conio.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "stm32f7xx_hal.h"

#define ROW_NONE -1

typedef struct{
	bool left;
	bool right;
	bool rotate_left;
	bool rotate_right;
} USER_INPUT_t;

USER_INPUT_t user_input;

bool pos_possible(piece_t *piece, game_t *game);
bool integrate_piece(piece_t *piece, game_t *game);
bool move_piece(piece_t *piece, game_t *game, direction_t direction);
void player_move(game_t *game, piece_t *piece);
void collapse_board(game_t *game);
bool row_full(game_t *game, int row);
bool row_empty(game_t *game, int row);
void clear_input();
void HAL_GPIO_EXTI_Callback(uint16_t pin);


HAL_GPIO_EXTI_Callback(uint16_t pin){
	switch(pin){
		case GPIO_PIN_9:
			user_input.rotate_left = true;
			break;
		case GPIO_PIN_10:
			user_input.rotate_right = true;
			break;
		case GPIO_PIN_11:
			user_input.left = true;
			break;
		case GPIO_PIN_12:
			user_input.right = true;
			break;
	}
}

void clear_input(){
	user_input.left = false;
	user_input.right = false;
	user_input.rotate_left = false;
	user_input.rotate_left = false;
}


cell_t piece_cell(piece_t *piece, int x, int y){
    if(x < 0 || y < 0 || x >= piece->proto->size || y >= piece->proto->size){
        CELL_EMPTY;
    }

    return piece->proto->cell[piece->rotation][x][y];
}

cell_t board_cell(game_t *game, int x, int y){
    if(x < 0 || y < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT_ACTUAL){
        return CELL_OUT_OF_BOUNDS;
    }

    return game->board[x][y];
}

void init_board(game_t *game){
    for(int i = 0; i < BOARD_WIDTH; i++){
        for(int j = 0; j < BOARD_HEIGHT_ACTUAL; j++){
            game->board[i][j] = CELL_COLOR1;
        }
    }
}

bool pos_possible(piece_t *piece, game_t *game){
    for(int i = 0; i < piece->proto->size; i++){
        for(int j = 0; j < piece->proto->size; j++){
            int board_x = piece->pos_x + i;
            int board_y = piece->pos_y + j;

            // check for cell collisions
            if(board_cell(game, board_x, board_y) != CELL_EMPTY && piece_cell(piece, i, j) != CELL_EMPTY){
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

    for(int i = 0; i < piece->proto->size; i++){
        for(int j = 0; j < piece->proto->size; j++){
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
        piece->rotation = piece->proto->num_rotations - 1;
    } else if (piece->rotation >= piece->proto->num_rotations){
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
    if(user_input.left){
		move_piece(piece, game, DIRECTION_LEFT);
    } else if(user_input.right){
		move_piece(piece, game, DIRECTION_RIGHT);
    }

    if(user_input.rotate_left){
    	rotate_piece(piece, game, false);
    } else if(user_input.rotate_right){
		rotate_piece(piece, game, true);
    }

    clear_input();
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

    piece_type_t piece_type = rand() % NUM_PIECES;

    // reroll if same piece as last time
    if(piece_type == last_piece_type){
        piece_type = rand() % NUM_PIECES;
    }

    last_piece_type = piece_type;
    piece->proto = get_piece_proto(piece_type);

    piece->pos_x = piece->proto->spawn_pos_x;
    piece->pos_y = piece->proto->spawn_pos_y;
    piece->frames_since_shift = 0;
    piece->rotation = DIRECTION_DOWN;
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

    while (row < BOARD_HEIGHT) {

        if(row_full(game, row)){

            if(row_full_start == ROW_NONE){
                row_full_start = row;  // record the start of full rows
            }

        } else { // row is empty
            // check if rows before were full, if so then clear out those rows and set back the row counter
            if(row_full_start != ROW_NONE){ 
                clear_rows(game, row_full_start, row);
                
                row = row_full_start; 
                row_full_start = ROW_NONE;
            }

        }

        row++;
    }
}

bool row_full(game_t *game, int row){
    for(int i = 0; i < BOARD_WIDTH; i++){
        // if any cell is empty, row is not full
        if(game->board[i][row] == CELL_EMPTY){
            return false;
        }
    }
    return true;
}

bool row_empty(game_t *game, int row){
    for(int i = 0; i < BOARD_WIDTH; i++){
        // if any cell is empty, row is not full
        if(game->board[i][row] != CELL_EMPTY){
            return false;
        }
    }
    return true;
}

bool topped_out(game_t *game){
    // should only need to check the first row above board, but check all just in case
    for(int row = BOARD_HEIGHT; row < BOARD_HEIGHT_ACTUAL; row++){
        if(!row_empty(game, row)){
            return true;
        }
    }

    return false;
}
