#include "sprite.h"
#include <stdlib.h>

Component *Sprite(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dst_rect) {
    Component *sprite = malloc(sizeof(Component));
    SpriteData *data = malloc(sizeof(SpriteData));

    data->texture = texture;
    data->src_rect = src_rect;
    data->dst_rect = dst_rect;

    sprite->data = data;
    sprite->update = NULL;
    sprite->respond = NULL;
    sprite->type = SPRITE;

    insert(sprites, sprite);
    return sprite;
}

void deleteSprite(Component *sprite) {
    if (sprite->type == SPRITE) {
        free(sprite->data);
        free(sprite);
        sprite = NULL;
    }
    #ifdef DEBUG
    else {
        printf("Error: Component at %p is not a sprite\n", sprite);
    }
    #endif
}

void drawSprite(void *sprite) {
    Component *s = sprite;
    SpriteData *data = s->data;
    SDL_RenderCopy(renderer, data->texture, &data->src_rect, &data->dst_rect);
}
