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
    SDL_Rect r2 = {0, 0, 64, 64};
    Component *s1 = Sprite(texture, 4, rects, &r1, 200);
    Component *s2 = Sprite(texture, 2, rects+2, &r2, 100);
    Component *thing = Control(0.9);
    Component *c1 = Collision(r1, SDL_TRUE);
    Component *c2 = Collision(r2, SDL_FALSE);
    GameObject *obj1 = newGameObject();
    GameObject *obj2 = newGameObject();
    insert(obj1->components, s1);
    insert(obj2->components, s2);
    insert(obj2->components, thing);
    insert(obj1->components, c1);
    insert(obj2->components, c2);
    move(obj1, 400, 200);
    move(obj2, 100, 200);

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
            for (int i = 0; i < objects->count; ++i) {
                temp = get(objects, i);
                respond(&evt, temp);
            }
        }
        for (int i = 0; i < objects->count; ++i) {
            temp = get(objects, i);
            update(SDL_GetTicks() - time, temp);
        }
        time = SDL_GetTicks();
        SDL_RenderClear(renderer);
        map(sprites, drawSprite);
        SDL_RenderPresent(renderer);
    }
    deleteSprite(s1);
    deleteSprite(s2);
    deleteControl(thing);
    deleteGameObject(obj1);
    deleteGameObject(obj2);
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
    current_controls = WASD;
    Uint32 events = SDL_RegisterEvents(2);
    #ifdef DEBUG
    if (events == (Uint32)-1) {
        printf("Error: too many event types to allocate\n");
    }
    #endif
    MOVEEVENT = events;
    COLLISIONEVENT = events + 1;
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
    return 0;
}

void quit() {
    deleteVector(sprites);
    deleteVector(objects);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
