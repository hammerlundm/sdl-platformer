#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

const static float G = 0.005;

typedef struct {
    SDL_Scancode key;
    Sint8 axis;
    Uint8 button;
    Uint8 hat;
} control_t;

typedef struct {
   float vy;
   float vx;
   Uint32 friction;
   control_t up;
   control_t left;
   control_t right;
} ControlData;

Component *Control(Uint32 friction);
void deleteControl(Component *control);

void updateControl(Uint32 interval, GameObject *self);
void respondControl(SDL_Event *evt, GameObject *self);

float getInput(control_t controls);

#endif /*CONTROL_H*/
