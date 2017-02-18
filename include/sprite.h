#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"

typedef struct {
    SDL_Texture *texture;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
} SpriteData;

Component *Sprite(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dst_rect);
void deleteSprite(Component *sprite);

void updateSprite(Uint32 interval, Component *self);
void respondsprite(SDL_Event *event, Component *self);

void drawSprite(void *sprite);

#endif /*SPRITE_H*/
