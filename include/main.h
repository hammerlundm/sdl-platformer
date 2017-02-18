#ifndef MAIN_H
#define MAIN_H

#include "SDL2/SDL.h"
#include "vector.h"

typedef enum {
    NONE = 0,
    SPRITE,
    CONTROL,
} Type;

typedef struct GameObject GameObject;

typedef struct {
    void *data;
    void (*update)(Uint32 interval, GameObject *self);
    void (*respond)(SDL_Event *event, GameObject *self);
    Type type;
} Component;

//Global Variables
SDL_Window *window;
SDL_Renderer *renderer;
SDL_bool running;
vector *sprites;
vector *objects;

int init();
void quit();

#endif /*MAIN_H*/
