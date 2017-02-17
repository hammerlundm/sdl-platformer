#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "sprite.h"

#include <stdio.h>

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("SDL error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface *image = IMG_Load("test.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    Sprite s = (Sprite) {texture, (SDL_Rect) {0, 0, 64, 64}, (SDL_Rect) {0, 0, 128, 128}};
    SDL_RenderCopy(renderer, s.texture, &s.source_rect, &s.screen_rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
