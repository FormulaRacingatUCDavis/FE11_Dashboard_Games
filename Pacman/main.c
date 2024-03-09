#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "being.h"
#include "board.h"

#define NUM_GHOSTS 10

cell_t board[BOARD_WIDTH][BOARD_HEIGHT] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                           {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
                                           {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0},
                                           {0, 2, 2, 2, 2, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 2, 2, 0},
                                           {0, 2, 0, 0, 3, 0, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 0, 2, 0, 0},
                                           {0, 2, 0, 2, 2, 0, 2, 0, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2, 2, 0},
                                           {0, 2, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 0},
                                           {0, 2, 0, 2, 0, 2, 2, 2, 0, 2, 0, 0, 0, 2, 0, 2, 2, 2, 2, 0},
                                           {0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 0},
                                           {0, 2, 2, 2, 2, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0},
                                           {0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
                                           {0, 2, 2, 2, 2, 2, 2, 2, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0},
                                           {0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 0},
                                           {0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 0, 0, 0},
                                           {0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 0, 0, 2, 2, 2, 0},
                                           {0, 2, 2, 2, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 2, 2, 2, 0, 2, 0},
                                           {0, 0, 0, 0, 2, 0, 2, 0, 2, 2, 2, 2, 2, 2, 0, 0, 2, 0, 2, 0},
                                           {0, 2, 2, 2, 2, 2, 2, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0},
                                           {0, 2, 0, 2, 0, 2, 0, 0, 2, 2, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
                                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void draw_board(cell_t board[BOARD_WIDTH][BOARD_HEIGHT], being_t *pacman, being_t *ghosts);
//int check_inputs(being_t *pacman);

int main(void) {
  //cell_t board[BOARD_WIDTH][BOARD_HEIGHT];
  being_t pacman;
  being_t ghosts[NUM_GHOSTS];
  

  //generate_board(board);
  init_being(&pacman, BEING_PACMAN, 1, 1, DIRECTION_RIGHT, 10);
  for(int i = 0; i < NUM_GHOSTS; i++){
    init_being(&ghosts[i], BEING_ENEMY, 6, 1, DIRECTION_RIGHT, 10);
  }

  while (1) {
    move_pacman(&pacman, board);
    for(int i = 0; i < NUM_GHOSTS; i++){
      move_ghost(&ghosts[i], board);
      if(beings_coliding(&pacman, &ghosts[i])){
        printf("\033c");
        printf("you done fucked up brother\n");
        return 0;
      }
    }
    draw_board(board, &pacman, ghosts);
    
    Sleep(50);
  }
  // printf("Hello World\n");
  return 0;
}

void draw_board(cell_t board[BOARD_WIDTH][BOARD_HEIGHT], being_t *pacman, being_t ghosts[NUM_GHOSTS]) {
  printf("\033c");
  for (int j = 0; j < BOARD_WIDTH; j++) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
      char c = ' ';
      switch (board[i][j]) {
        case CELL_BLOCKED:
        case CELL_BOARD_EDGE:
          c = '#';
          break;
        case CELL_BIG_FOOD:
          c = 'x';
          break;
        case CELL_SMALL_FOOD:
          c = '.';
          break;
        case CELL_EMPTY:
          c = ' ';
          break;
        default:
          c = '?';
          break;
      }

      for(int k = 0; k < NUM_GHOSTS; k++){
        if (i == ghosts[k].cell_x && j == ghosts[k].cell_y) {
          c = 'G';
        }
      }

      if (i == pacman->cell_x && j == pacman->cell_y) {
        c = 'P';
      }
      printf("%c ", c);
    }
    printf("\n");
  }
}

// int check_inputs(being_t *pacman)
// {
//   if(!_kbhit()) return 0; 

//   char c = _getch();
//   switch (c){
//     case 'q':
//       return 1;
//       break;
//     case 'w':
//       pacman->next_direction = DIRECTION_UP;
//       break;
//     case 's':
//       pacman->next_direction = DIRECTION_DOWN;
//       break;
//     case 'a':
//       pacman->next_direction = DIRECTION_LEFT;
//       break;
//     case 'd':
//       pacman->next_direction = DIRECTION_RIGHT;
//       break;
//   }
//   return 0;
// }
