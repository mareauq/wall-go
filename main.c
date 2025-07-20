#include "go.h"


int main(int argc, char *argv[])
{
    
    /* Asking the amount of players and waiting for a valid answer */

    int num_players = 0;
    while (num_players < 2 || num_players > 4)
    {
        printf("Choose the amount of players (2-4) : \n");
        scanf("%d", &num_players);
    }


    /* Declaring all needed variables */

    // Game management variables initialization

    struct board* board = init_board(7);
    init_walls(board);
    struct graph* graph_board = init_graph_board(board);


    // Window and renderer

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Textures
    
    SDL_Texture *text_board = NULL;
    SDL_Texture *BluePiece = NULL;
    SDL_Texture *BlueWall = NULL;
    SDL_Texture *RedPiece = NULL;
    SDL_Texture *RedWall = NULL;
    SDL_Texture *GreenPiece = NULL;
    SDL_Texture *GreenWall = NULL;
    SDL_Texture *YellowPiece = NULL;
    SDL_Texture *YellowWall = NULL;

    // Others
    size_t* square_mouse = NULL;
    int statut = EXIT_FAILURE;
    //SDL_Rect rect = {100, 100, 100, 100}; 
    SDL_Rect rec = {0, 0, Window_width, Window_height};
    SDL_Rect pos_piece = {65, 65, 0, 0};
    SDL_Rect pos2_piece = {165, 165, 0, 0};

    
    /* Window and renderer initialization */
    
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    if(0 != SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_SHOWN, &window, &renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        goto Quit;
    }

    /* Loading all textures needed */
    
    text_board = Load_picture("./assets/Background7x7.bmp", renderer);
    if(!text_board)
    {
        goto Quit;
    }
    
    for (enum Colors color = BLUE; color <= num_players; color++)
    {
        if (color == BLUE)
        {
            BluePiece = Load_picture("./assets/BluePiece.bmp", renderer);
            BlueWall = Load_picture("./assets/BlueWall.bmp", renderer);
            if(!BluePiece || !BlueWall)
            {
                goto Quit;
            }
        }
        if (color == RED)
        {
            RedPiece = Load_picture("./assets/RedPiece.bmp", renderer);
            RedWall = Load_picture("./assets/RedWall.bmp", renderer);
            if(!RedPiece || !RedWall)
            {
                goto Quit;
            }
        }
        if (color == GREEN)
        {
            GreenPiece = Load_picture("./assets/GreenPiece.bmp", renderer);
            GreenWall = Load_picture("./assets/GreenWall.bmp", renderer);
            if(!GreenPiece || !GreenWall)
            {
                goto Quit;
            }
        }
        if (color == YELLOW)
        {
            YellowPiece = Load_picture("./assets/YellowPiece.bmp", renderer);
            YellowWall = Load_picture("./assets/YellowWall.bmp", renderer);
            if(!YellowPiece || !YellowWall)
            {
                goto Quit;
            }
        }
    }
    


    size_t pos[2] = {0,1};
    add_piece(board, pos, RED);
    add_wall(board, pos, SOUTH, RED);
    add_wall(board, pos, EAST, RED);
    size_t pos2[2] = {1,0};
    add_piece(board, pos2, GREEN);
    add_wall(board, pos2, SOUTH, GREEN);
    add_wall(board, pos2, EAST, GREEN);
    adjust_walls(board);
    // print_board(board);
    // size_t size_mat[2] = {board->size*board->size, board->size*board->size};
    // struct graph* graph_board = init_graph_board(board);
    // update_adj_mat(board, graph_board->adj_mat);
    // update_graph(graph_board);
    // print_mat(size_mat, graph_board->adj_mat);



    /* Création fenêtres et rendu */

    
    
    SDL_QueryTexture(text_board, NULL, NULL, &rec.w, &rec.h);
    SDL_RenderCopy(renderer, text_board, NULL, &rec);
    // SDL_QueryTexture(Piece, NULL, NULL, &pos_piece.w, &pos_piece.h);
    // SDL_RenderCopy(renderer, Piece, NULL, &pos_piece);
    
    display_pieces(board, renderer, BluePiece, RedPiece, GreenPiece, YellowPiece);
    display_walls(board, renderer, BlueWall, RedWall, GreenWall, YellowWall);

    // SDL_SetTextureBlendMode(Piece, SDL_BLENDMODE_BLEND);
    // SDL_SetTextureAlphaMod(Piece, 130);
    // SDL_QueryTexture(Piece, NULL, NULL, &pos2_piece.w, &pos2_piece.h);
    // SDL_RenderCopy(renderer, Piece, NULL, &pos2_piece);
    //SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    statut = EXIT_SUCCESS;

    /* Event management */

    SDL_bool program_lauched = SDL_TRUE;

    while (program_lauched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    square_mouse = in_square(event.motion.x, event.motion.y);
                    if (square_mouse)
                        printf("On clique en x = %lu et y = %lu\n", square_mouse[0], square_mouse[1]);
                    else
                        printf("Clique sur une case frérot\n");
                    break;
            
                case SDL_QUIT:
                    program_lauched = SDL_FALSE;
                    break;
            
                default:
                    break;
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(FPS_DELAY);
    }

Quit:
    if(renderer)
        SDL_DestroyRenderer(renderer); // Destroy rendere and free associated textures (here all textures)
    if(window)
        SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
    SDL_Quit();
    return statut;
}