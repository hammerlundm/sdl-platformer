#include "control.h"
#include "game_object.h"
#include "sprite.h"
#include "collision.h"

Component *Control(float friction) {
    Component *control = malloc(sizeof(Component));
    ControlData *data = malloc(sizeof(ControlData));

    data->vx = 0;
    data->vy = 0;
    data->friction = friction;
    data->up = (control_t) {SDL_SCANCODE_W, 0, 1, 2};
    data->left = (control_t) {SDL_SCANCODE_A, -1, 0, 9};
    data->right = (control_t) {SDL_SCANCODE_D, 1, 0, 3};

    control->data = data;
    control->update = updateControl;
    control->respond =respondControl;
    control->type = CONTROL;

    return control;
}

void deleteControl(Component *control) {
    if (control->type == CONTROL) {
        free(control->data);
        free(control);
        control = NULL;
    }
    #ifdef DEBUG
    else {
        printf("Error: Component at %p is not a control\n", control);
    }
    #endif
}

void updateControl(Uint32 interval, GameObject *self) {
    ControlData *data = getComponent(self, CONTROL)->data;
    Component *collision = getComponent(self, COLLISION);
    if (collision) {
        CollisionData *cd = collision->data;
        if (cd->collision == TOP) {
            data->vy = -2*getInput(data->up);
        }
        else {
            data->vy += G * interval;
        }
    }
    else {
        data->vy = getInput(data->up);
    }
    data->vx += (getInput(data->right) - getInput(data->left)) / 10;
    data->vx *= data->friction;
    data->vy *= data->friction;
    move(self, data->vx * interval, data->vy * interval);
}

void respondControl(SDL_Event *evt, GameObject *self) {
    if (evt->type == COLLISIONEVENT && evt->user.data1 == self) {
        ControlData *data = getComponent(self, CONTROL)->data;
        if (evt->user.code == TOP || evt->user.code == BOTTOM) {
            data->vy = 0;
        }
        else if (evt->user.code == LEFT || evt->user.code == RIGHT) {
            data->vx = 0;
        }
    }
}

float getInput(control_t controls) {
    if (keyboard) {
        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
        return keyboard[controls.key];
    }
    else {
        float val = 0;
        if (controls.axis) {
            Uint8 axis;
            if (controls.axis < 0) {
                axis = -controls.axis - 1;
            }
            else {
                axis = controls.axis - 1;
            }
            val = SDL_JoystickGetAxis(joystick, axis) / 32768.;
            if (val < 0 && controls.axis < 0) {
                val = -val;
            }
            else if (val > 0 && controls.axis > 0) {
            }
            else {
                val = 0;
            }
        }
        if (val == 0 && controls.button) {
            val = SDL_JoystickGetButton(joystick, controls.button - 1);
        }
        if (val == 0 && controls.hat) {
            val = SDL_JoystickGetHat(joystick, 0) & (controls.hat - 1) ? 1 : 0;
        }
        return val;
    }
}
