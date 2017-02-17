#ifndef SPRITE_H
#define SPRITE_H

#include "SDL2/SDL.h"

typedef struct {
    SDL_Texture *texture;
    SDL_Rect source_rect;
    SDL_Rect screen_rect;
} Sprite;

#endif
