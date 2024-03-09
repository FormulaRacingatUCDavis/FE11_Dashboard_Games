#include "being.h"

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

//ghost movement parameters

void init_being(being_t *being, being_type type, int cell_x, int cell_y,
                direction_t direction, int speed) {
  being->cell_x = cell_x;
  being->cell_y = cell_y;
  being->sub_pos_x = SUBPOSITION_CENTER;
  being->sub_pos_y = SUBPOSITION_CENTER;
  being->direction = direction;
  being->next_direction = direction;
  being->type = type;
}

// void move_being_old(being_t *being, cell_t board[BOARD_WIDTH][BOARD_HEIGHT]) {

//   direction_t direction = being->next_direction;  // first attempt to move in new direction

//   for(int i = 0; i < 2; i++){
//     int new_pos_x = being->cell_x;
//     int new_pos_y = being->cell_y;
    
//     switch (direction) {
//       case DIRECTION_UP:
//         new_pos_y -= being->speed;
//         break;
//       case DIRECTION_DOWN:
//         new_pos_y += being->speed;
//         break;
//       case DIRECTION_LEFT:
//         new_pos_x -= being->speed;
//         break;
//       case DIRECTION_RIGHT:
//         new_pos_x += being->speed;
//         break;
//       default:
//         // do nothing
//         break;
//     }

//     cell_t *new_cell = get_cell_at_pos(board, new_pos_x, new_pos_y);

//     if (*new_cell == CELL_BLOCKED || *new_cell == CELL_BOARD_EDGE) {  
//       direction = being->direction;  // if new direction is blocked, try to move in old direction
//     } 
//     else { // new square is open
//       being->direction = direction; 
//       being->pos_x = new_pos_x;
//       being->pos_y = new_pos_y;

//       if (being->type == BEING_PACMAN) {
//         if (*new_cell == CELL_SMALL_FOOD) {
//           *new_cell = CELL_EMPTY;
//           being->score += SMALL_FOOD_SCORE;
//         }

//         if (*new_cell == CELL_BIG_FOOD) {
//           *new_cell = CELL_EMPTY;
//           being->score += BIG_FOOD_SCORE;
//         }
//       }

//       return;
//     }
//   }
// }

bool move_possible(being_t * being, cell_t board[BOARD_WIDTH][BOARD_HEIGHT], direction_t direction){
  switch(direction){
    case DIRECTION_UP:
      // reject if not centered on X axis
      if(being->sub_pos_x != SUBPOSITION_CENTER){
        return false;
      }

      // reject if too close to a border
      if(being->sub_pos_y <= SUBPOSITION_CENTER){
        return relative_cell_open(board, being->cell_x, being->cell_y, direction);
      }

      return true;

    case DIRECTION_DOWN:
      if(being->sub_pos_x != SUBPOSITION_CENTER){
        return false;
      }

      if(being->sub_pos_y >= SUBPOSITION_CENTER){
        return relative_cell_open(board, being->cell_x, being->cell_y, direction);
      }

      return true;

    case DIRECTION_LEFT:
      if(being->sub_pos_y != SUBPOSITION_CENTER){
        return false;
      }

      if(being->sub_pos_x <= SUBPOSITION_CENTER){
        return relative_cell_open(board, being->cell_x, being->cell_y, direction);
      }

      return true;

    case DIRECTION_RIGHT:
      if(being->sub_pos_y != SUBPOSITION_CENTER){
        return false;
      }

      if(being->sub_pos_x >= SUBPOSITION_CENTER){
        return relative_cell_open(board, being->cell_x, being->cell_y, direction);
      }

      return true;

    default:
      // do nothing

      break;
  }

  return false;
}

