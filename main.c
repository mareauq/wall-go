#include "go.h"

void print_mat(size_t* mat_size, bool** mat)
{
    for (size_t i = 0; i < mat_size[0]; i++)
    {
        putchar('|');
        putchar(' ');
        for (size_t j = 0; j < mat_size[1]; j++)
        {
            printf("%d  ", mat[i][j]);
        }
        putchar('|');
        putchar('\n');
    }
}

int main()
{
    struct board* board = init_board(3);
    init_walls(board);
    size_t pos[2] = {0,1};
    add_piece(board, pos, 1);
    add_wall(board, pos, SOUTH, 1);
    add_wall(board, pos, EAST, 1);
    size_t pos2[2] = {1,0};
    add_piece(board, pos2, 1);
    add_wall(board, pos2, SOUTH, 1);
    add_wall(board, pos2, EAST, 1);
    adjust_walls(board);
    print_board(board);
    size_t size_mat[2] = {board->size*board->size, board->size*board->size};
    struct graph* graph_board = init_graph_board(board);
    update_adj_mat(board, graph_board->adj_mat);
    update_graph(graph_board);
    print_mat(size_mat, graph_board->adj_mat);
    // for (size_t i = 0; i < 4; i++)
    // {
    //     printf("%p\n", graph_board->vertices[i]);
    // }
    // printf("Maintenant, les adresses des voisins du premier vertex : %p\n", graph_board->vertices[0]);
    // coloring_graph(graph_board, graph_board->vertices[0]);
    // for (size_t i = 0; i < 4; i++)
    // {
    //     printf("%d\n", graph_board->vertices[i]->seen);
    // }
    printf("%lu\n", region_size(graph_board, graph_board->vertices[2]));
    bool boubou = same_region(graph_board, graph_board->vertices[1], graph_board->vertices[6]);
    printf("Same region : %d\n", boubou);
    return 0;
}