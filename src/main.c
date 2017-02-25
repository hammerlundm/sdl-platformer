#include "main.h"
#include "SDL2/SDL_image.h"
#include "sprite.h"
#include "game_object.h"
#include "control.h"
#include "collision.h"

#ifdef DEBUG
#include <stdio.h>
#endif

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
    Component *s1 = Sprite(texture, 4, rects, &r1, 200);
    Component *s2 = Sprite(texture, 2, rects+2, &r2, 100);
    Component *s3 = Sprite(texture, 1, rects, &r3, 0);
    Component *s4 = Sprite(texture, 1, rects+3, &r4, 0);
    Component *thing = Control(0.9);
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
                keyboard = SDL_TRUE;
            }
            else if (evt.type == SDL_JOYBUTTONDOWN) {
                keyboard = SDL_FALSE;
            }
            for (int i = 0; i < objects->count; ++i) {
                temp = vGet(objects, i);
                respond(&evt, temp);
            }
        }
        for (int i = 0; i < objects->count; ++i) {
            temp = vGet(objects, i);
            update(SDL_GetTicks() - time, temp);
        }
        camera.x += (focus->x - camera.w / 2 - camera.x)*(SDL_GetTicks()-time)/500;
        camera.y += (focus->y - camera.h / 2 - camera.y)*(SDL_GetTicks()-time)/200;
        time = SDL_GetTicks();
        SDL_RenderClear(renderer);
        map(sprites, drawSprite);
        SDL_RenderPresent(renderer);
    }
    deleteSprite(s1);
    deleteSprite(s2);
    deleteSprite(s3);
    deleteSprite(s4);
    deleteControl(thing);
    deleteCollision(c1);
    deleteCollision(c2);
    deleteCollision(c3);
    deleteCollision(c4);
    deleteGameObject(obj1);
    deleteGameObject(obj2);
    deleteGameObject(obj3);
    deleteGameObject(obj4);
    quit();
}

int init() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        #ifdef DEBUG
        printf("SDL error: %s\n", SDL_GetError());
        #endif
        return -1;
    }
    running = SDL_TRUE;
    sprites = newVector();
    objects = newVector();
    keyboard = SDL_TRUE;
    Uint32 events = SDL_RegisterEvents(2);
    #ifdef DEBUG
    if (events == (Uint32)-1) {
        printf("Error: too many event types to allocate\n");
    }
    #endif
    MOVEEVENT = events;
    COLLISIONEVENT = events + 1;
    if (SDL_NumJoysticks() > 0) {
        joystick = SDL_JoystickOpen(0);
        keyboard = SDL_FALSE;
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
    SDL_Quit();
}
