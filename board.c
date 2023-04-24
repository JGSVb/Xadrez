#include <stdlib.h>
#include <stdio.h>
#include "board.h"

static char piece_char[BOARD_TYPE_COUNT] = {
	'w',
	'b',
	'P',
	'N',
	'B',
	'R',
	'Q',
	'K'
};

void chessboard_destroy(ChessBoard *board){
	free(board);
}

ChessBoard *chessboard_new(void){
	ChessBoard *board = malloc(sizeof(ChessBoard));
	board->turn = WHITE;
	for(int i = 0; i < BOARD_TYPE_COUNT; i++){
		board->boards[i] = BBOARD_EMPTY;
	}
	return board;
}

bboard_t chessboard_free(ChessBoard *board){
	return ~chessboard_occupied(board);
}

bboard_t chessboard_occupied(ChessBoard *board){
	return board->boards[WHITE] | board->boards[BLACK];
}

void chessboard_print(ChessBoard *board, board_type_t pov){
	bboard_t occ = chessboard_occupied(board);
	for(int i = 0; i < 63; i++){
		if(bboard_state_2(occ, i) == 1)
			printf("a ");
		else
			printf("b ");

	}
	puts(" ");
}

int main(int argc, char **argv){
	ChessBoard *board = chessboard_new();

	chessboard_print(board, WHITE);

	chessboard_destroy(board);
	return 0;
}
