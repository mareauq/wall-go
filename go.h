#ifndef GO_H
#define GO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

/*        MACROS        */

#define Window_height 800
#define Window_width 800
#define X_GAP 65
#define Y_GAP 65
#define SQUARE_SIZE 100
#define X_HWALL_GAP 14 //Media displayed too far away this is used negatively
#define Y_HWALL_GAP 75
#define X_VWALL_GAP 35
#define Y_VWALL_GAP 25
#define WALL_WIDTH 80
#define WALL_HEIGHT 20
#define FPS_DELAY 16

/*      ENUMS       */

// Directions : 0 -> north, 1 -> east, 2 -> south, 3 -> west

enum Directions {
  NORTH,
  EAST,
  SOUTH,
  WEST,
};

// Colors : 0 -> empty, 1 -> RED, 2 -> BLUE, 3 -> GREEN, 4 -> YELLOW

enum Colors {
  EMPTY,
  BLUE,
  RED,
  GREEN,
  YELLOW,
};

enum Players {
  PLAYER_BLUE,
  PLAYER_RED,
  PLAYER_GREEN,
  PLAYER_YELLOW,
};

/* Structures */

struct square
{
    size_t pos[2];
    bool walls[4];
    bool occ; /* Is the frame occupied */
    enum Colors color;
    enum Colors walls_color[4];
};

struct board
{
    struct square*** data;
    size_t size;
};

struct vertex
{
  bool seen;
  struct vertex* neighbours[4];
};

struct graph
{
  size_t size;
  struct vertex** vertices;
  bool** adj_mat;
};


/* Prototypes */

// Elementary functions 

struct board* init_board(size_t size);
void print_board(struct board* board);
void init_walls(struct board* board);
void reset_board(struct board* board);
void adjust_walls(struct board* board);
bool add_piece(struct board* board, Sint32 pos[2], int color);
Sint32* next_pos(Sint32 pos[2], enum Directions direction);
bool move_piece(struct board* board, Sint32 pos[2], enum Directions direction, int color);
bool add_wall(struct board* board, Sint32 pos[2], enum Directions direction, int color);
void free_board(struct board* board);
Sint32 dist(Sint32 pos1[2], Sint32 pos2[2]);
bool is_square_reachable(struct board* board, Sint32 pos1[2], Sint32 pos2[2]);
int move_direction(Sint32 pos1[2], Sint32 pos2[2]);

// Win management

bool** adj_mat_init(struct board* board);
void update_adj_mat(struct board* board, bool** adj_mat);
struct graph* init_graph_board(struct board* board);
void update_graph(struct graph* graph_board);
void coloring_graph(struct graph* graph_board, struct vertex* vertex);
struct vertex** vertex_region(struct graph* graph_board, struct vertex* vertex);
size_t region_size(struct graph* graph_board, struct vertex* vertex);
bool same_region(struct graph* graph_board, struct vertex* vertex1, struct vertex* vertex2);
void free_mat(bool** mat, size_t mat_size);
void free_graph(struct graph* graph_board);
bool can_move(struct board* board, Sint32 pos[2]);
bool can_play(struct board* board, enum Players player);
bool is_game_over(struct board* board, struct graph* graph_board, int score[4], int walls_count[4], int num_players);

// SDL Management functions

SDL_Texture* Load_picture(char* file, SDL_Renderer* renderer);
void in_square(Sint32 x, Sint32 y, Sint32 pos_square[2]);
bool in_rect(Sint32 x, Sint32 y, SDL_Rect rec);
enum Directions on_wall(Sint32 x, Sint32 y, Sint32 pos_square[2]);
void display_piece(SDL_Renderer *renderer, Sint32 pos[2], enum Colors color, SDL_Texture* BluePiece, SDL_Texture* RedPiece,
                                                                 SDL_Texture* GreenPiece, SDL_Texture* YellowPiece, Uint8 alpha);
void display_pieces(struct board* board, SDL_Renderer *renderer, SDL_Texture* BluePiece, SDL_Texture* RedPiece,
                                                                 SDL_Texture* GreenPiece, SDL_Texture* YellowPiece);
void display_wall(SDL_Renderer* renderer, struct square* square, enum Colors color, enum Directions direction, SDL_Texture* BlueWall, SDL_Texture* RedWall,
                                                                                        SDL_Texture* GreenWall, SDL_Texture* YellowWall, Uint8 alpha);
void display_walls(struct board* board, SDL_Renderer* renderer, SDL_Texture* BlueWall, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenWall, SDL_Texture* YellowWall);
void display_board(struct board* board, SDL_Renderer* renderer, SDL_Texture* text_board, SDL_Texture* BluePiece, SDL_Texture* BlueWall, 
                                                                                          SDL_Texture* RedPiece, SDL_Texture* RedWall,
                                                                                          SDL_Texture* GreenPiece, SDL_Texture* GreenWall,
                                                                                          SDL_Texture* YellowPiece, SDL_Texture* YellowWall);


#endif /* ! GO_H */