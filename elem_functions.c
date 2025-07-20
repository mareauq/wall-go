/*              GO BLOQUEUR              */

/*                HEADERS          */ 

#include "go.h"

/* I - Elementary functions */

struct board* init_board(size_t size)
{
    struct board* board = malloc(sizeof(struct board)); // Memory allocations
    struct square*** mat_squares = calloc(size, sizeof(struct square**));
    if (!board || !mat_squares) // Errors management
    {
        free(board);
        free(mat_squares);
        return NULL;
    }
    board->data = mat_squares;
    board->size = size;
    for (size_t i = 0; i < size; i++) // Values initialization
    {
        struct square** row_squares = calloc(size,sizeof(struct square*));
        if (!row_squares) // Errors management
        {
            for (size_t k = 0; k < i; k++)
                free(board->data[i]);
            free(board->data);
            free(board);
            return NULL;
        }
        board->data[i] = row_squares;
        for (size_t j = 0; j < size; j++)
        {
            struct square* square = malloc(sizeof(struct square));
            if (!square) // Errors management
            {
                for (size_t l = 0; l < size; l++)
                {
                    for (size_t m = 0; m < size; m++)
                    {
                        if (board->data[l][m] != NULL) 
                            free(board->data[l][m]);
                    }
                    free(board->data[l]);
                }
                free(board->data);
                free(board);
                return NULL;
            }
            board->data[i][j] = square;
            board->data[i][j]->pos[0] = X_GAP + i * SQUARE_SIZE;
            board->data[i][j]->pos[1] = Y_GAP + j * SQUARE_SIZE;
            board->data[i][j]->color = 0;
            board->data[i][j]->occ = false;
            board->data[i][j]->walls[NORTH] = false;
            board->data[i][j]->walls[EAST] = false;
            board->data[i][j]->walls[SOUTH] = false;
            board->data[i][j]->walls[WEST] = false;
        }
    }
    return board;
}

void init_walls(struct board* board)
{
    for (size_t i = 0; i < board->size; i++)
    {
        board->data[i][0]->walls[WEST] = true;
        board->data[i][board->size-1]->walls[EAST] = true;
    }
    for (size_t j = 0; j < board->size; j++)
    {
        board->data[0][j]->walls[NORTH] = true;
        board->data[board->size - 1][j]->walls[SOUTH] = true;
    }
}

void adjust_walls(struct board* board)
{
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            if (i < board->size - 1)
            {
                if (board->data[i][j]->walls[SOUTH])
                    board->data[i+1][j]->walls[NORTH] = true;
            }
            if (i > 0)
            {
                if (board->data[i][j]->walls[NORTH])
                    board->data[i-1][j]->walls[SOUTH] = true;
            }
            if (j < board->size - 1)
            {
                if (board->data[i][j]->walls[EAST])
                    board->data[i][j+1]->walls[WEST] = true;
            }
            if (j > 0)
            {
                if (board->data[i][j]->walls[WEST])
                    board->data[i][j-1]->walls[EAST] = true;
            }
        }
    }
}

void print_board(struct board* board)
{
    printf("    ");
    for (size_t j = 0; j < board->size; j++)
            printf(" %lu  ", j);
    printf("\n");
    printf("    ");
    for (size_t j = 0; j < board->size; j++)
            printf("----");
    printf("\n");
    for (size_t i = 0; i < board->size; i++)
    {
        printf("%lu  |", i);
        for (size_t j = 0; j < board->size; j++)
        {
            if (board->data[i][j]->walls[EAST])
            {
                if (board->data[i][j]->occ)
                {
                    if (board->data[i][j]->color == BLUE)
                    {
                        printf(" B |");
                    }
                    if (board->data[i][j]->color == RED)
                    {
                        printf(" R |");
                    }
                    if (board->data[i][j]->color == GREEN)
                    {
                        printf(" G |");
                    }
                    if (board->data[i][j]->color == YELLOW)
                    {
                        printf(" Y |");
                    }
                }
                else
                    printf("   |");
            }
            else
            {
                if (board->data[i][j]->occ)
                {
                    if (board->data[i][j]->color == BLUE)
                    {
                        printf(" B  ");
                    }
                    if (board->data[i][j]->color == RED)
                    {
                        printf(" R  ");
                    }
                    if (board->data[i][j]->color == GREEN)
                    {
                        printf(" G  ");
                    }
                    if (board->data[i][j]->color == YELLOW)
                    {
                        printf(" Y  ");
                    }
                }
                else
                    printf("    ");
            }
        }
        printf("\n");
        printf("    ");
        for (size_t j = 0; j < board->size; j++)
        {
            if (board->data[i][j]->walls[SOUTH] && i < board->size)
                printf("----");
            else
                printf("    ");
        }
        printf("\n");
    }
}

