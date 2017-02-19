#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

typedef struct {
   float vy;
   float vx;
   float friction;
} ControlData;

Component *Control(float friction);
void deleteControl(Component *control);

void updateControl(Uint32 interval, GameObject *self);
void respondControl(SDL_Event *evt, GameObject *self);

float getInput(control_t controls, SDL_bool horizontal);

#endif /*CONTROL_H*/
