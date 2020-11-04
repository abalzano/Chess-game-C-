#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NO_COLUMNS 8
#define ABS(a) ((a) < (0) ? (-a) : (a))
#define SIGN(a) ((a) < (0) ? (-1) : (1))

enum Color {Empty = 0, White, Black};
enum Game_state {running = 0, quit, egality, white_win, black_win};
enum Piece_id {Empty_id = 0, Pawn_id, Knight_id, 
               Bishop_id, Rook_id, Queen_id, King_id};

#endif