/* II - Pieces management */

bool add_piece(struct board* board, size_t pos[2], int color)
{
    if (pos[0] >= board->size ||pos[1] >= board->size)
    {
        printf("Invalid position\n");
        return false;
    }
    if (board->data[pos[0]][pos[1]]->occ)
    {
        printf("Position unavailable\n");
        return false;
    }
    board->data[pos[0]][pos[1]]->occ = true;
    board->data[pos[0]][pos[1]]->color = color;
    return true;
}

// size_t dist(size_t pos1[2], size_t pos2[2]) // Obsolete
// {
//     size_t dist_x;
//     size_t dist_y;
//     if (pos1[0] < pos2[0])
//         dist_x = pos2[0] - pos1[0];
//     else
//         dist_x = pos1[0] - pos2[0];
//     if (pos1[1] < pos2[1])
//         dist_y = pos2[1] - pos1[1];
//     else
//         dist_y = pos1[1] - pos2[1];
//     return dist_x + dist_y;
// }

size_t* next_pos(size_t pos[2], enum Directions direction)
{
    size_t* nextpos = malloc(2*sizeof(size_t));
    if (!nextpos)
        return NULL;
    if (direction == NORTH)
    {
        nextpos[0] = pos[0] - 1;
        nextpos[1] = pos[1];
    }
    if (direction == EAST)
    {
        nextpos[0] = pos[0];
        nextpos[1] = pos[1] + 1;
    }
    if (direction == SOUTH)
    {
        nextpos[0] = pos[0] + 1;
        nextpos[1] = pos[1];
    }
    if (direction == WEST)
    {
        nextpos[0] = pos[0];
        nextpos[1] = pos[1] - 1;
    }
    return nextpos;
}

bool move_piece(struct board* board, size_t pos[2], enum Directions direction, int color)
{
    if (pos[0] >= board->size || pos[1] >= board->size || board->data[pos[0]][pos[1]]->color != color)
    {
        printf("Choose a valid starting position\n");
        return false;
    }
    
    if (board->data[pos[0]][pos[1]]->walls[direction])
    {
        printf("A wall prevents you from moving\n");
        return false;
    }
    size_t* nextpos = next_pos(pos, direction);
    if (board->data[nextpos[0]][nextpos[1]]->occ)
    {
        printf("Choose an unoccupied goal position\n");
        return false;
    }
    board->data[pos[0]][pos[1]]->color = 0;
    board->data[pos[0]][pos[1]]->occ = false;
    board->data[nextpos[0]][nextpos[1]]->color = color;
    board->data[nextpos[0]][nextpos[1]]->occ = true;
    //free(nextpos); ????????
    return true;
}

/* III - Walls */

bool add_wall(struct board* board, size_t pos[2], enum Directions direction, int color)
{
    if (pos[0] >= board->size || pos[1] >= board->size || board->data[pos[0]][pos[1]]->color != color)
    {
        printf("Choose a valid square to set a wall\n");
        return false;
    }

    if (board->data[pos[0]][pos[1]]->walls[direction])
    {
        printf("There is already a wall there\n");
        return false;
    }
    board->data[pos[0]][pos[1]]->walls[direction] = true;
    adjust_walls(board);
    return true;
}
