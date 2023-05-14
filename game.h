#pragma once
#include "board.h"

#define CHESSGAME_BUFSIZE 1028

enum {
	MOVE_SILENT,
	MOVE_CAPTURE,
	MOVE_CASTLE,
	MOVE_EN_PASSANT
};

typedef struct _ChessGame {
	ChessBoard *buffer[CHESSGAME_BUFSIZE];
	ChessBoard *curr;
} ChessGame;

typedef struct _ChessMove {
	int type;
	int src_sq;
	int dst_sq;
} ChessMove;

ChessGame *chessgame_new(void);
void *chessgame_make_a_move(ChessGame *game, ChessMove *move);
