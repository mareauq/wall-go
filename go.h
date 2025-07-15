#ifndef GO_H
#define GO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Directions : 0 -> north, 1 -> east, 2 -> south, 3 -> west

enum Directions {
  NORTH,
  EAST,
  SOUTH,
  WEST,
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
    int color; /* 0 if empty, 1 if black, 2 if white*/
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


#endif /* ! GO_H */