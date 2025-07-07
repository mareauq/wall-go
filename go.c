/*              GO BLOQUEUR              */

/*                HEADERS          */ 

#include "go.h"

/* Elementary functions */

// Initialization 

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
            board->data[i][j]->pos[0] = i;
            board->data[i][j]->pos[1] = j;
            board->data[i][j]->color = 0;
            board->data[i][j]->occ = false;
            board->data[i][j]->east = false;
            board->data[i][j]->west = false;
            board->data[i][j]->north = false;
            board->data[i][j]->south = false;
        }
    }
    return board;
}

void init_walls(struct board* board)
{
    for (size_t i = 0; i < board->size; i++)
    {
        board->data[i][0]->west = true;
        board->data[i][board->size-1]->east = true;
    }
    for (size_t j = 0; j < board->size; j++)
    {
        board->data[0][j]->north = true;
        board->data[board->size - 1][j]->south = true;
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
                if (board->data[i][j]->south)
                    board->data[i+1][j]->north = true;
            }
            if (i > 0)
            {
                if (board->data[i][j]->north)
                    board->data[i-1][j]->south = true;
            }
            if (j < board->size - 1)
            {
                if (board->data[i][j]->east)
                    board->data[i][j+1]->west = true;
            }
            if (j > 0)
            {
                if (board->data[i][j]->west)
                    board->data[i][j-1]->east = true;
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
            if (board->data[i][j]->east)
            {
                if (board->data[i][j]->occ)
                {
                    if (board->data[i][j]->color == 1)
                    {
                        printf(" B |");
                    }
                    if (board->data[i][j]->color == 2)
                    {
                        printf(" W |");
                    }
                }
                else
                    printf("   |");
            }
            else
            {
                if (board->data[i][j]->occ)
                {
                    if (board->data[i][j]->color == 1)
                    {
                        printf(" B  ");
                    }
                    if (board->data[i][j]->color == 2)
                    {
                        printf(" W  ");
                    }
                }
                else
                    printf("     ");
            }
            // if (board->data[i][j]->east && !(board->data[i][j]->occ))
            // {
            //     printf("    |");
            // }
            // else if (board->data[i][j]->east && board->data[i][j]->occ)
            // {
            //     if (board->data[i][j]->color = 1)
            //     {

            //     }
            // }
            // else
            //     printf("     ");
        }
        printf("\n");
        printf("    ");
        for (size_t j = 0; j < board->size; j++)
        {
            if (board->data[i][j]->south && i < board->size)
                printf("----");
            else
                printf("     ");
        }
        printf("\n");
    }
}

bool add_piece(struct board* board, unsigned int pos[2], unsigned int color)
{
    bool setup = false;
}

int main()
{
    struct board* board = init_board(4);
    init_walls(board);
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            board->data[i][j]->east = true;
            board->data[i][j]->north = true;
            board->data[i][j]->west = true;
            board->data[i][j]->south = true;
            board->data[(2*i + 3)%board->size][(2*j)%board->size]->occ = true;
            board->data[(2*i + 3)%board->size][(2*j)%board->size]->color = 2;
        }
    printf("%d\n", board->data[i][board->size - 1]->east);
    }
    print_board(board);
    return 0;
}