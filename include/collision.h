#ifndef COLLISION_H
#define COLLISION_H

#include "main.h"

typedef struct {
    SDL_Rect bounds;
    SDL_bool fixed;
    Direction collision;
} CollisionData;

Component *Collision(SDL_Rect bounds, SDL_bool fixed);
void deleteCollision(Component *collision);

void updateCollision(Uint32 interval, GameObject *self);
void respondCollision(SDL_Event *event, GameObject *self);

Direction checkCollision(SDL_Rect r1, SDL_Rect r2);

#endif /*COLLISION_H*/
