#include "chess.h"
#include <glib-2.0/glib-unix.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

static const char *piece_type_chars = "PNBRQK";
static const char *piece_team_chars = "wb";

static void move_piece_in_board(ChessBoard *board, int src, int dst);
static void get_row_and_col_for_printing(int index, int *row, int *col, ChessPieceTeam pov);
static void classify_move(int src, int dst, char *direction, int *magnitude);

ChessBoard *new_chess_board(void){
	ChessBoard *board = malloc(sizeof(ChessBoard));
	board->pieces = g_ptr_array_new();

	for(int i = 0; i < 64; i++){
		board->squares[i] = NULL;
	}

	board->turn = WHITE;

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

static void get_row_and_col_for_printing(int index, int *row, int *col, ChessPieceTeam pov){
	*row = index/8;
	*col = index%8;

	if(pov == WHITE){
		*row = 7-(*row);
	}
	if(pov == BLACK){
		*col = 7-(*col);
	}
}
void print_board(ChessBoard *board, ChessPieceTeam pov){
	for(int i = 0; i < 64; i++){

		int row, col;
		get_row_and_col_for_printing(i, &row, &col, pov);

		int proper_index = row*8 + col;

		if(i%8 == 0) printf("%u   ", row+1);

		if(board->squares[proper_index] == NULL){
			printf("*  ");
		} else {
			ChessPiece *piece = board->squares[proper_index];
			ChessPieceType type = piece_type(piece->quality);
			ChessPieceTeam team = piece_team(piece->quality);

			char c_team = (team == WHITE ? piece_team_chars[0] : piece_team_chars[1]);
			printf("%c%c ", c_team, piece_type_chars[type]);
		}
		if((i+1)%8 == 0) printf("\n");
	}

	printf("\n    ");
	for(int i = 0; i < 8; i++) {
		int col, _;
		get_row_and_col_for_printing(i, &_, &col, pov);
		printf("%c  ", 'a' + col);
	};

	printf("\n\n");
}

/*
* n:NORTE => direção brancas -> negras;
* s:SUL   => direção negras  -> brancas;
* e:ESTE  => direção ala da dama -> ala do rei;
* w:OESTE => direção ala da rei  -> ala da dama;
*/
static void classify_move(int src, int dst, char *direction, int *magnitude){
	int diff = dst-src;

	if(diff%8 == 0){
		*magnitude = diff/8;
		*direction = (magnitude > 0 ? 'n' : 's');
	} else if(get_board_row(src) == get_board_row(dst)){
		*magnitude = diff;
		*direction = (magnitude > 0 ? 'e' : 'w');
	}
}

static void move_piece_in_board(ChessBoard *board, int src, int dst){
	ChessPiece *dst_piece = board->squares[dst];
	ChessPiece *src_piece = board->squares[src];

	board->squares[dst] = src_piece;
	board->squares[src]  = NULL;
	src_piece->position = dst;

	if(dst_piece){
		g_ptr_array_remove(board->pieces, dst_piece);
		free(dst_piece);
	}
}

bool make_a_move(ChessBoard *board, int src, int dst){
	ChessPiece *src_piece = board->squares[src];
	ChessPiece *dst_piece = board->squares[dst];

	if((dst_piece != NULL && piece_team(dst_piece->quality) == board->turn) ||
		piece_team(src_piece->quality) != board->turn) return false;

	switch(piece_type(src_piece->quality)){
		case PAWN: {
			int diff = dst-src;
			if(!((diff == 8  && board->turn == WHITE) ||
			     (diff == -8 && board->turn == BLACK))) return false;
			break;
		}
	}
	
	move_piece_in_board(board, src, dst);

	board->turn = (board->turn == WHITE ? BLACK : WHITE);

	return true;
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
