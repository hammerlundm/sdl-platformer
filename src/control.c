#include "control.h"
#include "game_object.h"
#include "sprite.h"
#include "collision.h"
#include "math.h"

Component *ControlTop(Uint32 friction, control_t *up, control_t *down, control_t *left, control_t *right) {
    Component *control = malloc(sizeof(Component));
    ControlData *data = malloc(sizeof(ControlData));

    data->vx = 0;
    data->vy = 0;
    data->friction = friction;
    data->type = TOP;
    data->up = up;
    data->down = down;
    data->left = left;
    data->right = right;

    control->data = data;
    control->update = updateControlTop;
    control->respond = respondControl;
    control->type = CONTROL;

    return control;
}

Component *ControlSide(Uint32 friction, control_t *up, control_t *down, control_t *left, control_t *right) {
    Component *control = malloc(sizeof(Component));
    ControlData *data = malloc(sizeof(ControlData));

    data->vx = 0;
    data->vy = 0;
    data->friction = friction;
    data->type = SIDE;
    data->up = up;
    data->down = down;
    data->left = left;
    data->right = right;

    control->data = data;
    control->update = updateControlSide;
    control->respond = respondControl;
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

void updateControlTop(Uint32 interval, GameObject *self) {
    ControlData *data = getComponent(self, CONTROL)->data;
    data->vx += (getInput(data->right) - getInput(data->left)) / 10;
    data->vx *= pow(0.5, (float) interval / data->friction);
    data->vy += (getInput(data->down) - getInput(data->up)) / 10;
    data->vy *= pow(0.5, (float) interval / data->friction);
    move(self, data->vx * interval, data->vy * interval);
}

void updateControlSide(Uint32 interval, GameObject *self) {
    ControlData *data = getComponent(self, CONTROL)->data;
    Component *collision = getComponent(self, COLLISION);
    if (collision) {
        CollisionData *cd = collision->data;
        if (cd->collision == UP) {
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
    data->vx *= pow(0.5, (float) interval / data->friction);
    data->vy *= pow(0.5, (float) interval / data->friction);
    move(self, data->vx * interval, data->vy * interval);
}

void respondControl(SDL_Event *evt, GameObject *self) {
    if (evt->type == COLLISIONEVENT && evt->user.data1 == self) {
        ControlData *data = getComponent(self, CONTROL)->data;
        if (evt->user.code == UP || evt->user.code == DOWN) {
            data->vy = 0;
        }
        else if (evt->user.code == LEFT || evt->user.code == RIGHT) {
            data->vx = 0;
        }
    }
}

float getInput(control_t *controls) {
    SDL_Finger *finger = SDL_GetTouchFinger(touch, 0);
    float val = 0.;
    if (joystick && controls->controller) {
        if (controls->controller->button >= 0) {
            val = SDL_JoystickGetButton(joystick, controls->controller->button);
        }
        if (val == 0. && controls->controller->hat >= 0) {
            val = SDL_JoystickGetHat(joystick, 0) & (controls->controller->hat) ? 1. : 0.;
        }
        if (val == 0. && controls->controller->pos_axis >= 0) {
            float x = SDL_JoystickGetAxis(joystick, controls->controller->pos_axis) / 32768.;
            if (x > 0) val = x;
        }
        if (val == 0. && controls->controller->neg_axis >= 0) {
            float x = SDL_JoystickGetAxis(joystick, controls->controller->neg_axis) / 32768.;
            if (x < 0) val = -x;
        }
    }
    if (val == 0. && controls->touch && finger) {
        SDL_Rect r = (SDL_Rect) {finger->x, finger->y, 0, 0};
        if (checkCollision(*controls->touch, r)) {
            val = finger->pressure;
        }
    }
    if (val == 0. && controls->key != SDL_SCANCODE_UNKNOWN) {
        val = SDL_GetKeyboardState(NULL)[controls->key];
    }
    return val;
}

SDL_bool checkInputEvent(SDL_Event *evt, control_t *controls) {
    if (controls->controller) {
        if (evt->type == SDL_JOYBUTTONDOWN && controls->controller->button >= 0) {
            return evt->jbutton.button == controls->controller->button;
        }
        else if (evt->type == SDL_JOYHATMOTION && controls->controller->hat >= 0) {
            return evt->jhat.value == controls->controller->hat;
        }
        else if (evt->type == SDL_JOYAXISMOTION) {
            if (evt->jaxis.value / 32768. > 0.1 && controls->controller->pos_axis >= 0) {
                return evt->jaxis.axis == controls->controller->pos_axis;
            }
            else if (evt->jaxis.value / 32768. < 0.1 && controls->controller->neg_axis >= 0) {
                return evt->jaxis.axis == controls->controller->neg_axis;
            }
        }
    }
    if (evt->type == SDL_KEYDOWN && controls->key != SDL_SCANCODE_UNKNOWN) {
        return evt->key.keysym.scancode == controls->key;
    }
    else if (evt->type == SDL_FINGERUP && controls->touch) {
        SDL_Rect finger = {evt->tfinger.x, evt->tfinger.y, 0, 0};
        return checkCollision(*controls->touch, finger);
    }
    else if (evt->type == SDL_MOUSEBUTTONDOWN && controls->touch) {
        SDL_Rect mouse = {evt->button.x, evt->button.y, 0, 0};
        return checkCollision(*controls->touch, mouse);
    }
    return SDL_FALSE;
}
