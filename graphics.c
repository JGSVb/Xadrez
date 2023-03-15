#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "chess.h"

#define WINDOW_SIZE (8*100)
#define UPDATE_TIME (1000/20)
#define TEXTURE_FOLDER "/home/vieira/Testes/xadrez/pieces/"

static const char *chess_pieces_filename[CHESS_PIECE_TEAM_COUNT][CHESS_PIECE_TYPE_COUNT] = {
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
	ChessBoard   *board;
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *pieces[CHESS_PIECE_TEAM_COUNT][CHESS_PIECE_TYPE_COUNT];
	SDL_Texture  *board_texture;
	int 	      selected_square;
} Application;

static Application app;

static void load_textures(void);
static void destroy_textures(void);
static void create_board_texture(void);

void init_graphics(ChessBoard *board){

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Impossível inicializar SDL: %s\n", SDL_GetError());
		exit(1);	
	}

	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	
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


	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	if(app.renderer == NULL){
		printf("Impossível criar janela: %s\n", SDL_GetError());
		exit(1);	
	}

	app.selected_square = -1;

	app.board = board;

	load_textures();
	create_board_texture();
}

static void load_textures(void){
	
	for(int k = 0; k < CHESS_PIECE_TEAM_COUNT; k++){
		for(int i = 0; i < CHESS_PIECE_TYPE_COUNT; i++){
			char dest[256] = TEXTURE_FOLDER;

			SDL_Surface *image;
			SDL_Texture *texture;

			strcat(dest, chess_pieces_filename[k][i]);

			if((image = IMG_Load(dest)) == NULL){
				printf("Impossível carregar imagem '%s': %s\n", dest, IMG_GetError());
				quit_graphics();
				exit(1);
			}
			

			if((texture = SDL_CreateTextureFromSurface(app.renderer, image)) == NULL){
				printf("Impossível carregar textura da imagem '%s': %s\n", dest, IMG_GetError());
				quit_graphics();
				exit(1);
			}	

			app.pieces[k][i] = texture;

			SDL_FreeSurface(image);
		}
	}
}

static void create_board_texture(void){
	int rect_size = WINDOW_SIZE/8;

	SDL_Surface *board_surface = SDL_CreateRGBSurface(0, WINDOW_SIZE, WINDOW_SIZE, 24, 0,0,0,0);

	SDL_Rect clear = {0, 0, WINDOW_SIZE, WINDOW_SIZE};
	SDL_FillRect(board_surface, &clear, 0xBBBBBB);

	SDL_Rect rects[32];

	for(int i = 0; i < 32; i++){
		int row = i/4;
		int col = i%4;
		rects[i].w = rect_size;
		rects[i].h = rect_size;	
		rects[i].x = (2*col + row%2) * rect_size;
		rects[i].y = row*rect_size;
	}

	SDL_FillRects(board_surface, rects, 32, 0x1F1F1F);

	SDL_Texture *board_texture = SDL_CreateTextureFromSurface(app.renderer, board_surface);
	SDL_FreeSurface(board_surface);

	app.board_texture = board_texture;
}

static void destroy_textures(void){
	for(int k = 0; k < CHESS_PIECE_TEAM_COUNT; k++){
		for(int i = 0; i < CHESS_PIECE_TYPE_COUNT; i++){
			SDL_DestroyTexture(app.pieces[k][i]);
		}

	}
}

void do_draw(void){

	SDL_Rect clear = {0, 0, WINDOW_SIZE, WINDOW_SIZE};
	SDL_RenderCopy(app.renderer, app.board_texture, NULL, &clear);

	int rect_size = WINDOW_SIZE/8;
	
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
	for(int i = 0; i < (int)app.board->pieces->len; i++){
		ChessPiece *piece = g_ptr_array_index(app.board->pieces, i);
		int row = piece->position/8;
		int col = piece->position%8;
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

void quit_graphics(void){
	SDL_DestroyWindow(app.window);
	SDL_Quit();
	destroy_textures();
	SDL_DestroyTexture(app.board_texture);
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
					int previous_selected_square = app.selected_square;

					if(previous_selected_square == clicked_square)
						app.selected_square = -1;

					app.selected_square = clicked_square;
					break;
			}
			case SDL_QUIT:
				quit_graphics();
				exit(0);
				break;

			default:
				break;
		}
	}
}
