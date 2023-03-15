#pragma once
#include <stdlib.h>
#include <glib-2.0/glib-unix.h>

typedef enum {
	PAWN,
	BISHOP,
	KNIGHT,
	ROOK,
	QUEEN,
	KING,
	CHESS_PIECE_TYPE_COUNT
} ChessPieceType;

typedef enum {
	WHITE,
	BLACK,
	CHESS_PIECE_TEAM_COUNT
} ChessPieceTeam;

typedef struct {
	int team;
	int type;
	int position;
	int space[64];
} ChessPiece;

typedef struct {
	GPtrArray *pieces;
	ChessPiece *squares[64];
} ChessBoard;

ChessBoard *new_chess_board(void);
void destroy_chess_board(ChessBoard *board);
ChessPiece *set_board_piece(ChessBoard *board, ChessPiece *piece);
void setup_board(ChessBoard *board);


