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

void bboard_print(bboard_t bb){
	puts("---------------");
	for(int i = 0; i < 64; i++){
		printf("%d ", bboard_state_2(bb, i));
		if((i+1)%8==0) printf("\n");
	}
	puts("---------------");
}

int main(void){
	bboard_t bb = 0b1110001111000001101000100101010010111101010110010111011010100111;
	bboard_print(bb);
	bboard_print(bboard_flip_vertical(bb));
	return 0;
}
