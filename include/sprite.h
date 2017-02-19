#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"

typedef struct {
    SDL_Texture *texture;
    Uint32 time;
    Uint32 time_left;
    int frame;
    vector *src_rects;
    SDL_Rect *dst_rect;
} SpriteData;

Component *Sprite(SDL_Texture *texture, int frames, SDL_Rect *src_rects, SDL_Rect *dst_rect, Uint32 time);
void deleteSprite(Component *sprite);

void updateSprite(Uint32 interval, GameObject *self);
void respondSprite(SDL_Event *evt, GameObject *self);

void drawSprite(void *sprite);

#endif /*SPRITE_H*/
