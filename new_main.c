#include "graphics.h" 
#include "chess.h"

int main(void){
	ChessBoard *board = new_chess_board();
	setup_board(board);
	init_graphics(board);
	do_draw();
	
	while(1)
	do_input();
	
	destroy_chess_board(board);
	quit_graphics();
	return 0;
}
