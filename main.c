#include <SDL2/SDL.h>
#include "graphics.h" 
#include "chess.h"

#define GRAPHICS

int main(void){
	ChessBoard *board = new_chess_board();
	setup_board(board);

	#ifdef GRAPHICS
	init_graphics(board);
	
	while(1) { 
		do_draw();
		do_input();
		SDL_Delay(50);
	}
	quit_graphics();
	#endif

	destroy_chess_board(board);
	return 0;
}
