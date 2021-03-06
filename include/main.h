#ifndef MAIN_H
#define MAIN_H

#include "SDL2/SDL.h"
#include "vector.h"

typedef enum {
    SPRITE,
    CONTROL,
    COLLISION
} ComponentType;

typedef enum {
    NONE=0,
    UP,
    LEFT,
    DOWN,
    RIGHT
} Direction;

typedef enum {
    TOP,
    SIDE
} SceneType;

typedef struct GameObject GameObject;

typedef struct {
    void *data;
    void (*update)(Uint32 interval, GameObject *self);
    void (*respond)(SDL_Event *event, GameObject *self);
    ComponentType type;
} Component;


//Global Variables
Uint32 MOVEEVENT;
Uint32 COLLISIONEVENT;

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Rect camera;
float zoom;
GameObject *focus;

SDL_bool running;
SDL_bool paused;

vector *sprites;
vector *objects;

Uint32 time;

SDL_TouchID touch;
SDL_Joystick *joystick;

int init();
void quit();

#endif /*MAIN_H*/
