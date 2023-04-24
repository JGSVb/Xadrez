#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bitboard.h"

bboard_t bboard_set(bboard_t bb, uint8_t index, bboard_t bb_2){
	assert(index<=63);
	bboard_t bb_03 = bb_2<<(bboard_t)index;
	return bb | bb_03 & bb_03;
}

bboard_t bboard_set_2(bboard_t bb, uint8_t index, bboard_t bb_2){
	return bboard_set(bb, index, bb_2&BBOARD_SINGLE);
}

bboard_t bboard_state(bboard_t bb, uint8_t index){
	assert(index<=63);
	return bb>>(bboard_t)index;
}

bboard_t bboard_state_2(bboard_t bb, uint8_t index){
	return bboard_state(bb, index)&BBOARD_SINGLE;
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

// https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating#Vertical
bboard_t bboard_flip_vertical(bboard_t bb){
	return  ((bb<<56ULL)) |
		((bb<<40ULL)&0x00FF000000000000ULL) |
		((bb<<24ULL)&0x0000FF0000000000ULL) |
		((bb<<8ULL) &0x000000FF00000000ULL) |
		((bb>>8ULL) &0x00000000FF000000ULL) |
		((bb>>24ULL)&0x0000000000FF0000ULL) |
		((bb>>40ULL)&0x000000000000FF00ULL) |
		((bb>>56ULL));
}

#define BBOARD_K1 0x5555555555555555ULL
#define BBOARD_K2 0x3333333333333333ULL
#define BBOARD_K4 0x0F0F0F0F0F0F0F0FULL

// https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating#Horizontal
bboard_t bboard_flip_horizontal(bboard_t bb){
	bb = ((bb >> 1) & BBOARD_K1) | ((bb & BBOARD_K1) << 1);
	bb = ((bb >> 2) & BBOARD_K2) | ((bb & BBOARD_K2) << 2);
	bb = ((bb >> 4) & BBOARD_K4) | ((bb & BBOARD_K4) << 4);
	return bb;
};

void bboard_print(bboard_t bb){
	puts("---------------");
	for(int i = 0; i < 64; i++){
		printf("%c ", (bboard_state_2(bb, i) == 1 ? '1' : '.'));
		if((i+1)%8==0) printf("\n");
	}
	puts("---------------");
}
