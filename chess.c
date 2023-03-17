#include "chess.h"
#include <glib-2.0/glib-unix.h>

ChessBoard *new_chess_board(void){
	ChessBoard *board = malloc(sizeof(ChessBoard));
	board->pieces = g_ptr_array_new();

	for(int i = 0; i < 64; i++){
		board->squares[i] = NULL;
	}

	return board;
}

void destroy_chess_board(ChessBoard *board){
	g_ptr_array_free(board->pieces, TRUE);
	free(board);
}

ChessPiece *set_board_piece(ChessBoard *board, ChessPiece *piece){
	ChessPiece *previous_piece = board->squares[piece->position];
	if(previous_piece != NULL) g_ptr_array_remove(board->pieces, previous_piece);

	g_ptr_array_add(board->pieces, piece);
	board->squares[piece->position] = piece;
	
	return previous_piece;
}

void setup_board(ChessBoard *board){
	ChessPieceType setup[64] = {
		ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
		PAWN, PAWN,   PAWN,   PAWN,  PAWN, PAWN,   PAWN,   PAWN,
		-1,  -1,      -1,      -1,   -1,   -1,     -1,     -1,
		-1,  -1,      -1,      -1,   -1,   -1,     -1,     -1,
		-1,  -1,      -1,      -1,   -1,   -1,     -1,     -1,
		-1,  -1,      -1,      -1,   -1,   -1,     -1,     -1,
		PAWN, PAWN,   PAWN, PAWN,    PAWN, PAWN,   PAWN,   PAWN,
		ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
	};

	ChessPieceTeam territory[64] = {
		WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
		WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
		0,     0,     0,     0,     0,    0,      0,     0,
		0,     0,     0,     0,     0,    0,      0,     0,
		0,     0,     0,     0,     0,    0,      0,     0,
		0,     0,     0,     0,     0,    0,      0,     0,
		BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
	};

	for(int i = 0; i < 64; i++){
		if(setup[i] == -1) continue;

		ChessPiece *piece = malloc(sizeof(ChessPiece));
		piece->position = i;
		piece->type = setup[i];
		piece->team = territory[i];
		set_board_piece(board, piece);
	}
}
