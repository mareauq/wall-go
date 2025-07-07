#ifndef GO_H
#define GO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structures */

struct square
{
    unsigned int pos[2];
    // Bools for walls
    bool north;
    bool east;
    bool south;
    bool west;
    bool occ; /* Is the frame occupied */
    unsigned int color; /* 0 if empty, 1 if black, 2 if white*/
};

struct board
{
    struct square*** data;
    size_t size;
};

/* Prototypes */

struct board* init_board(size_t size);
void print_board(struct board* board);
void init_walls(struct board* board);
void adjust_walls(struct board* board);
bool add_piece(struct board* board, unsigned int pos[2], unsigned int color);
bool move_piece(struct board* board, unsigned int pos[2], unsigned int next_pos[2], unsigned int color);
unsigned int dist(unsigned int pos1[2], unsigned int pos2[2]);



#endif /* ! GO_H */