#include <stdlib.h>
#include <stdio.h>
#include "board.h"

int board(void){
	ChessBoard *board = chessboard_new();
	chessboard_reset(board);

	chessboard_print(board, POV_BLACK);
	chessboard_print(board, POV_WHITE);

	chessboard_destroy(board);
}

int main(void){
	board();

	return 0;
}
