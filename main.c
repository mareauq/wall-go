#include "go.h"

int main(int argc, char *argv[])
{

    /* Asking the amount of players and waiting for a valid answer */

    int num_players = 0;
    int pieces_available;
    int moves_available = 2;

    while (num_players < 2 || num_players > 4)
    {
        printf("Choose the amount of players (2-4) : \n");
        scanf("%d", &num_players);
    }

    if (num_players == 2)
        pieces_available = 6;
    else
        pieces_available = 2*num_players; 

    /* Declaring all needed variables */

    // Game management variables declaration and initialization

    struct board* board = init_board(7);
    init_walls(board);
    struct graph* graph_board = init_graph_board(board);
    update_adj_mat(board, graph_board->adj_mat);
    update_graph(graph_board);

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

    int statut = EXIT_FAILURE;

    int score[4] = {0, 0, 0, 0};
    int walls_count[4] = {20, 20, 20, 20};
    Sint32 square_mouse[2] = {-1, -1};
    Sint32 pos_square_selected[2] = {-1, -1};
    Sint32* nextpos = NULL;

    SDL_Rect rec = {0, 0, Window_width, Window_height};
    struct square* square_selected = NULL;

    enum Colors color_selected = EMPTY;
    enum Players playing_player = PLAYER_BLUE;
    enum Directions move_dir = -1;
    enum Directions wall_dir = -1;
    
    bool valid_pos = false;
    bool is_square_selected = false;
    bool good_color = false;
    bool square_reachable = false;
    bool game_over = false;
    
    /* Window and renderer initialization */
    
    if (!board)
    {
        printf("Board uninitialized");
        goto Quit;
    }
    if (!graph_board)
    {
        printf("Graph uninitialized");
        goto Quit;
    }

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
    
    for (enum Colors color = BLUE; color <= YELLOW; color++)
    {
        if (color == BLUE)
        {
            BluePiece = Load_picture("./assets/BluePiece.bmp", renderer);
            BlueWall = Load_picture("./assets/BlueWall.bmp", renderer);
            SDL_SetTextureBlendMode(BluePiece, SDL_BLENDMODE_BLEND);
            SDL_SetTextureBlendMode(BlueWall, SDL_BLENDMODE_BLEND);
            if(!BluePiece || !BlueWall)
            {
                goto Quit;
            }
        }
        if (color == RED)
        {
            RedPiece = Load_picture("./assets/RedPiece.bmp", renderer);
            RedWall = Load_picture("./assets/RedWall.bmp", renderer);
            SDL_SetTextureBlendMode(RedPiece, SDL_BLENDMODE_BLEND);
            SDL_SetTextureBlendMode(RedWall, SDL_BLENDMODE_BLEND);
            if(!RedPiece || !RedWall)
            {
                goto Quit;
            }
        }
        if (color == GREEN)
        {
            GreenPiece = Load_picture("./assets/GreenPiece.bmp", renderer);
            GreenWall = Load_picture("./assets/GreenWall.bmp", renderer);
            SDL_SetTextureBlendMode(GreenPiece, SDL_BLENDMODE_BLEND);
            SDL_SetTextureBlendMode(GreenWall, SDL_BLENDMODE_BLEND);
            if(!GreenPiece || !GreenWall)
            {
                goto Quit;
            }
        }
        if (color == YELLOW)
        {
            YellowPiece = Load_picture("./assets/YellowPiece.bmp", renderer);
            YellowWall = Load_picture("./assets/YellowWall.bmp", renderer);
            SDL_SetTextureBlendMode(YellowPiece, SDL_BLENDMODE_BLEND);
            SDL_SetTextureBlendMode(YellowWall, SDL_BLENDMODE_BLEND);
            if(!YellowPiece || !YellowWall)
            {
                goto Quit;
            }
        }
    }

    /* Displaying board */
   
    SDL_QueryTexture(text_board, NULL, NULL, &rec.w, &rec.h);
    SDL_RenderCopy(renderer, text_board, NULL, &rec);
    display_board(board, renderer, text_board, BluePiece, BlueWall, RedPiece, RedWall, GreenPiece, GreenWall, YellowPiece, YellowWall);
    SDL_RenderPresent(renderer);

    /* Event management */

    SDL_bool program_lauched = SDL_TRUE;

    while (program_lauched)
    {
        SDL_Event event;
        if (!game_over)
        {
            while (SDL_PollEvent(&event))
            {
                display_board(board, renderer, text_board, BluePiece, BlueWall, RedPiece, RedWall, GreenPiece, GreenWall, YellowPiece, YellowWall);

                if (!can_play(board, playing_player) && pieces_available == 0)
                        {
                            playing_player = (playing_player + 1)%num_players;
                        }

                switch (event.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        
                        in_square(event.motion.x, event.motion.y, square_mouse);
                        valid_pos = square_mouse[0] != -1 && square_mouse[1] != -1;
                        

                        if (valid_pos && pieces_available != 0 && !board->data[square_mouse[0]][square_mouse[1]]->occ) // Adding pieces
                        {
                            add_piece(board, square_mouse, playing_player + 1);
                            pieces_available--;
                            playing_player = (playing_player + 1)%num_players;
                        }
                        

                        if(valid_pos && pieces_available == 0)
                        {
                            
                            if (is_square_selected)
                            {   
                                square_reachable = is_square_reachable(board, pos_square_selected, square_mouse);
                                move_dir = move_direction(pos_square_selected, square_mouse);
                                if (move_dir != -1 && square_reachable && moves_available > 0)
                                {
                                    move_piece(board, pos_square_selected, move_dir, color_selected);
                                    nextpos = next_pos(pos_square_selected, move_dir);
                                    pos_square_selected[0] = nextpos[0];
                                    pos_square_selected[1] = nextpos[1];
                                    free(nextpos);
                                    square_selected = board->data[pos_square_selected[0]][pos_square_selected[1]];
                                    moves_available--;
                                }
                                else if (moves_available == 2)
                                {
                                    square_selected = NULL;
                                    color_selected = EMPTY;
                                    is_square_selected = false;
                                }
                            }

                            good_color = (playing_player == PLAYER_BLUE && board->data[square_mouse[0]][square_mouse[1]]->color == BLUE) || 
                                        (playing_player == PLAYER_RED && board->data[square_mouse[0]][square_mouse[1]]->color == RED) ||
                                        (playing_player == PLAYER_GREEN && board->data[square_mouse[0]][square_mouse[1]]->color == GREEN) ||
                                        (playing_player == PLAYER_YELLOW && board->data[square_mouse[0]][square_mouse[1]]->color == YELLOW);
                            
                            if (board->data[square_mouse[0]][square_mouse[1]]->occ && good_color && moves_available == 2) // Selection of a piece to move
                            {
                                pos_square_selected[0] = square_mouse[0];
                                pos_square_selected[1] = square_mouse[1];
                                square_selected = board->data[square_mouse[0]][square_mouse[1]];
                                color_selected = board->data[square_mouse[0]][square_mouse[1]]->color;
                                is_square_selected = true;
                            }
                            else if (moves_available == 2)
                            {
                                square_selected = NULL;
                                color_selected = EMPTY;
                                is_square_selected = false;
                            }

                        }
                        else if (wall_dir != -1 && is_square_selected && !square_selected->walls[wall_dir])
                        {
                            if (wall_dir == NORTH)
                                add_wall(board, pos_square_selected, NORTH, color_selected);
                            if (wall_dir == EAST)
                                add_wall(board, pos_square_selected, EAST, color_selected);
                            if (wall_dir == SOUTH)
                                add_wall(board, pos_square_selected, SOUTH, color_selected);
                            if (wall_dir == WEST)
                                add_wall(board, pos_square_selected, WEST, color_selected);
                            walls_count[color_selected - 1]--;
                            adjust_walls(board);
                            update_adj_mat(board, graph_board->adj_mat);
                            update_graph(graph_board);
                            square_selected = NULL;
                            color_selected = EMPTY;
                            is_square_selected = false;
                            playing_player = (playing_player + 1)%num_players;
                            moves_available = 2;
                            game_over = is_game_over(board, graph_board, score, walls_count, num_players);

                        }
                        else if (moves_available == 2)
                        {
                            square_selected = NULL;
                            color_selected = EMPTY;
                            is_square_selected = false;
                        }
                        SDL_RenderClear(renderer);
                        display_board(board, renderer, text_board, BluePiece, BlueWall, RedPiece, RedWall, GreenPiece, GreenWall, YellowPiece, YellowWall);

                        break;
                    
                    case SDL_MOUSEMOTION:
                        
                        in_square(event.motion.x, event.motion.y, square_mouse);
                        valid_pos = square_mouse[0] != -1 && square_mouse[1] != -1;
                        square_reachable = is_square_reachable(board, pos_square_selected, square_mouse);

                        // DISPLAY THE SHADOW OF NEXT MOVE / WALLS
                        if (valid_pos && pieces_available != 0 && !board->data[square_mouse[0]][square_mouse[1]]->occ)
                        {
                            SDL_RenderClear(renderer);
                            display_board(board, renderer, text_board, BluePiece, BlueWall, RedPiece, RedWall, GreenPiece, GreenWall, YellowPiece, YellowWall);
                            display_piece(renderer, square_mouse, playing_player + 1, BluePiece, RedPiece, GreenPiece, YellowPiece, 100);
                        }
                        else if (is_square_selected)
                        {
                            SDL_RenderClear(renderer);
                            display_board(board, renderer, text_board, BluePiece, BlueWall, RedPiece, RedWall, GreenPiece, GreenWall, YellowPiece, YellowWall);
                            wall_dir = on_wall(event.motion.x, event.motion.y, pos_square_selected);
                            if (valid_pos && square_reachable && moves_available > 0) // Mouse pointing a empty square to move a piece
                            {
                                display_piece(renderer, square_mouse, color_selected, BluePiece, RedPiece, GreenPiece, YellowPiece, 100);
                            }
                            else if (wall_dir != -1 && !square_selected->walls[wall_dir] && pieces_available == 0) // Mouse pointing a empty wall
                            {
                                display_wall(renderer, square_selected, color_selected, wall_dir, BlueWall, RedWall, GreenWall, YellowWall, 100);
                            }
                            else
                            {
                                SDL_RenderClear(renderer);
                                display_board(board, renderer, text_board, BluePiece, BlueWall, RedPiece, RedWall, GreenPiece, GreenWall, YellowPiece, YellowWall);
                            }
                        }

                        break;
                    
                    case SDL_KEYDOWN:
                            {

                                if (event.key.keysym.sym == SDLK_SPACE)
                                {
                                    update_adj_mat(board, graph_board->adj_mat);
                                    update_graph(graph_board);
                                    print_board(board);
                                    printf("BLUE : %d\n", score[PLAYER_BLUE]);
                                    printf("RED : %d\n", score[PLAYER_RED]);
                                    printf("GREEN : %d\n", score[PLAYER_GREEN]);
                                    printf("YELLOW : %d\n", score[PLAYER_YELLOW]);
                                }
                                break;
                            }

                    case SDL_QUIT:
                        program_lauched = SDL_FALSE;
                        break;
                
                    default:
                        break;
                }
            }
        }
        else
        {
            printf("BLUE : %d\n", score[PLAYER_BLUE]);
            printf("RED : %d\n", score[PLAYER_RED]);
            printf("GREEN : %d\n", score[PLAYER_GREEN]);
            printf("YELLOW : %d\n", score[PLAYER_YELLOW]);
            printf("Press Return for a new game or Escape to leave.\n");
            while(SDL_WaitEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_RETURN)
                        {
                            moves_available = 2;
                            reset_board(board);
                            game_over = false;
                            playing_player = PLAYER_BLUE;
                            num_players = 0;
                            while (num_players < 2 || num_players > 4)
                            {
                                printf("Choose the amount of players (2-4) : \n");
                                scanf("%d", &num_players);
                            }
                            
                            if (num_players == 2)
                                pieces_available = 6;
                            else
                                pieces_available = 2*num_players;
                        }
                        
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            program_lauched = SDL_FALSE;
                        }
                        break;
                    
                    case SDL_QUIT:
                        program_lauched = SDL_FALSE;
                        break;
                
                    default:
                        break;
                }

                if (!program_lauched || !game_over)
                    break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    statut = EXIT_SUCCESS;

Quit:
    //Freeing game variables 

    if (board)
        free_board(board);
    if (graph_board)
        free_graph(graph_board);
    
    //Freeing textures

    if (text_board)
        SDL_DestroyTexture(text_board);
    if (BluePiece)
        SDL_DestroyTexture(BluePiece);
    if (BlueWall)
        SDL_DestroyTexture(BlueWall);
    if (RedPiece)
        SDL_DestroyTexture(RedPiece);
    if (RedWall)
        SDL_DestroyTexture(RedWall);
    if (GreenPiece)
        SDL_DestroyTexture(GreenPiece);
    if (GreenWall)
        SDL_DestroyTexture(GreenWall);
    if (YellowPiece)
        SDL_DestroyTexture(YellowPiece);
    if (YellowWall)
        SDL_DestroyTexture(YellowWall);

    // Freeing renderer and window
    
    if(renderer)
        SDL_DestroyRenderer(renderer); // Destroy renderer and free associated textures (here all textures)
    if(window)
        SDL_DestroyWindow(window);
    
    SDL_Quit();
    return statut;
}