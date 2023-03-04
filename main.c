#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <glib-2.0/glib-unix.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#define WINDOW_SIZE (8*100)
#define UPDATE_TIME (1000/20)
#define TEXTURE_FOLDER "/home/vieira/Testes/xadrez/pieces/"

typedef enum {
	PAWN,
	BISHOP,
	KNIGHT,
	ROOK,
	QUEEN,
	KING,
	CHESS_PIECE_TYPE_COUNT
} ChessPieceType;

typedef enum {
	WHITE,
	BLACK,
	CHESS_PIECE_TEAM_COUNT
} ChessPieceTeam;

const char *chess_pieces_filename[CHESS_PIECE_TEAM_COUNT][CHESS_PIECE_TYPE_COUNT] = {
	{ "w_pawn.svg",
	  "w_bishop.svg",
	  "w_knight.svg",
	  "w_rook.svg",
	  "w_queen.svg",
	  "w_king.svg"
	},
	{ "b_pawn.svg",
	  "b_bishop.svg",
	  "b_knight.svg",
	  "b_rook.svg",
	  "b_queen.svg",
	  "b_king.svg"
	}
};

typedef struct {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *pieces[CHESS_PIECE_TEAM_COUNT][CHESS_PIECE_TYPE_COUNT];
	int 	      selected_square;
} Application;

typedef struct {
	int team;
	int type;
	int position;
	int space[64];
} ChessPiece;

typedef struct {
	GPtrArray *pieces;
	ChessPiece *squares[64];
} ChessBoard;

Application app;
ChessBoard  board;

void init_sdl(void);
void load_textures(void);
void destroy_textures(void);
void quit_sdl(void);
void finish(void);
void do_input(void);
void do_draw(void);
void init_board(void);
void finish_board(void);
ChessPiece *set_piece(ChessPiece *piece);
void init(void);
void setup_board(void);

void init_sdl(void){

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Impossível inicializar SDL: %s\n", SDL_GetError());
		exit(1);	
	}

	
	memset(&app, 0, sizeof(Application));
	app.window = SDL_CreateWindow("Xadrez", SDL_WINDOWPOS_UNDEFINED,
						SDL_WINDOWPOS_UNDEFINED,
						WINDOW_SIZE,
						WINDOW_SIZE,
						0);

	if(app.window == NULL){
		printf("Impossível criar janela: %s\n", SDL_GetError());
		exit(1);	
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	if(app.renderer == NULL){
		printf("Impossível criar janela: %s\n", SDL_GetError());
		exit(1);	
	}

	app.selected_square = -1;

}

void load_textures(void){
	
	for(int k = 0; k < CHESS_PIECE_TEAM_COUNT; k++){
		for(int i = 0; i < CHESS_PIECE_TYPE_COUNT; i++){
			char dest[256] = TEXTURE_FOLDER;

			SDL_Surface *image;
			SDL_Texture *texture;

			strcat(dest, chess_pieces_filename[k][i]);

			if((image = IMG_Load(dest)) == NULL){
				printf("Impossível carregar imagem '%s': %s\n", dest, IMG_GetError());
				quit_sdl();
				exit(1);
			}
			
			if((texture = SDL_CreateTextureFromSurface(app.renderer, image)) == NULL){
				printf("Impossível carregar textura da imagem '%s': %s\n", dest, IMG_GetError());
				quit_sdl();
				exit(1);
			}	

			app.pieces[k][i] = texture;

			SDL_FreeSurface(image);
		}
	}
}

void destroy_textures(void){
	for(int k = 0; k < CHESS_PIECE_TEAM_COUNT; k++){
		for(int i = 0; i < CHESS_PIECE_TYPE_COUNT; i++){
			SDL_DestroyTexture(app.pieces[k][i]);
		}
	}
}

void quit_sdl(void){
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}

void finish_board(void){
	g_ptr_array_free(board.pieces, TRUE);
}

void finish(void){
	destroy_textures();
	quit_sdl();
	finish_board();
}
 
