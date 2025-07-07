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
                    printf("    ");
            }
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

bool move_piece(struct board* board, size_t pos[2], int direction, int color)
{
    if (pos[0] >= board->size || pos[1] >= board->size || board->data[pos[0]][pos[1]]->color != color)
    {
        printf("Choose a valid starting position\n");
        return false;
    }
    if (direction == 0) // Moving toward north
    {
        if (pos[0] == 0)
        {
            printf("Choose a valid goal position\n");
            return false;
        }
        if (board->data[pos[0]][pos[1]]->north)
        {
            printf("A wall prevents you from moving\n");
            return false;
        }
        if (board->data[pos[0]- 1][pos[1]]->occ)
        {
            printf("Choose an unoccupied goal position\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->color = 0;
        board->data[pos[0]][pos[1]]->occ = false;
        board->data[pos[0] - 1][pos[1]]->color = color;
        board->data[pos[0] - 1][pos[1]]->occ = true;
        return true;
    }

    if (direction == 1) // Moving toward east
    {
        if (pos[1] == board->size - 1)
        {
            printf("Choose a valid goal position\n");
            return false;
        }
        if (board->data[pos[0]][pos[1]]->east)
        {
            printf("A wall prevents you from moving\n");
            return false;
        }
        if (board->data[pos[0]][pos[1] + 1]->occ)
        {
            printf("Choose an unoccupied goal position\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->color = 0;
        board->data[pos[0]][pos[1]]->occ = false;
        board->data[pos[0]][pos[1] + 1]->color = color;
        board->data[pos[0]][pos[1] + 1]->occ = true;
        return true;
    }
    
    if (direction == 2) // Moving toward south
    {
        if (pos[0] == board->size - 1)
        {
            printf("Choose a valid goal position\n");
            return false;
        }
        if (board->data[pos[0]][pos[1]]->south)
        {
            printf("A wall prevents you from moving\n");
            return false;
        }
        if (board->data[pos[0] + 1][pos[1]]->occ)
        {
            printf("Choose an unoccupied goal position\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->color = 0;
        board->data[pos[0]][pos[1]]->occ = false;
        board->data[pos[0] + 1][pos[1]]->color = color;
        board->data[pos[0] + 1][pos[1]]->occ = true;
        return true;
    }

    if (direction == 3) // Moving toward west
    {
        if (pos[1] == 0)
        {
            printf("Choose a valid goal position\n");
            return false;
        }
        if (board->data[pos[0]][pos[1]]->west)
        {
            printf("A wall prevents you from moving\n");
            return false;
        }
        if (board->data[pos[0]][pos[1] - 1]->occ)
        {
            printf("Choose an unoccupied goal position\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->color = 0;
        board->data[pos[0]][pos[1]]->occ = false;
        board->data[pos[0]][pos[1] - 1]->color = color;
        board->data[pos[0]][pos[1] - 1]->occ = true;
        return true;
    }
    printf("Give a valid direction\n");
    return false;
}

/* III - Walls */

bool add_wall(struct board* board, size_t pos[2], int direction, int color)
{
    if (pos[0] >= board->size || pos[1] >= board->size || board->data[pos[0]][pos[1]]->color != color)
    {
        printf("Choose a valid square to set a wall\n");
        return false;
    }
    if (direction == 0) // Wall toward north
    {
        if (board->data[pos[0]][pos[1]]->north)
        {
            printf("There is already a wall there\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->north = true;
        adjust_walls(board);
        return true;
    }

    if (direction == 1) // Wall toward east
    {
        if (board->data[pos[0]][pos[1]]->east)
        {
            printf("There is already a wall there\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->east = true;
        adjust_walls(board);
        return true;
    }

    if (direction == 2) // Wall toward south
    {
        if (board->data[pos[0]][pos[1]]->south)
        {
            printf("There is already a wall there\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->south = true;
        adjust_walls(board);
        return true;
    }
    if (direction == 3) // Wall toward north
    {
        if (board->data[pos[0]][pos[1]]->west)
        {
            printf("There is already a wall there\n");
            return false;
        }
        board->data[pos[0]][pos[1]]->west = true;
        adjust_walls(board);
        return true;
    }
    printf("Give a valid direction\n");
    return false;
}

/* Test zone */
int main()
{
    struct board* board = init_board(4);
    init_walls(board);
    // size_t pos1[2] = {1,2};
    // size_t pos2[2] = {3,3};
    // add_piece(board, pos1, 1);
    // add_piece(board, pos1, 1);
    // add_piece(board, pos2, 2);
    for (size_t j = 0; j < board->size; j++)
    {
        size_t pos[2] = {j, j};
        // size_t pos2[2] = {2, j};
        add_piece(board, pos, 1);
        // add_piece(board, pos2, 2);
    }
    print_board(board);
    size_t pos[2] = {0,0};
    size_t pos2[2] = {0,1};
    move_piece(board, pos, 1, 1);
    add_wall(board, pos2, 3, 2);
    move_piece(board, pos2, 3, 2);
    print_board(board);
    return 0;
}