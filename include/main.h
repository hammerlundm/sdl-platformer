#ifndef MAIN_H
#define MAIN_H

#include "SDL2/SDL.h"

//Global Variables
SDL_Window *window;
SDL_Renderer *renderer;
SDL_bool running;

int init();
int quit();

#endif
