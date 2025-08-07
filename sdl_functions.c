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

void in_square(Sint32 x, Sint32 y, Sint32 pos_square[2])
{
    Sint32 x_board = x - X_GAP;
    Sint32 y_board = y - Y_GAP;
    if (x_board < 5 || y_board < 5 || x_board > 695 || y_board > 695 ||x_board%100 > 75 || y_board%100 > 75)
        {
            pos_square[0] = -1;
            pos_square[1] = -1;
        }
    else
    {
        Sint32 x_square = y_board/100; // Mouse position and squares positions are inverted in x,y   
        Sint32 y_square = x_board/100;
        pos_square[0] = x_square;
        pos_square[1] = y_square;
    }
}

bool in_rect(Sint32 x, Sint32 y, SDL_Rect rec)
{
    return ((x >= rec.x) && (x <= rec.x + rec.w) && (y >= rec.y) && (y <= rec.y + rec.h));
}

enum Directions on_wall(Sint32 x, Sint32 y, Sint32 pos_square[2])
{
    SDL_Rect north_wall = {X_GAP + pos_square[1]*SQUARE_SIZE - 5, Y_GAP + pos_square[0] * SQUARE_SIZE - 25, WALL_WIDTH, WALL_HEIGHT};
    SDL_Rect east_wall = {X_GAP + pos_square[1] * SQUARE_SIZE + 75, Y_GAP + pos_square[0]*SQUARE_SIZE - 5, WALL_HEIGHT, WALL_WIDTH};
    SDL_Rect south_wall = {X_GAP + pos_square[1] * SQUARE_SIZE - 5, Y_GAP + pos_square[0]*SQUARE_SIZE + 75, WALL_WIDTH, WALL_HEIGHT};
    SDL_Rect west_wall = {X_GAP + pos_square[1] * SQUARE_SIZE - 25, Y_GAP + pos_square[0]*SQUARE_SIZE - 5, WALL_HEIGHT, WALL_WIDTH};
    if (in_rect(x, y, north_wall))
        return NORTH;
    if (in_rect(x, y, east_wall))
        return EAST;
    if (in_rect(x, y, south_wall))
        return SOUTH;
    if (in_rect(x, y, west_wall))
        return WEST;
    return -1;
}

