/*              GO BLOQUEUR              */

/*                HEADERS          */ 

#include "go.h"

/* I - Graph initialization */

bool** adj_mat_init(struct board* board)
{
    size_t size = board->size * board->size;
    bool** adj_mat = malloc(size * sizeof(size_t*));
    if (!adj_mat)
        return NULL;
    for (size_t i = 0; i < size; i++)
    {
        bool* adj_row = calloc(size, sizeof(size_t));
        if (!adj_row)
        {
            for (size_t j = 0; j < i; j++)
            {
                free(adj_mat[j]);
            }
            free(adj_mat);
            return NULL;
        }
        adj_mat[i] = adj_row;
    }
    return adj_mat;
}

void update_adj_mat(struct board* board, bool** adj_mat)
{
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            Sint32 pos[2] = { i, j};
            for (enum Directions dir = NORTH; dir <= WEST; dir++)
            {
                Sint32* nextpos = next_pos(pos, dir);
                if (nextpos[0] < board->size && nextpos[1] < board->size)
                {
                    if (!(board->data[pos[0]][pos[1]]->walls[dir]))
                        adj_mat[pos[0]*board->size + pos[1]][nextpos[0]*board->size + nextpos[1]] = true;
                    else
                        adj_mat[pos[0]*board->size + pos[1]][nextpos[0]*board->size + nextpos[1]] = false;
                }
                free(nextpos);
            }
        }
    }
}

struct graph* init_graph_board(struct board* board)
{
    struct graph* graph_board = malloc(sizeof(struct graph));
    struct vertex** vertices = malloc(board->size * board->size * sizeof(struct vertex*));
    bool** adj_mat = adj_mat_init(board);
    if (!graph_board || !vertices || !adj_mat)
    {
        free(graph_board);
        free(vertices);
        return NULL;
    }
    
    for (size_t i = 0; i < board->size * board->size; i++)
    {
        struct vertex* vertex = malloc(sizeof(struct vertex));
        if (!vertex)
        {
            for (size_t j = 0; j < i; j++)
            {
                free(vertices[j]);
            }
            free(vertices);
            free(graph_board);
            free(adj_mat);
            return NULL;
        }
        vertices[i] = vertex;
        vertices[i]->neighbours[0] = NULL;
        vertices[i]->neighbours[1] = NULL;
        vertices[i]->neighbours[2] = NULL;
        vertices[i]->neighbours[3] = NULL;
        vertices[i]->seen = false;
    }
    graph_board->size = board->size * board->size;
    graph_board->vertices = vertices;
    graph_board->adj_mat = adj_mat;
    return(graph_board);
}

void update_graph(struct graph* graph_board)
{
    for (size_t i = 0; i < graph_board->size; i++)
    {
        for (size_t m = 0; m < 4; m++)
        {
            graph_board->vertices[i]->neighbours[m] = NULL;
        }
        size_t k = 0;
        for (size_t j = 0; j < graph_board->size; j++)
        {
            if (k > 3)
                break;
            if (graph_board->adj_mat[i][j] == 1)
            {
                graph_board->vertices[i]->neighbours[k] = graph_board->vertices[j];
                k++;
            }
        }
    }
}

void coloring_graph(struct graph* graph_board, struct vertex* vertex)
{
    vertex->seen = true;
    for (size_t i = 0; i < 4; i++)
    {
        if (vertex->neighbours[i] && !vertex->neighbours[i]->seen)
        {
            coloring_graph(graph_board, vertex->neighbours[i]);
        }
    }
}

struct vertex** vertex_region(struct graph* graph_board, struct vertex* vertex)
{
    struct vertex** vertex_region = malloc((graph_board->size + 1)*sizeof(struct vertex*));
    if (!vertex_region)
        return NULL;
    for (size_t i = 0; i < graph_board->size + 1; i++)
        vertex_region[i] = NULL;
    coloring_graph(graph_board, vertex);
    size_t k = 0;
    for (size_t i = 0; i < graph_board->size; i++)
    {
        if (graph_board->vertices[i]->seen)
        {
            graph_board->vertices[i]->seen = false;
            vertex_region[k] = graph_board->vertices[i];
            k++;
        }
    }
    return vertex_region;
}

size_t region_size(struct graph* graph_board, struct vertex* vertex)
{
    struct vertex** vert_region = vertex_region(graph_board, vertex);
    size_t k = 0;
    while (vert_region[k])
        k++;
    free(vert_region);
    return k;
}

bool same_region(struct graph* graph_board, struct vertex* vertex1, struct vertex* vertex2)
{
    struct vertex** vertex1_region = vertex_region(graph_board, vertex1);
    bool same_region = false;
    for (size_t i = 0; i < graph_board->size; i++)
    {
        if (vertex1_region[i] == vertex2)
        {
            same_region = true;
            break;
        }
    }
    free(vertex1_region);
    return same_region;
}

