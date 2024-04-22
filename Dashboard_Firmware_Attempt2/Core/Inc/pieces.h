#ifndef PIECES_H
#define PIECES_H

#define PIECE_MAX_XY 4
#define DIRECTIONS 4
#define NUM_PIECES 7

#define NULL_PIECE {{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}}

typedef enum {
    CELL_EMPTY = 0,
    CELL_COLOR1 = 1,
    CELL_COLOR2 = 2,
    CELL_COLOR3 = 3,
    CELL_COLOR4 = 4,
    CELL_OUT_OF_BOUNDS
} cell_t;

typedef enum {
    DIRECTION_DOWN = 0,
    DIRECTION_LEFT = 1,
    DIRECTION_UP = 2,
    DIRECTION_RIGHT = 3
} direction_t;

typedef enum {
    PIECE_I = 0,
    PIECE_J = 1,
    PIECE_T = 2,
    PIECE_L = 3,
    PIECE_S = 4,
    PIECE_Z = 5,
    PIECE_O = 6,
    PIECE_NONE = 7,
} piece_type_t;

typedef struct {
    int spawn_pos_x;
    int spawn_pos_y;
    //int width_raw;
    //int height_raw;
    int size;
    int num_rotations;
    piece_type_t type;
    cell_t cell[DIRECTIONS][PIECE_MAX_XY][PIECE_MAX_XY];
} piece_proto_t;

typedef struct {
    int pos_x;
    int pos_y;
    int frames_since_shift;
    direction_t rotation;
    int num_rotations;
    piece_proto_t *proto;
} piece_t;


piece_proto_t *get_piece_proto(piece_type_t type);

#endif