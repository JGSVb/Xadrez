#pragma once

#include <stdint.h>

/*
* 0b00000000
*   ....     cor
*       .... tipo
*
* NOTE: se eventualmente for preciso algum espaço para
* a posição da peça eu meto
*/

#define piece_new(color, type)  ((piece_t)((color) | (type)))
#define piece_color(piece)	((piece)&0xF0)
#define piece_type(piece)  	((piece)&0x0F)

typedef uint8_t piece_t;

typedef enum {
	PIECE_PAWN = 0x01,
	PIECE_KNIGHT,
	PIECE_BISHOP,
	PIECE_ROOK,
	PIECE_QUEEN,
	PIECE_KING,
} piece_type_t;

typedef enum {
	PIECE_WHITE = 0x10,
	PIECE_BLACK = 0x20,
} piece_color_t;

#define PIECE_NONE (0)
// 2 (branco ou preto) * 6 (peão, cavalo, ...)
#define PIECE_POSSIBILITIES (12)
