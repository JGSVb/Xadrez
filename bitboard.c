#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bitboard.h"

bboard_t bboard_set(bboard_t bb, uint8_t index, bboard_t bb_2){
	assert(index<=63);
	return bb||bboard_ltshift(bb_2, index);
}

bboard_t bboard_ltshift(bboard_t bb, bboard_t shift){
	return bb<<shift;
}
bboard_t bboard_rhshift(bboard_t bb, bboard_t shift){
	return bb>>shift;
}

bboard_t bboard_state(bboard_t bb, uint8_t square){
	assert(square<=63);
	return bboard_rhshift(bb,square) & BBOARD_SINGLE;
}

uint8_t bboard_index(uint8_t rank, uint8_t file){
	return rank * 8 + file;
}

uint8_t bboard_rank(uint8_t index){
	return index >> 3;
}

uint8_t bboard_file(uint8_t index){
	return index & 7;
}


void bboard_print(bboard_t bb){
	puts("---------------");
	for(int i = 0; i < 64; i++){
		printf("%d ", bboard_state(bb, i));
		if((i+1)%8==0) printf("\n");
	}
	puts("---------------");
}
