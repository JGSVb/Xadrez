#include <stdlib.h>
#include <stdio.h>
#include "chess.h"

#define run_test(name, expr) \
do {\
	int res = expr;\
	if(res!=0){\
		printf("\033[31mTESTE '%s' falhou: %d\033[0m\n", name, res);\
		exit(-1);\
	}\
} while(0)

#define test_case(name, case) \
do {\
	if(!case){\
		printf("TEST %s(" #case ") falhou\n", name);\
		return -1;\
	}\
} while(0)

int test_rook(void){
	ChessBoard *board = new_chess_board();

	ChessPiece *horizontal_move = malloc(sizeof(ChessPiece));
	horizontal_move->position = get_board_index(0, 0);
	horizontal_move->quality = piece_quality(WHITE, ROOK);
	set_board_piece(board, horizontal_move);
	test_case("movimento horizontal da torre",
		  make_a_move(board, horizontal_move->position, horizontal_move->position+7));

	ChessPiece *vertical_move = malloc(sizeof(ChessPiece));
	vertical_move->quality = piece_quality(BLACK, ROOK);
	vertical_move->position = get_board_index(5, 7);
	set_board_piece(board, vertical_move);
	test_case("movimento da torre extrapolante do tabuleiro",
		  make_a_move(board, vertical_move->position, get_board_index(1, 7)));

	ChessPiece *invalid_move = malloc(sizeof(ChessPiece));
	invalid_move->quality = piece_quality(BLACK, ROOK);
	invalid_move->position = get_board_index(0, 0);
	set_board_piece(board, invalid_move);
	test_case("movimento da torre extrapolante do tabuleiro",
		  make_a_move(board, invalid_move->position, invalid_move->position+8*10) == false);

	ChessPiece *random_pawn = malloc(sizeof(ChessPiece));
	random_pawn->quality = piece_quality(BLACK, PAWN);
	random_pawn->position = get_board_index(3, 5);
	set_board_piece(board, random_pawn);
	ChessPiece *rook_through = malloc(sizeof(ChessPiece));
	rook_through->quality = piece_quality(WHITE, ROOK);
	rook_through->position = get_board_index(0, 5);
	set_board_piece(board, rook_through);
	test_case("movimento da torre atravessante sobre um peão",
		  make_a_move(board, rook_through->position, get_board_index(4, 5)) == false);
	
	destroy_chess_board(board);

	return 0;
}

int main(void){
	run_test("movimentos_da_torre", test_rook());

	return 0;
}
