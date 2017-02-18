#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

typedef struct {
   int temp; 
} ControlData;

Component *Control();
void deleteControl(Component *control);

void updateControl(Uint32 interval, GameObject *self);
void respondControl(SDL_Event *evt, GameObject *self);

#endif /*CONTROL_H*/
