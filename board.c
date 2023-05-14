#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "board.h"

void chessboard_destroy(ChessBoard *board){
	assert(board!= NULL);
	free(board);
}

ChessBoard *chessboard_new(void){
	ChessBoard *board = malloc(sizeof(ChessBoard));
	memset(board, 0, sizeof(ChessBoard));
	board->side_to_move = PIECEBB_WHITE;
	board->castle_rights = CASTLE_RIGHTS_BOTH;
	return board;
}

ChessBoard *chessboard_copy(ChessBoard *src){
	assert(src != NULL);
	ChessBoard *new = chessboard_new();
	for(int i = 0; i < sizeof(new->piece_bb); i++){
		new->piece_bb[i] = src->piece_bb[i];
	}
	new->side_to_move = src->side_to_move;
	new->castle_rights = src->castle_rights;
	new->en_passant_target = src->en_passant_target;
	new->halfmove_clock = src->halfmove_clock;
	return new;
}

void chessboard_reset(ChessBoard *board){
	memset(board, 0, sizeof(board));
	board->side_to_move = PIECEBB_WHITE;
	board->castle_rights = CASTLE_RIGHTS_BOTH;

	for(int i = 0; i < 8; i++){
		chessboard_set(board, bboard_index(1, i), PIECEBB_WHITE, PIECEBB_PAWN, 1);
		chessboard_set(board, bboard_index(6, i), PIECEBB_BLACK, PIECEBB_PAWN, 1);
	}

	chessboard_set(board, A1, PIECEBB_WHITE, PIECEBB_ROOK, 1);
	chessboard_set(board, H1, PIECEBB_WHITE, PIECEBB_ROOK, 1);
	chessboard_set(board, A8, PIECEBB_BLACK, PIECEBB_ROOK, 1);
	chessboard_set(board, H8, PIECEBB_BLACK, PIECEBB_ROOK, 1);

	chessboard_set(board, B1, PIECEBB_WHITE, PIECEBB_KNIGHT, 1);
	chessboard_set(board, G1, PIECEBB_WHITE, PIECEBB_KNIGHT, 1);
	chessboard_set(board, B8, PIECEBB_BLACK, PIECEBB_KNIGHT, 1);
	chessboard_set(board, G8, PIECEBB_BLACK, PIECEBB_KNIGHT, 1);

	chessboard_set(board, C1, PIECEBB_WHITE, PIECEBB_BISHOP, 1);
	chessboard_set(board, F1, PIECEBB_WHITE, PIECEBB_BISHOP, 1);
	chessboard_set(board, C8, PIECEBB_BLACK, PIECEBB_BISHOP, 1);
	chessboard_set(board, F8, PIECEBB_BLACK, PIECEBB_BISHOP, 1);

	chessboard_set(board, D1, PIECEBB_WHITE, PIECEBB_QUEEN, 1);
	chessboard_set(board, D8, PIECEBB_BLACK, PIECEBB_QUEEN, 1);

	chessboard_set(board, E1, PIECEBB_WHITE, PIECEBB_KING, 1);
	chessboard_set(board, E8, PIECEBB_BLACK, PIECEBB_KING, 1);
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
	assert(board!=NULL && (pov == POV_WHITE || pov == POV_BLACK));

	int start, step, fn;
	bboard_t (*flip_function)(bboard_t);

	bboard_t transformed_bb[PIECEBB_COUNT];

	if(pov == POV_WHITE){
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

