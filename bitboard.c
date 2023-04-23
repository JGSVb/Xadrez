#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define BBOARD_EMPTY  (bboard_t)0x0000000000000000
#define BBOARD_FULL   (bboard_t)0xFFFFFFFFFFFFFFFF
#define BBOARD_SINGLE (bboard_t)0b1

typedef uint64_t bboard_t;

void bboard_print(bboard_t bb);
bboard_t bboard_state(bboard_t bb, uint8_t square);
bboard_t bboard_ltshift(bboard_t bb, bboard_t shift);
bboard_t bboard_rhshift(bboard_t bb, bboard_t shift);
uint8_t bboard_rank(uint8_t index);
uint8_t bboard_file(uint8_t index);
uint8_t bboard_square(uint8_t rank, uint8_t file);

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


int main(int argc, char **argv){
	return 0;
}
