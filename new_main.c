#include <SDL2/SDL.h>
#include "graphics.h" 
#include "chess.h"

int main(void){
	ChessBoard *board = new_chess_board();
	setup_board(board);
	init_graphics(board);
	
	while(1) { 
		do_draw();
		do_input();
		SDL_Delay(50);
	}
	
	destroy_chess_board(board);
	quit_graphics();
	return 0;
}
