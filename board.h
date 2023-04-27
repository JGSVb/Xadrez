#pragma once

#include <stdlib.h>
#include <stdint.h>
#include "bitboard.h"
#include "piece.h"

enum {
	PIECEBB_OCCUPIED,
	PIECEBB_WHITE,
	PIECEBB_WHITE_PAWN,
	PIECEBB_WHITE_KNIGHT,
	PIECEBB_WHITE_BISHOP,
	PIECEBB_WHITE_ROOK,
	PIECEBB_WHITE_QUEEN,
	PIECEBB_WHITE_KING,
	PIECEBB_BLACK,
	PIECEBB_BLACK_PAWN,
	PIECEBB_BLACK_KNIGHT,
	PIECEBB_BLACK_BISHOP,
	PIECEBB_BLACK_ROOK,
	PIECEBB_BLACK_QUEEN,
	PIECEBB_BLACK_KING,
	PIECEBB_COUNT
};

typedef struct _ChessBoard {
	bboard_t piece_bb[PIECEBB_COUNT];
	piece_type_t side_to_move;
} ChessBoard;

ChessBoard *chessboard_new(void);
void chessboard_destroy(ChessBoard *board);
void chessboard_print(ChessBoard *board, piece_type_t pov);
piece_t chessboard_get(ChessBoard *board, uint8_t index);
piece_t chessboard_put(ChessBoard *board, uint8_t index, piece_t piece);
