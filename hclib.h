#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char board_t[8][8];
//row "rank" 0-7/"1-8"
//column "file" 0-7/"1-8"
typedef struct{
    char bprison[16];
    char bairfield[16];

    board_t board;

    char wprison[16];
    char wairfield[16];

} exboard_t;

typedef struct{
    int from_i, from_j;
    int to_i, to_j;
    char promotion;
    char hostage;
} move_t;

exboard_t *newboard();
exboard_t *copyboard(exboard_t *board);
char *stringboard(exboard_t *board);
exboard_t *apply_move(exboard_t *board, move_t *move);
move_t **moves(board_t *board, int from_i, int from_j);
move_t **knightmoves(board_t *board, int from_i, int from_j, int colour);
move_t **bishopmoves(board_t *board, int from_i, int from_j, int colour);
move_t **rookmoves(board_t *board, int from_i, int from_j, int colour);
move_t **queenmoves(board_t *board, int from_i, int from_j, int colour);
move_t **king_moves(board_t *board, int from_i, int from_j, int colour);
move_t **pawn_moves(board_t *board, int from_i, int from_j, int colour);
char *fen1(exboard_t *board, char *active, char *castling, char *enpassant, int half, int full);
char *fen(exboard_t *board, char *active, char *castling, char *enpassant, int half, int full);
exboard_t *boardstring( char *string );