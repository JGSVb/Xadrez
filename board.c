#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "board.h"

void chessboard_destroy(ChessBoard *board){
	assert(board!= NULL);
	free(board);
}

ChessBoard *chessboard_new(void){
	ChessBoard *board = malloc(sizeof(ChessBoard));
	board->side_to_move = PIECEBB_WHITE;
	for(int i = 0; i < sizeof(board->piece_bb)/sizeof(bboard_t); i++){
		board->piece_bb[i] = BBOARD_EMPTY;
	}
	return board;
}

void chessboard_set(ChessBoard *board, uint8_t index, piecebb_t color, piecebb_t type, uint8_t state){
	assert(board != NULL && index < 64 && (state == 1 || state == 0));
	bboard_t bb;

	if(state == 0){
		for(int i = 0; i < PIECEBB_COUNT; i++){
			bb = board->piece_bb[i];
			board->piece_bb[i] = bboard_setbit(bb, index, 0);
		}
	} else {
		bb = board->piece_bb[PIECEBB_OCCUPIED];
		bb = bboard_setbit(bb, index, 1);
		board->piece_bb[PIECEBB_OCCUPIED] = bb;

		bb = board->piece_bb[color];
		bb = bboard_setbit(bb, index, 1);
		board->piece_bb[color] = bb;

		bb = board->piece_bb[type];
		bb = bboard_setbit(bb, index, 1);
		board->piece_bb[type] = bb;
	}
}

static const char *print_chars = " wbPNBRQK";

void chessboard_print(ChessBoard *board, printpov_t pov){
	assert(board!=NULL && (pov == CHESSBOARD_WHITE || pov == CHESSBOARD_BLACK));

	int start, step, fn;
	bboard_t (*flip_function)(bboard_t);

	bboard_t transformed_bb[PIECEBB_COUNT];

	if(pov == CHESSBOARD_WHITE){
		flip_function = bboard_flip_vertical;
		start = 0;
		step = 1;
		fn = 8;
	} else {
		flip_function = bboard_flip_horizontal;
		start = 7;
		step = -1;
		fn = 1;
	}

	for(int ii = 0; ii < PIECEBB_COUNT; ii++){
		transformed_bb[ii] = flip_function(board->piece_bb[ii]);
	}

	for(int ii = 0; ii < 64; ii++){
		if(ii%8 == 0){
			printf("\n%d ", fn);
			fn -= step;
		}

		if(!bboard_getbit(transformed_bb[PIECEBB_OCCUPIED], ii)){
			printf("**  ");
		} else {
			for(int jj = 1; jj < PIECEBB_COUNT; jj++){
				if(bboard_getbit(transformed_bb[jj], ii)){
					putchar(print_chars[jj]);
				}
			}
			printf("  ");
		}

		if((ii+1)%8 == 0) {
			printf("\n");
		}
		
	}

	int ii = start;
	for(int kk = 0; kk < 8; kk++){
		printf("  %c ", 'a'+ii);
		ii += step;
	}

	printf("\n");
}

int main(int argc, char **argv){
	ChessBoard *board = chessboard_new();

	chessboard_set(board, 35, UNPKPAWN(PIECEBB_WHITE), 1);

	chessboard_print(board, CHESSBOARD_WHITE);
	chessboard_print(board, CHESSBOARD_BLACK);

	chessboard_destroy(board);
	return 0;
}
