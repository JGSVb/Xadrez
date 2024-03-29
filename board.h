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
	POV_WHITE,
	POV_BLACK,
} printpov_t;

typedef enum {
	CASTLE_RIGHTS_NONE  = 0,
	CASTLE_RIGHTS_BLACK = 0x0F,
	CASTLE_RIGHTS_WHITE = 0xF0,
	CASTLE_RIGHTS_BOTH  = 0xFF,
} castle_rights_t;

typedef struct _ChessBoard {
	bboard_t piece_bb[PIECEBB_COUNT];
	bboard_t en_passant_target;
	piecebb_t side_to_move;
	castle_rights_t castle_rights;
	int halfmove_clock;
} ChessBoard;

ChessBoard *chessboard_new(void);
ChessBoard *chessboard_copy(ChessBoard *src);
void chessboard_reset(ChessBoard *board);
void chessboard_destroy(ChessBoard *board);
void chessboard_print(ChessBoard *board, printpov_t pov);
void chessboard_set(ChessBoard *board, uint8_t index, piecebb_t color, piecebb_t type, uint8_t state);
