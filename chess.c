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

#define WP piece_quality(WHITE, PAWN)
#define WN piece_quality(WHITE, KNIGHT)
#define WB piece_quality(WHITE, BISHOP)
#define WR piece_quality(WHITE, ROOK)
#define WQ piece_quality(WHITE, QUEEN)
#define WK piece_quality(WHITE, KING)

#define BP piece_quality(BLACK, PAWN)
#define BN piece_quality(BLACK, KNIGHT)
#define BB piece_quality(BLACK, BISHOP)
#define BR piece_quality(BLACK, ROOK)
#define BQ piece_quality(BLACK, QUEEN)
#define BK piece_quality(BLACK, KING)

void setup_board(ChessBoard *board){
	ChessPieceType setup[64] = {
		WR, WN, WB, WQ, WK, WB, WN, WR,
		WP, WP, WP, WP, WP, WP, WP, WP,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1,
		BP, BP, BP, BP, BP, BP, BP, BP,
		BR, BN, BB, BQ, BK, BB, BN, BR,
	};

	for(int i = 0; i < 64; i++){
		if(setup[i] == -1) continue;

		ChessPiece *piece = malloc(sizeof(ChessPiece));
		piece->position = i;
		piece->quality = setup[i];
		set_board_piece(board, piece);
	}
}
