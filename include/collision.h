#ifndef COLLISION_H
#define COLLISION_H

#include "main.h"

typedef enum {
    NONE=0,
    TOP,
    LEFT,
    BOTTOM,
    RIGHT
} collision_t;

typedef struct {
   SDL_Rect bounds;
   SDL_bool fixed;
} CollisionData;

Component *Collision(SDL_Rect bounds, SDL_bool fixed);
void deleteCollision(Component *collision);

void updateCollision(Uint32 interval, GameObject *self);
void respondCollision(SDL_Event *event, GameObject *self);

collision_t checkCollision(SDL_Rect r1, SDL_Rect r2);

#endif /*COLLISION_H*/
