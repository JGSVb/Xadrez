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

	ChessPiece *test_rook = malloc(sizeof(ChessPiece));
	test_rook->position = get_board_index(0, 0);
	test_rook->quality = piece_quality(WHITE, ROOK);
	set_board_piece(board, test_rook);
	test_case("movimento horizontal da torre",
		  make_a_move(board, test_rook->position, test_rook->position+7));

	return 0;
}

int main(void){
	run_test("movimentos_da_torre", test_rook());

	return 0;
}
