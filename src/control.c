#include "control.h"
#include "game_object.h"
#include "sprite.h"

Component *Control() {
    Component *control = malloc(sizeof(Component));
    ControlData *data = malloc(sizeof(ControlData));

    data->temp = 0;

    control->data = data;
    control->update = NULL;
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

void respondControl(SDL_Event *evt, GameObject *self) {
    if (evt->type == SDL_KEYDOWN) {
        if (evt->key.keysym.sym == SDLK_w) {
            move(self, 0, -1);
        }
        else if (evt->key.keysym.sym == SDLK_a) {
            move(self, -1, 0);
        }
        else if (evt->key.keysym.sym == SDLK_s) {
            move(self, 0, 1);
        }
        else if (evt->key.keysym.sym == SDLK_d) {
            move(self, 1, 0);
        }
    }
}
