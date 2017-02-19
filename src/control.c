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
    float y = getInput(current_controls, 0) / 10;
    if (collision) {
        CollisionData *cd = collision->data;
        if (cd->collision == TOP) {
            data->vy = y < 0 ? 20*y : 0;
        }
        else {
            data->vy += G * interval;
        }
    }
    else {
        data->vy = y;
    }
    data->vx += getInput(current_controls, 1) / 10;
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
    else if (evt->type == SDL_KEYDOWN) {
        switch (evt->key.keysym.sym) {
            case SDLK_w:
            case SDLK_a:
            case SDLK_s:
            case SDLK_d:
                current_controls = WASD;
                break;
            case SDLK_UP:
            case SDLK_LEFT:
            case SDLK_DOWN:
            case SDLK_RIGHT:
                current_controls = ARROWS;
                break;
        }
    }
}

float getInput(control_t controls, SDL_bool horizontal) {
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    if (controls & WASD) {
        if (horizontal) {
            return keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A];
        }
        else {
            return keyboard[SDL_SCANCODE_S] - keyboard[SDL_SCANCODE_W];
        }
    }
    else if (controls & ARROWS) {
        if (horizontal) {
            return keyboard[SDL_SCANCODE_RIGHT] - keyboard[SDL_SCANCODE_LEFT];
        }
        else {
            return keyboard[SDL_SCANCODE_DOWN] - keyboard[SDL_SCANCODE_UP];
        }
    }
    return 0;
}
