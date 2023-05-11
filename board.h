#pragma once

#include <stdlib.h>
#include <stdint.h>
#include "bitboard.h"

#define UNPKPAWN(color)   PIECEBB_PAWN,   (color)
#define UNPKKNIGHT(color) PIECEBB_KNIGHT, (color)
#define UNPKBISHOP(color) PIECEBB_BISHOP, (color)
#define UNPKROOK(color)   PIECEBB_ROOK,   (color)
#define UNPKQUEEN(color)  PIECEBB_QUEEN,  (color)
#define UNPKKING(color)   PIECEBB_KING,   (color)

typedef enum {
	PIECEBB_OCCUPIED,
	PIECEBB_WHITE,
	PIECEBB_BLACK,
	PIECEBB_PAWN,
	PIECEBB_KNIGHT,
	PIECEBB_BISHOP,
	PIECEBB_ROOK,
	PIECEBB_QUEEN,
	PIECEBB_KING,
	PIECEBB_COUNT
} piecebb_t;

typedef enum {
	CHESSBOARD_WHITE,
	CHESSBOARD_BLACK,
} printpov_t;

typedef struct _ChessBoard {
	bboard_t piece_bb[PIECEBB_COUNT];
	piecebb_t side_to_move;
} ChessBoard;

ChessBoard *chessboard_new(void);
ChessBoard *chessboard_copy(ChessBoard *src);
void chessboard_destroy(ChessBoard *board);
void chessboard_print(ChessBoard *board, printpov_t pov);
void chessboard_set(ChessBoard *board, uint8_t index, piecebb_t color, piecebb_t type, uint8_t state);
