/*              Wall Go             */

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
            {
                free(board->data[k]);
            }
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
            board->data[i][j]->pos[0] = X_GAP + j * SQUARE_SIZE;
            board->data[i][j]->pos[1] = Y_GAP + i * SQUARE_SIZE;
            board->data[i][j]->color = 0;
            board->data[i][j]->occ = false;
            board->data[i][j]->walls[NORTH] = false;
            board->data[i][j]->walls[EAST] = false;
            board->data[i][j]->walls[SOUTH] = false;
            board->data[i][j]->walls[WEST] = false;
            board->data[i][j]->walls_color[NORTH] = BLUE; // Default color, must be modified if wall is displayed
            board->data[i][j]->walls_color[EAST] = BLUE; // Default color, must be modified if wall is displayed
            board->data[i][j]->walls_color[SOUTH] = BLUE; // Default color, must be modified if wall is displayed
            board->data[i][j]->walls_color[WEST] = BLUE; // Default color, must be modified if wall is displayed
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

void reset_board(struct board* board)
{
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            board->data[i][j]->color = 0;
            board->data[i][j]->occ = false;
            board->data[i][j]->walls[NORTH] = false;
            board->data[i][j]->walls[EAST] = false;
            board->data[i][j]->walls[SOUTH] = false;
            board->data[i][j]->walls[WEST] = false;
            board->data[i][j]->walls_color[NORTH] = BLUE; 
            board->data[i][j]->walls_color[EAST] = BLUE; 
            board->data[i][j]->walls_color[SOUTH] = BLUE; 
            board->data[i][j]->walls_color[WEST] = BLUE;
        }
    }
    init_walls(board);
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
                {
                    board->data[i+1][j]->walls[NORTH] = true;
                    board->data[i+1][j]->walls_color[NORTH] = board->data[i][j]->walls_color[SOUTH];
                }
            }
            if (i > 0)
            {
                if (board->data[i][j]->walls[NORTH])
                {
                    board->data[i-1][j]->walls[SOUTH] = true;
                    board->data[i-1][j]->walls_color[SOUTH] = board->data[i][j]->walls_color[NORTH];
                }

            }
            if (j < board->size - 1)
            {
                if (board->data[i][j]->walls[EAST])
                {
                    board->data[i][j+1]->walls[WEST] = true;
                    board->data[i][j+1]->walls_color[WEST] = board->data[i][j]->walls_color[EAST];
                }

            }
            if (j > 0)
            {
                if (board->data[i][j]->walls[WEST])
                {
                    board->data[i][j-1]->walls[EAST] = true;
                    board->data[i][j-1]->walls_color[EAST] = board->data[i][j]->walls_color[WEST];
                }

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

void free_board(struct board* board)
{
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            free(board->data[i][j]);
        }
        free(board->data[i]);
    }
    free(board->data);
    free(board);
}

/* II - Pieces management */

bool add_piece(struct board* board, Sint32 pos[2], int color)
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

Sint32 dist(Sint32 pos1[2], Sint32 pos2[2]) // Obsolete
{
    Sint32 dist_x;
    Sint32 dist_y;
    if (pos1[0] < pos2[0])
        dist_x = pos2[0] - pos1[0];
    else
        dist_x = pos1[0] - pos2[0];
    if (pos1[1] < pos2[1])
        dist_y = pos2[1] - pos1[1];
    else
        dist_y = pos1[1] - pos2[1];
    return dist_x + dist_y;
}

bool is_square_reachable(struct board* board, Sint32 pos1[2], Sint32 pos2[2])
{
    if (pos2[0] == pos1[0] - 1 && pos2[1] == pos1[1] )
    {
        return !board->data[pos1[0]][pos1[1]]->walls[NORTH] && !board->data[pos2[0]][pos2[1]]->occ;
    }
    if (pos2[0] == pos1[0] && pos2[1] == pos1[1] + 1)
    {
        return !board->data[pos1[0]][pos1[1]]->walls[EAST] && !board->data[pos2[0]][pos2[1]]->occ;
    }
    if (pos2[0] == pos1[0] + 1 && pos2[1] == pos1[1])
    {
        return !board->data[pos1[0]][pos1[1]]->walls[SOUTH] && !board->data[pos2[0]][pos2[1]]->occ;
    }
    if (pos2[0] == pos1[0] && pos2[1] == pos1[1] - 1)
    {
        return !board->data[pos1[0]][pos1[1]]->walls[WEST] && !board->data[pos2[0]][pos2[1]]->occ;
    }
    return false;
}

int move_direction(Sint32 pos1[2], Sint32 pos2[2])
{
    if (pos2[0] == pos1[0] - 1 && pos2[1] == pos1[1] )
    {
        return NORTH;
    }
    if (pos2[0] == pos1[0] && pos2[1] == pos1[1] + 1)
    {
        return EAST;
    }
    if (pos2[0] == pos1[0] + 1 && pos2[1] == pos1[1])
    {
        return SOUTH;
    }
    if (pos2[0] == pos1[0] && pos2[1] == pos1[1] - 1)
    {
        return WEST;
    }
    return -1;
}

Sint32* next_pos(Sint32 pos[2], enum Directions direction)
{
    Sint32* nextpos = malloc(2*sizeof(Sint32));
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

bool move_piece(struct board* board, Sint32 pos[2], enum Directions direction, int color)
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
    Sint32* nextpos = next_pos(pos, direction);
    if (board->data[nextpos[0]][nextpos[1]]->occ)
    {
        printf("Choose an unoccupied goal position\n");
        return false;
    }
    board->data[pos[0]][pos[1]]->color = 0;
    board->data[pos[0]][pos[1]]->occ = false;
    board->data[nextpos[0]][nextpos[1]]->color = color;
    board->data[nextpos[0]][nextpos[1]]->occ = true;
    free(nextpos);
    return true;
}

/* III - Walls */

bool add_wall(struct board* board, Sint32 pos[2], enum Directions direction, int color)
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
    board->data[pos[0]][pos[1]]->walls_color[direction] = color;
    adjust_walls(board);
    return true;
}
