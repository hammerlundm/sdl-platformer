#ifndef MAIN_H
#define MAIN_H

#include "SDL2/SDL.h"
#include "vector.h"

typedef enum {
    DEFAULT = 0,
    WASD,
    ARROWS 
} control_t;

typedef enum {
    SPRITE,
    CONTROL,
    COLLISION
} Type;

typedef struct GameObject GameObject;

typedef struct {
    void *data;
    void (*update)(Uint32 interval, GameObject *self);
    void (*respond)(SDL_Event *event, GameObject *self);
    Type type;
} Component;

//Global Variables
Uint32 MOVEEVENT;
Uint32 COLLISIONEVENT;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_bool running;
vector *sprites;
vector *objects;
Uint32 time;
control_t current_controls;

int init();
void quit();

#endif /*MAIN_H*/