void do_input(void){
	
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_MOUSEBUTTONDOWN: {
					int rect_size = WINDOW_SIZE / 8;
					int ty = event.button.y / rect_size;
					int tx = event.button.x / rect_size;
					int clicked_square = ty * 8 + tx;

					if(app.selected_square == clicked_square)
						app.selected_square = -1;
					else {
						ChessPiece *piece;
						int previous_square = app.selected_square;
						app.selected_square = clicked_square;
						if(previous_square == -1) break;
						if((piece = board.squares[previous_square]) != NULL){
							piece->position = clicked_square;
							if(board.squares[piece->position] != NULL){
								ChessPiece *previous_piece = board.squares[piece->position];
								g_ptr_array_remove(board.pieces, previous_piece);
								free(previous_piece);
								board.squares[piece->position] = NULL;
							}
							board.squares[piece->position] = piece;
						}
					}
					break;
			}
			case SDL_QUIT:
				finish();
				exit(0);
				break;

			default:
				break;
		}
	}
}

void do_draw(void){

	int rect_size = WINDOW_SIZE/8;

	// NOTA: Tabuleiro
	SDL_SetRenderDrawColor(app.renderer, 200, 200, 200, 255);
	SDL_RenderClear(app.renderer);

	SDL_Rect rects[32];

	for(int i = 0; i < 32; i++){
		int row = i/4;
		int col = i%4;
		rects[i].w = rect_size;
		rects[i].h = rect_size;	
		rects[i].x = (2*col + row%2) * rect_size;
		rects[i].y = row*rect_size;
	}

	SDL_SetRenderDrawColor(app.renderer, 30, 30, 30, 255);
	SDL_RenderFillRects(app.renderer, rects, 32);

	// NOTA: Quadrado selecionado
	if(app.selected_square != -1){
		int x = app.selected_square % 8 * rect_size;
		int y = app.selected_square / 8 * rect_size;
		SDL_Rect rect = {
			.x = x,
			.y = y,
			.w = rect_size,
			.h = rect_size
		};
		SDL_SetRenderDrawColor(app.renderer, 200, 30, 30, 255);
		SDL_RenderFillRect(app.renderer, &rect);
	}

	// NOTA: Peças
	for(int i = 0; i < (int)board.pieces->len; i++){
		ChessPiece *piece = g_ptr_array_index(board.pieces, i);
		int row = piece->position / 8;
		int col = piece->position % 8;
		SDL_Rect rect = {
			.w = rect_size,
			.h = rect_size,
			.x = rect_size*col,
			.y = rect_size*row

		};

		SDL_RenderCopy(app.renderer, app.pieces[piece->team][piece->type], NULL, &rect);

	}
	
	SDL_RenderPresent(app.renderer);
}

void init_board(void){
	board.pieces = g_ptr_array_new();

	for(int i = 0; i < 64; i++){
		board.squares[i] = NULL;
	}
}

ChessPiece *set_piece(ChessPiece *piece){
	ChessPiece *previous_piece = board.squares[piece->position];
	if(previous_piece != NULL) g_ptr_array_remove(board.pieces, previous_piece);

	g_ptr_array_add(board.pieces, piece);
	board.squares[piece->position] = piece;
	
	return previous_piece;
}

// TODO: quando precisar de uma função para remover as peças, terei que fazê-la


void init(void){
	init_sdl();
	init_board();
	load_textures();
}

void setup_board(void){
	for(int i = 0; i < 8; i++){
		ChessPiece *pawn_white = malloc(sizeof(ChessPiece));
		pawn_white->position = 8 + i;
		pawn_white->type = PAWN;
		pawn_white->team = WHITE;
		set_piece(pawn_white);

		ChessPiece *pawn_black = malloc(sizeof(ChessPiece));
		pawn_black->position = 48 + i;
		pawn_black->type = PAWN;
		pawn_black->team = BLACK;
		set_piece(pawn_black);
	}

	ChessPieceType piece_order[8] = {
		ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
		};

	for(int i = 0; i < 8; i++){
		ChessPiece *piece_white = malloc(sizeof(ChessPiece));
		piece_white->team = WHITE;
		piece_white->type = piece_order[i];
		piece_white->position = i;

		set_piece(piece_white);

		ChessPiece *piece_black = malloc(sizeof(ChessPiece));
		piece_black->team = BLACK;
		piece_black->type = piece_order[i];
		piece_black->position = 56 + i;

		set_piece(piece_black);
	}
}

int main(int argc, char **argv){

	init();
	setup_board();

	while(true){
		do_input();
		SDL_Delay(UPDATE_TIME);
		do_draw();
	}

	finish();

	return 0;
}
