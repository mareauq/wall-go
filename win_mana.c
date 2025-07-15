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
            size_t pos[2] = { i , j };
            for (enum Directions dir = NORTH; dir <= WEST; dir++)
            {
                size_t* nextpos = next_pos(pos, dir);
                if (nextpos[0] < board->size && nextpos[1] < board->size && !(board->data[pos[0]][pos[1]]->walls[dir]))
                {
                    adj_mat[pos[0]*board->size + pos[1]][nextpos[0]*board->size + nextpos[1]] = true;
                }
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
    struct vertex** vertex_region = malloc(graph_board->size*sizeof(struct vertex*));
    if (!vertex_region)
        return NULL;
    for (size_t i = 0; i < graph_board->size; i++)
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
    for (size_t i = 0; i < graph_board->size; i++)
    {
        if (vertex1_region[i] == vertex2)
            return true;
    }
    free(vertex1_region);
    return false;
}