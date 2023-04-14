#pragma once
#include <stdlib.h>
#include <glib-2.0/glib-unix.h>
#include <stdbool.h>

#define piece_quality(team, type) ((team) | (type))
#define piece_team(quality) ((quality) & 0xf0)
#define piece_type(quality) ((quality) & 0x0f)

typedef enum {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	CHESS_PIECE_TYPE_COUNT
} ChessPieceType;

typedef enum {
	WHITE = 0x10,
	BLACK = 0x20,
} ChessPieceTeam;

#define CHESS_PIECE_TEAM_COUNT 2

typedef struct {
	int quality;
	int position;
	int space[64];
} ChessPiece;

typedef struct {
	GPtrArray *pieces;
	ChessPiece *squares[64];
	ChessPieceTeam turn;
} ChessBoard;

ChessBoard *new_chess_board(void);
void destroy_chess_board(ChessBoard *board);
ChessPiece *set_board_piece(ChessBoard *board, ChessPiece *piece);
void setup_board(ChessBoard *board);
void print_board(ChessBoard *board, ChessPieceTeam pov);
void get_possible_moves_in_board(ChessBoard *board, int position, int *possible_moves);
bool make_a_move(ChessBoard *board, int src, int dest);



