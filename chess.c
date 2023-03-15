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
	for(int i = 0; i < 8; i++){
		ChessPiece *pawn_white = malloc(sizeof(ChessPiece));
		pawn_white->position = 8 + i;
		pawn_white->type = PAWN;
		pawn_white->team = WHITE;
		set_board_piece(board, pawn_white);

		ChessPiece *pawn_black = malloc(sizeof(ChessPiece));
		pawn_black->position = 48 + i;
		pawn_black->type = PAWN;
		pawn_black->team = BLACK;
		set_board_piece(board, pawn_black);
	}

	ChessPieceType piece_order[8] = {
		ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
		};

	for(int i = 0; i < 8; i++){
		ChessPiece *piece_white = malloc(sizeof(ChessPiece));
		piece_white->team = WHITE;
		piece_white->type = piece_order[i];
		piece_white->position = i;

		set_board_piece(board, piece_white);

		ChessPiece *piece_black = malloc(sizeof(ChessPiece));
		piece_black->team = BLACK;
		piece_black->type = piece_order[i];
		piece_black->position = 56 + i;

		set_board_piece(board, piece_black);
	}
}
