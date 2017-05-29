#include "main.h"
#include "SDL2/SDL_image.h"
#include "sprite.h"
#include "game_object.h"
#include "control.h"
#include "collision.h"
#include "ui.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void stop() {
    running = SDL_FALSE;
}

void resume() {
    paused = SDL_FALSE;
}

int main(int argc, char **argv) {
    if (init()) {
        quit();
        return -1;
    }
    SDL_Surface *image = IMG_Load("test.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Rect rects[4] = {{0, 0, 64, 64}, {64, 0, 64, 64}, {0, 64, 64, 64}, {64, 64, 64, 64}};
    SDL_Rect r1 = {0, 0, 128, 128};
    SDL_Rect r2 = {0, 0, 512, 64};
    SDL_Rect r3 = {0, 0, 256, 128};
    SDL_Rect r4 = {0, 0, 1024, 256};
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    SDL_Rect left_rect = {0, h/3, w/3, h/3};
    SDL_Rect right_rect = {2*w/3, h/3, w/3, h/3};
    SDL_Rect up_rect = {w/3, 0, w/3, h/3};
    controller_t up_controller = {0, SDL_HAT_UP, -1, -1};
    controller_t left_controller = {-1, SDL_HAT_LEFT, -1, 0};
    controller_t right_controller = {-1, SDL_HAT_RIGHT, 0, -1};
    control_t up_control = {SDL_SCANCODE_W, &up_controller, &up_rect};
    control_t left_control = {SDL_SCANCODE_A, &left_controller, &left_rect};
    control_t right_control = {SDL_SCANCODE_D, &right_controller, &right_rect};
    Component *s1 = Sprite(texture, 4, rects, &r1, 200);
    Component *s2 = Sprite(texture, 2, rects+2, &r2, 100);
    Component *s3 = Sprite(texture, 1, rects, &r3, 0);
    Component *s4 = Sprite(texture, 1, rects+3, &r4, 0);
    Component *thing = Control(250, &up_control, &left_control, &right_control);
    Component *c1 = Collision(r1, SDL_FALSE);
    Component *c2 = Collision(r2, SDL_TRUE);
    Component *c3 = Collision(r3, SDL_TRUE);
    Component *c4 = Collision(r4, SDL_TRUE);
    GameObject *obj1 = newGameObject();
    GameObject *obj2 = newGameObject();
    GameObject *obj3 = newGameObject();
    GameObject *obj4 = newGameObject();
    vInsert(obj1->components, s1);
    vInsert(obj2->components, s2);
    vInsert(obj3->components, s3);
    vInsert(obj4->components, s4);
    vInsert(obj1->components, thing);
    vInsert(obj1->components, c1);
    vInsert(obj2->components, c2);
    vInsert(obj3->components, c3);
    vInsert(obj4->components, c4);
    move(obj1, 400, 200);
    move(obj2, 100, 500);
    move(obj3, 612, 436);
    move(obj4, 868, 564);
    focus = obj1;

    SDL_Color bg = {32, 32, 32, 128};
    SDL_Color fg = {255, 255, 255, 255};
    SDL_Color bg1 = {128, 32, 32, 32};
    SDL_Color clear = {0, 0, 0, 0};
    UI_Element *pause = UI_NewLayout(&bg, &fg, NULL, UI_VERT, 50);
    UI_NewEmpty(&clear, &clear, pause);
    UI_Element *middle = UI_NewLayout(&clear, &fg, pause, UI_HORIZ, 20);
    UI_NewEmpty(&clear, &clear, pause);
    UI_NewButton(&bg1, &fg, middle, UI_RenderLines("Quit", &fg), stop)->scale = UI_KEEP;
    UI_NewButton(&bg1, &fg, middle, UI_RenderLines("Resume", &fg), resume)->scale = UI_KEEP;

    controller_t pause_controller = {9, -1, -1, -1};
    control_t pause_control = {SDL_SCANCODE_ESCAPE, &pause_controller, NULL};

    SDL_Event evt;
    GameObject *temp;
    time = SDL_GetTicks();
    while (running) {
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_WINDOWEVENT) {
                if (evt.window.event == SDL_WINDOWEVENT_CLOSE) {
                    running = SDL_FALSE;
                }
            }
            else if (evt.type == SDL_KEYDOWN) {
                if (evt.key.keysym.sym == SDLK_q) {
                    zoom *= 1.2;
                }
                else if (evt.key.keysym.sym == SDLK_e) {
                    zoom /= 1.2;
                }
            }
            if (checkInputEvent(&evt, &pause_control)) {
                paused = !paused;
            }
            if (paused) {
                UI_Respond(pause, &evt);
            }
            else {
                for (int i = 0; i < objects->count; i++) {
                    temp = vGet(objects, i);
                    respond(&evt, temp);
                }
            }
        }
        if (paused) {
            Uint32 t = SDL_GetTicks() - time;
            UI_Update(pause, &t);
        }
        else {
            for (int i = 0; i < objects->count; i++) {
                temp = vGet(objects, i);
                update(SDL_GetTicks() - time, temp);
            }
        }
        camera.x += (focus->x - camera.w / 2 - camera.x)*zoom*(SDL_GetTicks()-time)/500;
        camera.y += (focus->y - camera.h / 2 - camera.y)*zoom*(SDL_GetTicks()-time)/200;
        time = SDL_GetTicks();
        SDL_RenderClear(renderer);
        for (int i = 0; i < objects->count; i++) {
            drawSprite(vGet(sprites, i));
        }
        if (paused) {
            UI_Draw(pause);
        }
        SDL_RenderPresent(renderer);
    }
    for (int i = objects->count - 1; i >= 0; i--) {
        deleteGameObject(vGet(objects, i));
        vRemove(objects, i);
    }
    UI_FreeElement(pause);
    quit();
    return 0;
}

int init() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        #ifdef DEBUG
        printf("SDL error: %s\n", SDL_GetError());
        #endif
        return -1;
    }
    TTF_Init();
    running = SDL_TRUE;
    sprites = newVector();
    objects = newVector();
    Uint32 events = SDL_RegisterEvents(2);
    zoom = 1.;
    touch = SDL_GetTouchDevice(0);
    #ifdef DEBUG
    if (events == (Uint32)-1) {
        printf("Error: too many event types to allocate\n");
    }
    #endif
    MOVEEVENT = events;
    COLLISIONEVENT = events + 1;
    if (SDL_NumJoysticks() > 0) {
        joystick = SDL_JoystickOpen(0);
        #ifdef DEBUG
        if (joystick == NULL) {
            printf("SDL error: %s\n", SDL_GetError());
        }
        else {
            printf("Joystick connected\n");
        }
        #endif
    }
    #ifdef DEBUG
    else {
        printf("No joysticks detected\n");
    }
    #endif
    window = SDL_CreateWindow("Game!", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 1600, 900, 0);
    if (!window) {
        #ifdef DEBUG
        printf("SDL error: %s\n", SDL_GetError());
        #endif
        return -1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        #ifdef DEBUG
        printf("SDL error: %s\n", SDL_GetError());
        #endif
        return -1;
    }
    UI_Init(renderer, TTF_OpenFont("font.ttf", 72), NULL);
    camera = (SDL_Rect) {0, 0, 1600, 900};
    return 0;
}

void quit() {
    if (SDL_JoystickGetAttached(joystick)) {
        SDL_JoystickClose(joystick);
    }
    deleteVector(sprites);
    deleteVector(objects);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    UI_Quit();
    TTF_Quit();
    SDL_Quit();
}