bool move_being(being_t * being, cell_t board[BOARD_WIDTH][BOARD_HEIGHT], direction_t direction){
  if(!move_possible(being, board, direction)){
    return false;
  }
  
  // increment or decrement subposition
  switch (direction) {
    case DIRECTION_UP:
      being->sub_pos_y--;
      being->sub_pos_x = SUBPOSITION_CENTER;
      break;
    case DIRECTION_DOWN:
      being->sub_pos_y++;
      being->sub_pos_x = SUBPOSITION_CENTER;
      break;
    case DIRECTION_LEFT:
      being->sub_pos_x--;
      being->sub_pos_y = SUBPOSITION_CENTER;
      break;
    case DIRECTION_RIGHT:
      being->sub_pos_x++;
      being->sub_pos_y = SUBPOSITION_CENTER;
      break;
    default:
      // do nothing
      break;
  }

  // handle subposition over/underflows
  if(being->sub_pos_x > SUBPOSITIONS_PER_CELL){
    being->cell_x = being->cell_x + 1;
    being->sub_pos_x = -SUBPOSITIONS_PER_CELL;
  }
  else if(being->sub_pos_x < -SUBPOSITIONS_PER_CELL){
    being->cell_x--;
    being->sub_pos_x = SUBPOSITIONS_PER_CELL;
  }

  if(being->sub_pos_y > SUBPOSITIONS_PER_CELL){
    being->cell_y++;
    being->sub_pos_y = -SUBPOSITIONS_PER_CELL;
  }
  else if(being->sub_pos_y < -SUBPOSITIONS_PER_CELL){
    being->cell_y--;
    being->sub_pos_y = SUBPOSITIONS_PER_CELL;
  }

  return true;
}

void move_pacman(being_t *pacman, cell_t board[BOARD_WIDTH][BOARD_HEIGHT]){
  check_inputs(pacman);

  // attemp to move in new direction
  if(move_being(pacman, board, pacman->next_direction)){
    pacman->direction = pacman->next_direction;
  } else {
    move_being(pacman, board, pacman->direction);
  }

  cell_t *new_cell = get_cell(board, pacman->cell_x, pacman->cell_y);
  if (*new_cell == CELL_SMALL_FOOD) {
    *new_cell = CELL_EMPTY;
  }
  else if (*new_cell == CELL_BIG_FOOD) {
    *new_cell = CELL_EMPTY;
  }
}

void move_ghost(being_t *ghost, cell_t board[BOARD_WIDTH][BOARD_HEIGHT]){

  direction_t opposite_dir = opposite_direction(ghost->direction);

  // determine possible directions
  int dir_count = 0;
  direction_t directions[4];

  for(int i = 0; i < 4; i++){
    direction_t direction = (direction_t)i;

    if(direction == opposite_dir){
      continue; // don't include opposite direction in list
    }

    if(move_possible(ghost, board, direction)){
      directions[dir_count] = direction;
      dir_count++;
    }
  }

  // only go backwards if there are no other options
  if(dir_count == 0){
    move_being(ghost, board, opposite_dir);
    ghost->direction = opposite_dir;
    return;
  }

  // chose randomly from possible directions
  int dir_index = rand() % dir_count;
  ghost->direction = directions[dir_index];
  move_being(ghost, board, ghost->direction);
}

bool beings_coliding(being_t *being1, being_t *being2){
  return (being1->cell_x == being2->cell_x && being1->cell_y == being2->cell_y);
}

// bool being_centered(being_t *being){
//   return (being->sub_pos_x == SUBPOSITION_CENTER && being->sub_pos_y == SUBPOSITION_CENTER);
// }

bool check_inputs(being_t *pacman)
{
  if(!_kbhit()) return false; 

  char c = _getch();
  switch (c){
    case 'w':
      pacman->next_direction = DIRECTION_UP;
      return true;
      break;
    case 's':
      pacman->next_direction = DIRECTION_DOWN;
      return true;
      break;
    case 'a':
      pacman->next_direction = DIRECTION_LEFT;
      return true;
      break;
    case 'd':
      pacman->next_direction = DIRECTION_RIGHT;
      return true;
      break;
  }
  return false;
}