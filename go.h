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
#define X_GAP 60
#define Y_GAP 60
#define SQUARE_SIZE 100
#define X_HWALL_GAP 9 //Media displayed too far away this is used negatively
#define Y_HWALL_GAP 80
#define X_VWALL_GAP 40
#define Y_VWALL_GAP 30 
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

/* Structures */

struct square
{
    size_t pos[2];
    // Bools for walls
    // bool north;
    // bool east;
    // bool south;
    // bool west;
    bool walls[4];
    bool occ; /* Is the frame occupied */
    enum Colors color;
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
void adjust_walls(struct board* board);
bool add_piece(struct board* board, size_t pos[2], int color);
size_t* next_pos(size_t pos[2], enum Directions direction);
bool move_piece(struct board* board, size_t pos[2], enum Directions direction, int color);
bool add_wall(struct board* board, size_t pos[2], enum Directions direction, int color);
//size_t dist(size_t pos1[2], size_t pos2[2]); // Obsolete

// Win management

bool** adj_mat_init(struct board* board);
void update_adj_mat(struct board* board, bool** adj_mat);
struct graph* init_graph_board(struct board* board);
void update_graph(struct graph* graph_board);
void coloring_graph(struct graph* graph_board, struct vertex* vertex);
struct vertex** vertex_region(struct graph* graph_board, struct vertex* vertex);
size_t region_size(struct graph* graph_board, struct vertex* vertex);
bool same_region(struct graph* graph_board, struct vertex* vertex1, struct vertex* vertex2);

// SDL Management functions

SDL_Texture* Load_picture(char* file, SDL_Renderer* renderer);
size_t* in_square(Sint32 x, Sint32 y);
void display_pieces(struct board* board, SDL_Renderer *renderer, SDL_Texture* BluePiece, SDL_Texture* RedPiece,
                                                                 SDL_Texture* GreenPiece, SDL_Texture* YellowPiece);
void display_walls(struct board* board, SDL_Renderer *renderer, SDL_Texture* BlueWall, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenWall, SDL_Texture* YellowWall);
void display_board(struct board* board, SDL_Renderer *renderer, SDL_Texture* BluePiece, SDL_Texture* BlueWall, 
                                                                SDL_Texture* RedPiece, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenPiece, SDL_Texture* GreenWall,
                                                                SDL_Texture* YellowPiece, SDL_Texture* YellowWall);


#endif /* ! GO_H */