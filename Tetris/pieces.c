#include "pieces.h"

piece_proto_t *get_piece_proto(piece_type_t type) {
    static piece_proto_t t_piece = {
        .spawn_pos_x = 5,
        .spawn_pos_y = 17,
        .size = 3,
        .type = PIECE_T,
        .num_rotations = 4,
        .cell={{{0, 0, 0, 0},
                {1, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}},
                
                {{0, 1, 0, 0},
                {1, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}},
                
                {{0, 1, 0, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}},
                
               {{0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}}}
    };

    static piece_proto_t s_piece = {
        .spawn_pos_x = 5,
        .spawn_pos_y = 17,
        .size = 3,
        .type = PIECE_S,
        .num_rotations = 2,
        .cell={{{0, 0, 0, 0},
                {0, 1, 1, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0}},
                
                {{0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 0}},
                
            NULL_PIECE,
            NULL_PIECE}
    };

    static piece_proto_t z_piece = {
        .spawn_pos_x = 5,
        .spawn_pos_y = 17,
        .size = 3,
        .type = PIECE_Z,
        .num_rotations = 2,
        .cell={{{0, 0, 0, 0},
                {1, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0}},
                
                {{0, 0, 1, 0},
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}},
                
            NULL_PIECE,
            NULL_PIECE}
    };

    static piece_proto_t l_piece = {
        .spawn_pos_x = 5,
        .spawn_pos_y = 17,
        .size = 3,
        .type = PIECE_L,
        .num_rotations = 4,
        .cell={{{0, 0, 0, 0},
                {1, 1, 1, 0},
                {1, 0, 0, 0},
                {0, 0, 0, 0}},
                
                {{1, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}},
                
                {{0, 0, 1, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}},
                
                {{0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0}}}
    };

    static piece_proto_t j_piece = {
        .spawn_pos_x = 5,
        .spawn_pos_y = 17,
        .size = 3,
        .type = PIECE_J,
        .num_rotations = 4,
        .cell={{{0, 0, 0, 0},
                {1, 1, 1, 0},
                {1, 0, 0, 0},
                {0, 0, 0, 0}},
                
                {{1, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}},
                
                {{0, 0, 1, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}},
                
                {{0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0}}}
    };

    static piece_proto_t i_piece = {
        .spawn_pos_x = 5,
        .spawn_pos_y = 17,
        .size = 4,
        .type = PIECE_I,
        .num_rotations = 2,
        .cell={{{0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}},
                
                {{0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}},
                
            NULL_PIECE,
            NULL_PIECE}
    };

    static piece_proto_t o_piece = {
        .spawn_pos_x = 5,
        .spawn_pos_y = 17,
        .size = 4,
        .type = PIECE_O,
        .num_rotations = 1,
        .cell={{{0, 0, 0, 0},
                {0, 1, 1, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0}},

                NULL_PIECE,
                NULL_PIECE,
                NULL_PIECE}
    };

    switch(type){
        case PIECE_I: 
            return &i_piece;
        case PIECE_J: 
            return &j_piece;
        case PIECE_L:
            return &l_piece;
        case PIECE_O:
            return &o_piece;
        case PIECE_S:
            return &s_piece;
        case PIECE_Z:
            return &z_piece;
        case PIECE_T:
        default:
            return &t_piece;
    }
}


