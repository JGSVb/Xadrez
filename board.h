#pragma once

#include <stdlib.h>
#include "bitboard.h"

typedef enum {
	WHITE,
	BLACK,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	BOARD_TYPE_COUNT
} board_type_t;

typedef struct _ChessBoard {
	bboard_t boards[BOARD_TYPE_COUNT];
	int turn;
} ChessBoard;

ChessBoard *chessboard_new(void);
void chessboard_destroy(ChessBoard *board);
void chessboard_print(ChessBoard *board, board_type_t pov);
bboard_t chessboard_free(ChessBoard *board);
bboard_t chessboard_occupied(ChessBoard *board);

