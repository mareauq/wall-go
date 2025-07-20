#include "go.h"

SDL_Texture* Load_picture(char* file, SDL_Renderer* renderer)
{
    SDL_Texture* texture = NULL;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                SDL_TEXTUREACCESS_TARGET, 200, 200);
    if(!texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        return NULL;
    }
    SDL_Surface *tmp = NULL; 
    tmp = SDL_LoadBMP(file);
    if(!tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        SDL_DestroyTexture(texture);
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp); 
    if(!texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        SDL_DestroyTexture(texture);
        return NULL;
    }
    return texture;
}

size_t* in_square(Sint32 x, Sint32 y)
{
    int x_board = x - X_GAP;
    int y_board = y - Y_GAP;
    if (x_board < 0 || y_board < 0 || x_board > 700 || y_board > 700 ||x_board%100 > 80 || y_board%100 > 80)
        return NULL;
    size_t x_square = y_board/100; // Mouse position and squares positions are inverted in x,y   
    size_t y_square = x_board/100;
    size_t* pos_square = malloc(2*sizeof(size_t));
    pos_square[0] = x_square;
    pos_square[1] = y_square;
    return pos_square;
}

void display_pieces(struct board* board, SDL_Renderer *renderer, SDL_Texture* BluePiece, SDL_Texture* RedPiece,
                                                                 SDL_Texture* GreenPiece, SDL_Texture* YellowPiece)
{
    SDL_Rect pos_piece = {0, 0, 0, 0};
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            pos_piece.x = board->data[i][j]->pos[0] + 5; // +5 since the piece is not centred for some reasons
            pos_piece.y = board->data[i][j]->pos[1] + 5;
            if (board->data[i][j]->color == BLUE)
            {
                SDL_QueryTexture(BluePiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
                SDL_RenderCopy(renderer, BluePiece, NULL, &pos_piece);
            }
            if (board->data[i][j]->color == RED)
            {
                SDL_QueryTexture(RedPiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
                SDL_RenderCopy(renderer, RedPiece, NULL, &pos_piece);
            }
            if (board->data[i][j]->color == GREEN)
            {
                SDL_QueryTexture(GreenPiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
                SDL_RenderCopy(renderer, GreenPiece, NULL, &pos_piece);
            }
            if (board->data[i][j]->color == YELLOW)
            {
                SDL_QueryTexture(YellowPiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
                SDL_RenderCopy(renderer, YellowPiece, NULL, &pos_piece);
            }
        }
    }
}

void display_walls(struct board* board, SDL_Renderer *renderer, SDL_Texture* BlueWall, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenWall, SDL_Texture* YellowWall)
{
    SDL_Rect pos_wall = {0, 0, 0, 0};
    for (size_t i = 0; i < board->size - 1; i++)
    {
        for (size_t j = 0; j < board->size - 1; j++)
        {
            if (board->data[i][j]->walls[SOUTH])
            {
                pos_wall.x = board->data[i][j]->pos[0] - X_HWALL_GAP; 
                pos_wall.y = board->data[i][j]->pos[1] + Y_HWALL_GAP;
                if (board->data[i][j]->color == BLUE)
                {
                    SDL_QueryTexture(BlueWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopy(renderer, BlueWall, NULL, &pos_wall);
                }
                if (board->data[i][j]->color == RED)
                {
                    SDL_QueryTexture(RedWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopy(renderer, RedWall, NULL, &pos_wall);
                }
                if (board->data[i][j]->color == GREEN)
                {
                    SDL_QueryTexture(GreenWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopy(renderer, GreenWall, NULL, &pos_wall);
                }
                if (board->data[i][j]->color == YELLOW)
                {
                    SDL_QueryTexture(YellowWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopy(renderer, YellowWall, NULL, &pos_wall);
                }
            }
            if (board->data[i][j]->walls[EAST])
            {
                pos_wall.x = board->data[i][j]->pos[0] + X_VWALL_GAP; 
                pos_wall.y = board->data[i][j]->pos[1] + Y_VWALL_GAP;
                if (board->data[i][j]->color == BLUE)
                {
                    SDL_QueryTexture(BlueWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopyEx(renderer, BlueWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
                }
                if (board->data[i][j]->color == RED)
                {
                    SDL_QueryTexture(RedWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopyEx(renderer, RedWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
                }
                if (board->data[i][j]->color == GREEN)
                {
                    SDL_QueryTexture(GreenWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopyEx(renderer, GreenWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
                }
                if (board->data[i][j]->color == YELLOW)
                {
                    SDL_QueryTexture(YellowWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
                    SDL_RenderCopyEx(renderer, YellowWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
                }
            }
        }
    }
}

void display_board(struct board* board, SDL_Renderer *renderer, SDL_Texture* BluePiece, SDL_Texture* BlueWall, 
                                                                SDL_Texture* RedPiece, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenPiece, SDL_Texture* GreenWall,
                                                                SDL_Texture* YellowPiece, SDL_Texture* YellowWall)
{
    display_pieces(board, renderer, BluePiece, RedPiece, GreenPiece, YellowPiece);
    display_walls(board, renderer, BlueWall, RedWall, GreenWall, YellowWall);
}