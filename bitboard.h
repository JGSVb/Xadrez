#pragma once

#include <stdint.h>
#include <stdlib.h>

#define BBOARD_EMPTY     0x0000000000000000ULL
#define BBOARD_FULL      0xFFFFFFFFFFFFFFFFULL
#define BBOARD_SINGLE    0b1ULL

typedef uint64_t bboard_t;

void bboard_print(bboard_t bb);
bboard_t bboard_state(bboard_t bb, uint8_t index);
bboard_t bboard_state_2(bboard_t bb, uint8_t index);
bboard_t bboard_set(bboard_t bb, uint8_t index, bboard_t bb_2);
bboard_t bboard_set_2(bboard_t bb, uint8_t index, bboard_t bb_2);
bboard_t bboard_flip_vertical(bboard_t bb);
bboard_t bboard_flip_horizontal(bboard_t bb);
bboard_t bboard_mask_index(bboard_t bb, uint8_t index);
uint8_t bboard_rank(uint8_t index);
uint8_t bboard_file(uint8_t index);
uint8_t bboard_index(uint8_t rank, uint8_t file);
