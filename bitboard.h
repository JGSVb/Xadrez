#pragma once

#include <stdint.h>
#include <stdlib.h>

#define A1 0
#define B1 1
#define C1 2
#define D1 3
#define E1 4
#define F1 5
#define G1 6
#define H1 7
#define A2 8
#define B2 9
#define C2 10
#define D2 11
#define E2 12
#define F2 13
#define G2 14
#define H2 15
#define A3 16
#define B3 17
#define C3 18
#define D3 19
#define E3 20
#define F3 21
#define G3 22
#define H3 23
#define A4 24
#define B4 25
#define C4 26
#define D4 27
#define E4 28
#define F4 29
#define G4 30
#define H4 31
#define A5 32
#define B5 33
#define C5 34
#define D5 35
#define E5 36
#define F5 37
#define G5 38
#define H5 39
#define A6 40
#define B6 41
#define C6 42
#define D6 43
#define E6 44
#define F6 45
#define G6 46
#define H6 47
#define A7 48
#define B7 49
#define C7 50
#define D7 51
#define E7 52
#define F7 53
#define G7 54
#define H7 55
#define A8 56
#define B8 57
#define C8 58
#define D8 59
#define E8 60
#define F8 61
#define G8 62
#define H8 63

#define BBOARD_EMPTY     0x0000000000000000ULL
#define BBOARD_FULL      0xFFFFFFFFFFFFFFFFULL
#define BBOARD_SINGLE    0b1ULL

typedef uint64_t bboard_t;

void bboard_print(bboard_t bb);
bboard_t bboard_insert(bboard_t bb, uint8_t st_index, uint8_t en_index, bboard_t bb_2);
bboard_t bboard_getbit(bboard_t bb, uint8_t index);
bboard_t bboard_setbit(bboard_t bb, uint8_t index, bboard_t bb_2);
bboard_t bboard_flip_vertical(bboard_t bb);
bboard_t bboard_flip_horizontal(bboard_t bb);
bboard_t bboard_mask_index(bboard_t bb, uint8_t index);
uint8_t bboard_rank(uint8_t index);
uint8_t bboard_file(uint8_t index);
uint8_t bboard_index(uint8_t rank, uint8_t file);
