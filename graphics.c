#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "chess.h"

#define WINDOW_SIZE (8*100)
#define RECT_SIZE   ((WINDOW_SIZE)/8)
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
	ChessBoard     *board;
	SDL_Window     *window;
	SDL_Renderer   *renderer;
	SDL_Texture    *pieces[CHESS_PIECE_TEAM_COUNT][CHESS_PIECE_TYPE_COUNT];
	SDL_Texture    *board_texture;
	ChessPieceTeam pov;
	int 	       selected_square;
} Application;

static Application app;

static void load_textures(void);
static void destroy_textures(void);
static void create_board_texture(void);
static void get_rect_from_position(SDL_Rect *rect, int position);
static int  get_position_on_screen(int x, int y);

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
	
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);

	app.selected_square = -1;
	app.pov = WHITE;

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

	SDL_Surface *board_surface = SDL_CreateRGBSurface(0, WINDOW_SIZE, WINDOW_SIZE, 24, 0,0,0,0);

	SDL_Rect clear = {0, 0, WINDOW_SIZE, WINDOW_SIZE};
	SDL_FillRect(board_surface, &clear, 0x1F1F1F);

	SDL_Rect rects[32];

	for(int i = 0; i < 32; i++){
		int row = i/4;
		int col = i%4;
		rects[i].w = RECT_SIZE;
		rects[i].h = RECT_SIZE;	
		rects[i].x = (2*col + row%2) * RECT_SIZE;
		rects[i].y = row*RECT_SIZE;
	}

	SDL_FillRects(board_surface, rects, 32, 0xBBBBBB);

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

static void get_rect_from_position(SDL_Rect *rect, int position){
	int row = position / 8;
	int col = position % 8;

	rect->w = RECT_SIZE;
	rect->h = RECT_SIZE;

	if(app.pov == WHITE){
		rect->x = col*RECT_SIZE;
		rect->y = (7-row)*RECT_SIZE;
	} else {
		rect->x = (7-col)*RECT_SIZE;
		rect->y = row*RECT_SIZE;
	}

}

void do_draw(void){
	SDL_Rect clear = {0, 0, WINDOW_SIZE, WINDOW_SIZE};
	SDL_RenderCopy(app.renderer, app.board_texture, NULL, &clear);

	// NOTA: Quadrado selecionado
	if(app.selected_square != -1){
		SDL_Rect rect;
		get_rect_from_position(&rect, app.selected_square);

		SDL_SetRenderDrawColor(app.renderer, 243, 156, 18, 150);

		SDL_RenderFillRect(app.renderer, &rect);
	}

	// NOTA: Peças
	for(int i = 0; i < (int)app.board->pieces->len; i++){
		ChessPiece *piece = g_ptr_array_index(app.board->pieces, i);
		SDL_Rect rect;
		get_rect_from_position(&rect, piece->position);

		int team = piece_team(piece->quality);
		int type = piece_type(piece->quality);

		if(team == WHITE){
			SDL_RenderCopy(app.renderer, app.pieces[0][type], NULL, &rect);
		} else {
			SDL_RenderCopy(app.renderer, app.pieces[1][type], NULL, &rect);
		}
		


	}
	
	SDL_RenderPresent(app.renderer);
}

void quit_graphics(void){
	SDL_DestroyWindow(app.window);
	SDL_Quit();
	destroy_textures();
	SDL_DestroyTexture(app.board_texture);
}

static int get_position_on_screen(int x, int y){
	int rx = x*8/WINDOW_SIZE;
	int ry = y*8/WINDOW_SIZE;

	if(app.pov == WHITE)
		return (7-ry)*8 + rx;
	else
		return ry*8     + (7-rx);

}

void do_input(void){
	
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		switch(event.type){

			case SDL_MOUSEBUTTONDOWN: {
					SDL_MouseButtonEvent b_event = event.button;
					if(b_event.button == SDL_BUTTON_RIGHT){
						app.pov = (app.pov == WHITE) ? BLACK : WHITE;
					} else if (b_event.button == SDL_BUTTON_LEFT)
						app.selected_square = get_position_on_screen(b_event.x, b_event.y);
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