void display_piece(SDL_Renderer *renderer, Sint32 pos[2], enum Colors color, SDL_Texture* BluePiece, SDL_Texture* RedPiece,
                                                                 SDL_Texture* GreenPiece, SDL_Texture* YellowPiece, Uint8 alpha)
{
    SDL_Rect pos_piece = {0, 0, 0, 0};
    pos_piece.x = X_GAP + pos[1] * SQUARE_SIZE;
    pos_piece.y = X_GAP + pos[0] * SQUARE_SIZE;
    
    if (color == BLUE)
    {
        SDL_SetTextureAlphaMod(BluePiece, alpha);
        SDL_QueryTexture(BluePiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
        SDL_RenderCopy(renderer, BluePiece, NULL, &pos_piece);
        SDL_SetTextureAlphaMod(BluePiece, 255);
    }
    if (color == RED)
    {
        SDL_SetTextureAlphaMod(RedPiece, alpha);
        SDL_QueryTexture(RedPiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
        SDL_RenderCopy(renderer, RedPiece, NULL, &pos_piece);
        SDL_SetTextureAlphaMod(RedPiece, 255);
    }
    if (color == GREEN)
    {
        SDL_SetTextureAlphaMod(GreenPiece, alpha);
        SDL_QueryTexture(GreenPiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
        SDL_RenderCopy(renderer, GreenPiece, NULL, &pos_piece);
        SDL_SetTextureAlphaMod(GreenPiece, 255);
    }
    if (color == YELLOW)
    {
        SDL_SetTextureAlphaMod(YellowPiece, alpha);
        SDL_QueryTexture(YellowPiece, NULL, NULL, &pos_piece.w, &pos_piece.h);
        SDL_RenderCopy(renderer, YellowPiece, NULL, &pos_piece);
        SDL_SetTextureAlphaMod(YellowPiece, 255);
    } 
}

void display_pieces(struct board* board, SDL_Renderer *renderer, SDL_Texture* BluePiece, SDL_Texture* RedPiece,
                                                                 SDL_Texture* GreenPiece, SDL_Texture* YellowPiece)
{
    //SDL_Rect pos_piece = {0, 0, 0, 0};
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            Sint32 pos[2] = {i, j};
            display_piece(renderer, pos, board->data[i][j]->color, BluePiece, RedPiece, GreenPiece, YellowPiece, 255);
        }
    }
}

void display_wall(SDL_Renderer *renderer, struct square* square, enum Colors color, enum Directions direction, SDL_Texture* BlueWall, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenWall, SDL_Texture* YellowWall, Uint8 alpha)
{
    SDL_Rect pos_wall = {0, 0, 0, 0};
    if (direction == NORTH)
    {
        pos_wall.x = square->pos[0] - X_HWALL_GAP; 
        pos_wall.y = square->pos[1] + Y_HWALL_GAP - SQUARE_SIZE;
        if (color == BLUE)
        {
            SDL_SetTextureAlphaMod(BlueWall, alpha);
            SDL_QueryTexture(BlueWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, BlueWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(BlueWall, 255);
        }
        if (color == RED)
        {
            SDL_SetTextureAlphaMod(RedWall, alpha);
            SDL_QueryTexture(RedWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, RedWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(RedWall, 255);
        }
        if (color == GREEN)
        {
            SDL_SetTextureAlphaMod(GreenWall, alpha);
            SDL_QueryTexture(GreenWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, GreenWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(GreenWall, 255);
        }
        if (color == YELLOW)
        {
            SDL_SetTextureAlphaMod(YellowWall, alpha);
            SDL_QueryTexture(YellowWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, YellowWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(YellowWall, 255);
        }
    }
    if (direction == EAST)
    {
        pos_wall.x = square->pos[0] + X_VWALL_GAP; 
        pos_wall.y = square->pos[1] + Y_VWALL_GAP;
        if (color == EAST)
        {
            SDL_SetTextureAlphaMod(BlueWall, alpha);
            SDL_QueryTexture(BlueWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, BlueWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(BlueWall, 255);
        }
        if (color == RED)
        {
            SDL_SetTextureAlphaMod(RedWall, alpha);
            SDL_QueryTexture(RedWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, RedWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(RedWall, 255);
        }
        if (color == GREEN)
        {
            SDL_SetTextureAlphaMod(GreenWall, alpha);
            SDL_QueryTexture(GreenWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, GreenWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(GreenWall, 255);
        }
        if (color == YELLOW)
        {
            SDL_SetTextureAlphaMod(YellowWall, alpha);
            SDL_QueryTexture(YellowWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, YellowWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(YellowWall, 255);
        }
    }
    if (direction == SOUTH)
    {
        pos_wall.x = square->pos[0] - X_HWALL_GAP; 
        pos_wall.y = square->pos[1] + Y_HWALL_GAP;
        if (color == BLUE)
        {
            SDL_SetTextureAlphaMod(BlueWall, alpha);
            SDL_QueryTexture(BlueWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, BlueWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(BlueWall, 255);
        }
        if (color == RED)
        {
            SDL_SetTextureAlphaMod(RedWall, alpha);
            SDL_QueryTexture(RedWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, RedWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(RedWall, 255);
        }
        if (color == GREEN)
        {
            SDL_SetTextureAlphaMod(GreenWall, alpha);
            SDL_QueryTexture(GreenWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, GreenWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(GreenWall, 255);
        }
        if (color == YELLOW)
        {
            SDL_SetTextureAlphaMod(YellowWall, alpha);
            SDL_QueryTexture(YellowWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopy(renderer, YellowWall, NULL, &pos_wall);
            SDL_SetTextureAlphaMod(YellowWall, 255);
        }
    }
    if (direction == WEST)
    {
        pos_wall.x = square->pos[0] + X_VWALL_GAP - SQUARE_SIZE; 
        pos_wall.y = square->pos[1] + Y_VWALL_GAP;
        if (color == BLUE)
        {
            SDL_SetTextureAlphaMod(BlueWall, alpha);
            SDL_QueryTexture(BlueWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, BlueWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(BlueWall, 255);
        }
        if (color == RED)
        {
            SDL_SetTextureAlphaMod(RedWall, alpha);
            SDL_QueryTexture(RedWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, RedWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(RedWall, 255);
        }
        if (color == GREEN)
        {
            SDL_SetTextureAlphaMod(GreenWall, alpha);
            SDL_QueryTexture(GreenWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, GreenWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(GreenWall, 255);
        }
        if (color == YELLOW)
        {
            SDL_SetTextureAlphaMod(YellowWall, alpha);
            SDL_QueryTexture(YellowWall, NULL, NULL, &pos_wall.w, &pos_wall.h);
            SDL_RenderCopyEx(renderer, YellowWall, NULL, &pos_wall, 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(YellowWall, 255);
        }
    }
    
}

void display_walls(struct board* board, SDL_Renderer *renderer, SDL_Texture* BlueWall, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenWall, SDL_Texture* YellowWall)
{
    for (size_t i = 0; i < board->size; i++)
    {
        for (size_t j = 0; j < board->size; j++)
        {
            if (board->data[i][j]->walls[SOUTH] && i != board->size - 1)
            {
                display_wall(renderer, board->data[i][j], board->data[i][j]->walls_color[SOUTH] , SOUTH, BlueWall, RedWall, GreenWall, YellowWall, 255);
            }
            if (board->data[i][j]->walls[EAST] && j != board->size - 1)
            {
                display_wall(renderer, board->data[i][j], board->data[i][j]->walls_color[EAST], EAST, BlueWall, RedWall, GreenWall, YellowWall, 255);
            }
        }
    }
}


void display_board(struct board* board, SDL_Renderer *renderer, SDL_Texture* text_board, SDL_Texture* BluePiece, SDL_Texture* BlueWall, 
                                                                SDL_Texture* RedPiece, SDL_Texture* RedWall,
                                                                SDL_Texture* GreenPiece, SDL_Texture* GreenWall,
                                                                SDL_Texture* YellowPiece, SDL_Texture* YellowWall)
{
    SDL_Rect rec = {0, 0, Window_width, Window_height};
    SDL_QueryTexture(text_board, NULL, NULL, &rec.w, &rec.h);
    SDL_RenderCopy(renderer, text_board, NULL, &rec);
    display_pieces(board, renderer, BluePiece, RedPiece, GreenPiece, YellowPiece);
    display_walls(board, renderer, BlueWall, RedWall, GreenWall, YellowWall);
}