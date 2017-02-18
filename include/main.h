#ifndef MAIN_H
#define MAIN_H

#include "SDL2/SDL.h"

typedef struct Component Component;
struct Component {
    void *data;
    void (*update)(Uint32 interval, Component *self);
    void (*respond)(SDL_Event *event, Component *self);
};

//Global Variables
SDL_Window *window;
SDL_Renderer *renderer;
SDL_bool running;

int init();
void quit();

#endif /*MAIN_H*/
