#include "main.h"
#include "SDL2/SDL_image.h"
#include "sprite.h"

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
    Sprite s = (Sprite) {texture, (SDL_Rect) {0, 0, 64, 64}, (SDL_Rect) {0, 0, 128, 128}};
    SDL_Event evt;
    while (running) {
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_WINDOWEVENT) {
                if (evt.window.event == SDL_WINDOWEVENT_CLOSE) {
                    running = SDL_FALSE;
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, s.texture, &s.source_rect, &s.screen_rect);
        SDL_RenderPresent(renderer);
    }
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
    window = SDL_CreateWindow("Game!", SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 640, 480, 0);
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

int quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
