#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

const static float G = 0.005;

typedef struct {
    Sint8 button;
    Sint8 hat;
    Sint8 pos_axis;
    Sint8 neg_axis;
} controller_t;

typedef struct {
    SDL_Scancode key;
    controller_t *controller;
    SDL_Rect *touch;
} control_t;

typedef struct {
   float vy;
   float vx;
   Uint32 friction;
   control_t *up;
   control_t *left;
   control_t *right;
} ControlData;

Component *Control(Uint32 friction, control_t *up, control_t *left, control_t *right);
void deleteControl(Component *control);

void updateControl(Uint32 interval, GameObject *self);
void respondControl(SDL_Event *evt, GameObject *self);

float getInput(control_t *controls);
SDL_bool checkInputEvent(SDL_Event *evt, control_t *controls);

#endif /*CONTROL_H*/
