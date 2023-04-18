#include "chess.h"
#include <glib-2.0/glib-unix.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

static const char *piece_type_chars = "PNBRQK";
static const char *piece_team_chars = "wb";

static void move_piece(ChessBoard *board, int src, int dst);
static void get_row_and_col_for_printing(int index, int *row, int *col, ChessPieceTeam pov);
static void remove_piece(ChessBoard *board, ChessPiece *piece);
static bool check_piece_take(ChessBoard *board, int src, int dst);
static void move_piece(ChessBoard *board, int src, int dst);
static bool check_piece_move(ChessBoard *board, int src, int dst);
static bool subcheck_hv_move(ChessBoard *board, int src, int dst);

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
	if(previous_piece != NULL){
		g_ptr_array_remove(board->pieces, previous_piece);
	}

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


static void move_piece(ChessBoard *board, int src, int dst){
	ChessPiece *src_piece = board->squares[src];

	board->squares[dst] = src_piece;
	board->squares[src] = NULL;
	src_piece->position = dst;
}

static void remove_piece(ChessBoard *board, ChessPiece *piece){
	board->squares[piece->position] = NULL;
	g_ptr_array_remove(board->pieces, piece);
	free(piece);
}

static bool subcheck_hv_move(ChessBoard *board, int src, int dst){
	int src_row, dst_row, src_col, dst_col;
	src_row = get_board_row(src);
	dst_row = get_board_row(dst);
	src_col = get_board_col(src);
	dst_col = get_board_col(dst);

	if(src_col == dst_col || src_row == dst_row){
		int row_diff = dst_row-src_row;
		int col_diff = dst_col-src_col;
		int row_dir = (row_diff == 0 ? 0 : row_diff/abs(row_diff));
		int col_dir = (col_diff == 0 ? 0 : col_diff/abs(col_diff));
		int test_row = src_row;
		int test_col = src_col;

		while(test_row != dst_row-row_dir || test_col != dst_col-col_dir){
			test_row = test_row + row_dir;
			test_col = test_col + col_dir;
			if(board->squares[get_board_index(test_row, test_col)] != NULL){
				return false;
			}
		}
		return true;
	}

	return false;
}

static bool check_piece_move(ChessBoard *board, int src, int dst){
	ChessPiece *src_piece = board->squares[src];
	ChessPiece *dst_piece = board->squares[dst];

	if(src == dst)
		return false;
	if(src > 63 || dst > 63)
		return false;

	if(piece_team(src_piece->quality)!=board->turn ||
	   dst_piece != NULL)
		return false;

	int src_row, dst_row, src_col, dst_col;
	src_row = get_board_row(src);
	dst_row = get_board_row(dst);
	src_col = get_board_col(src);
	dst_col = get_board_col(dst);

	switch(piece_type(src_piece->quality)){
		case PAWN: {
			// podem mover-se apenas verticalmente, como é sabido cá entre nós
			if(src_col != dst_col){
				return false;
			}

			int distance = dst_row - src_row;

			if((distance == 2  && src_row == 1 && board->turn == WHITE) ||
			   (distance == -2 && src_row == 6 && board->turn == BLACK)){
				return true;
			}

			if((distance == 1  && board->turn == WHITE) ||
			   (distance == -1 && board->turn == BLACK)){
				return true;
			}

			return false;
			break; // este break é apenas por cortesia, claro
		}
		case ROOK: {
			return subcheck_hv_move(board, src, dst);
			break;
		}
	}

	return false;
}

static bool check_piece_take(ChessBoard *board, int src, int dst){
	return false;
}

bool make_a_move(ChessBoard *board, int src, int dst){
	if(!check_piece_move(board, src, dst)) return false;
	
	move_piece(board, src, dst);

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
