#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bitboard.h"

bboard_t bboard_insert(bboard_t bb, uint8_t st_index, uint8_t en_index, bboard_t bb_2){
	assert(st_index < 64 && en_index < 64 && st_index <= en_index);

	bboard_t mask = (BBOARD_FULL<<(bboard_t)st_index) & ~(BBOARD_FULL<<(bboard_t)en_index);
	return (bb & ~mask) | (bb_2 & mask);
}

bboard_t bboard_setbit(bboard_t bb, uint8_t index, bboard_t bb_2){
	bboard_t mask = BBOARD_SINGLE<<(bboard_t)index;
	return (bb & ~mask) | ((bb_2<<(bboard_t)index) & mask);
}

bboard_t bboard_getbit(bboard_t bb, uint8_t index){
	assert(index<=63);
	return (bb>>(bboard_t)index) & BBOARD_SINGLE;
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
}

bboard_t bboard_mask_index(bboard_t bb, uint8_t index){
	return bb & (BBOARD_SINGLE<<index);
}

void bboard_print(bboard_t bb){
	puts("---------------");
	for(int i = 0; i < 64; i++){
		printf("%c ", (bboard_getbit(bb, i) == 1 ? '1' : '.'));
		if((i+1)%8==0) printf("\n");
	}
	puts("---------------");
}
