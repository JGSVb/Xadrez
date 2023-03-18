#include "chess.h"
#include <glib-2.0/glib-unix.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>

static const wchar_t *piece_chars[CHESS_PIECE_TEAM_COUNT] = {
		L"♙♘♗♖♕♔",
		L"♟︎♞♝♜♛♚"
};

void init_chess(void){
    setlocale(LC_CTYPE, "");	
}

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

// TODO: finalizar
void print_board(ChessBoard *board){
	for(int i = 0; i < 64; i++){
		if(board->squares[i] == NULL){
			printf("* ");
		} else {
			char c;
			ChessPiece *piece = board->squares[(63-i)];
			ChessPieceType type = piece_type(piece->quality);
			ChessPieceTeam team = piece_team(piece->quality);
			if(team == WHITE)
				wprintf(L"%lc ", piece_chars[0][type]);
			else
				wprintf(L"%lc ", piece_chars[1][type]);
		}
		if((i+1)%8 == 0) wprintf(L"\n");
	}
	wprintf(L"\n");
}

ChessPiece *move_piece_in_board(ChessBoard *board, int src, int dest, bool replace){
	ChessPiece *previous_piece = board->squares[dest];

	if(previous_piece && !replace) return previous_piece;

	ChessPiece *moving = board->squares[src];
	board->squares[dest] = moving;
	board->squares[src]  = NULL;
	moving->position = dest;

	if(previous_piece){
		g_ptr_array_remove(board->pieces, previous_piece);
	}

	return previous_piece;

}

void get_possible_moves_in_board(ChessBoard *board, int position, int *possible_moves){
	

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
