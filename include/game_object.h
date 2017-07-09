#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "main.h"

struct GameObject {
    float x;
    float y;
    vector *components;
};

GameObject *newGameObject();
void deleteGameObject(GameObject *obj);

void deleteComponent(Component *c);

void update(Uint32 interval, GameObject *obj);
void respond(SDL_Event *evt, GameObject *obj);

Component *getComponent(GameObject *obj, ComponentType t);
void move(GameObject *obj, float x, float y);

#endif /*GAME_OBJECT_H*/