void free_mat(bool** mat, size_t mat_size)
{
    for (size_t i = 0; i < mat_size; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void free_graph(struct graph* graph_board)
{
    for (size_t i = 0; i < graph_board->size; i++)
    {
        free(graph_board->vertices[i]);
    }
    free_mat(graph_board->adj_mat, graph_board->size);
    free(graph_board);
}

bool can_move(struct board* board, Sint32 pos[2])
{
    Sint32* nextpos = NULL;
    for (enum Directions dir = NORTH; dir <= WEST; dir++)
    {
        nextpos = next_pos(pos, dir);
        if (!board->data[pos[0]][pos[1]]->walls[dir] && !board->data[nextpos[0]][nextpos[1]]->occ)
        {
            free(nextpos);
            return true;
        }
        free(nextpos);
    }
    return false;
}

bool can_play(struct board* board, enum Players player)
{
    Sint32** player_pieces_pos = malloc(3*sizeof(Sint32*));
    size_t k = 0;
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j  < board->size; j++)
        {
            if (board->data[i][j]->color == player + 1)
            {
                player_pieces_pos[k] = malloc(2*sizeof(Sint32));
                player_pieces_pos[k][0] = i;
                player_pieces_pos[k][1] = j;
                k++;
            }
        }
    }

    bool can_play = false;

    for (size_t i = 0; i < k; i++)
    {
        if (can_move(board, player_pieces_pos[i]))
            can_play = true;
        for (enum Directions dir = NORTH; dir <= WEST; dir++)
        {
            if (!board->data[player_pieces_pos[i][0]][player_pieces_pos[i][1]]->walls[dir])
                can_play= true;
        }
        free(player_pieces_pos[i]);
    }

    free(player_pieces_pos);
    return can_play;
}

bool is_game_over(struct board* board, struct graph* graph_board, int score[4], int walls_count[4], int num_players)
{
    
    struct vertex** blue_pieces = malloc(3*sizeof(struct vertex*));
    struct vertex** red_pieces = malloc(3*sizeof(struct vertex*));
    struct vertex** green_pieces = malloc(3*sizeof(struct vertex*));
    struct vertex** yellow_pieces = malloc(3*sizeof(struct vertex*));

    score[PLAYER_BLUE] = 0;
    score[PLAYER_RED] = 0;
    score[PLAYER_GREEN] = 0;
    score[PLAYER_YELLOW] = 0;

    size_t size_blue = 0;
    size_t size_red = 0;
    size_t size_green = 0;
    size_t size_yellow = 0;

    bool game_over = true;
    bool new_region;

    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            if (board->data[i][j]->color == BLUE)
            {
                blue_pieces[size_blue] = graph_board->vertices[i*board->size + j];
                new_region = true;
                for (size_t k = 0; k < size_blue; k++)
                {
                    if (same_region(graph_board, blue_pieces[k], blue_pieces[size_blue]))
                        new_region = false;
                }
                if (new_region || size_blue == 0)
                    score[PLAYER_BLUE] += region_size(graph_board, blue_pieces[size_blue]);
                size_blue++;

            }
            if (board->data[i][j]->color == RED)
            {
                red_pieces[size_red] = graph_board->vertices[i*board->size + j];
                new_region = true;
                for (size_t k = 0; k < size_red; k++)
                {
                    if (same_region(graph_board, red_pieces[k], red_pieces[size_red]))
                        new_region = false;
                }
                if (new_region || size_red == 0)
                    score[PLAYER_RED] += region_size(graph_board, red_pieces[size_red]);
                size_red++;
            }
            if (board->data[i][j]->color == GREEN)
            {
                green_pieces[size_green] = graph_board->vertices[i*board->size + j];
                new_region = true;
                for (size_t k = 0; k < size_green; k++)
                {
                    if (same_region(graph_board, green_pieces[k], green_pieces[size_green]))
                        new_region = false;
                }
                if (new_region || size_green == 0)
                    score[PLAYER_GREEN] += region_size(graph_board, green_pieces[size_green]);
                size_green++;
            }
            if (board->data[i][j]->color == YELLOW)
            {
                yellow_pieces[size_yellow] = graph_board->vertices[i*board->size + j];
                new_region = true;
                for (size_t k = 0; k < size_yellow; k++)
                {
                    if (same_region(graph_board, yellow_pieces[k], yellow_pieces[size_yellow]))
                        new_region = false;
                }
                if (new_region || size_yellow == 0)
                    score[PLAYER_YELLOW] += region_size(graph_board, yellow_pieces[size_yellow]);
                size_yellow++;
            }
        }
    }


    for (size_t i = 0; i < size_blue; i++)
    {
        for(size_t j = 0; j < size_red; j++)
        {
            if (same_region(graph_board, blue_pieces[i], red_pieces[j]))
                game_over = false;
        }
        for(size_t j = 0; j < size_green; j++)
        {
            if (same_region(graph_board, blue_pieces[i], green_pieces[j]))
                game_over = false;
        }
        for(size_t j = 0; j < size_yellow; j++)
        {
            if (same_region(graph_board, blue_pieces[i], yellow_pieces[j]))
                game_over = false;
        }
    }
    for (size_t i = 0; i < size_red; i++)
    {
        for(size_t j = 0; j < size_green; j++)
        {
            if (same_region(graph_board, red_pieces[i], green_pieces[j]))
                game_over = false;
        }
        for(size_t j = 0; j < size_yellow; j++)
        {
            if (same_region(graph_board, red_pieces[i], yellow_pieces[j]))
                game_over = false;
        }
    }
    for (size_t i = 0; i < size_green; i++)
    {
        for(size_t j = 0; j < size_yellow; j++)
        {
            if (same_region(graph_board, green_pieces[i], yellow_pieces[j]))
                game_over = false;
        }
    }

    for (enum Players player = PLAYER_BLUE; player < num_players; player++)
    {
        if (walls_count[player] != 0 && can_play(board, player))
        {
            game_over = false;
            break;
        }
    }

    free(blue_pieces);
    free(red_pieces);
    free(green_pieces);
    free(yellow_pieces);

    return game_over;
}