#pragma once

#include <stdint.h>
#include <stdlib.h>

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
