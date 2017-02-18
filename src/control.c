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
        Component *sprite = getComponent(self, SPRITE);
        if (sprite) {
            SpriteData *data = sprite->data;
            switch (evt->key.keysym.sym) {
            case SDLK_w:
                data->dst_rect.y -= 1;
                break;
            case SDLK_a:
                data->dst_rect.x -= 1;
                break;
            case SDLK_s:
                data->dst_rect.y += 1;
                break;
            case SDLK_d:
                data->dst_rect.x += 1;
                break;
            }
        }
    }
}
