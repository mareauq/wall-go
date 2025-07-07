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
    size_t pos[2];
    // Bools for walls
    bool north;
    bool east;
    bool south;
    bool west;
    bool occ; /* Is the frame occupied */
    int color; /* 0 if empty, 1 if black, 2 if white*/
};

struct board
{
    struct square*** data;
    size_t size;
};

// Directions : 0 -> north, 1 -> east, 2 -> south, 3 -> west

/* Prototypes */

struct board* init_board(size_t size);
void print_board(struct board* board);
void init_walls(struct board* board);
void adjust_walls(struct board* board);
bool add_piece(struct board* board, size_t pos[2], int color);
bool move_piece(struct board* board, size_t pos[2], int direction, int color);
//size_t dist(size_t pos1[2], size_t pos2[2]); // Obsolete



#endif /* ! GO_H */