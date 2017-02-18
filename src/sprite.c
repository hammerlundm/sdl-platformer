#include "sprite.h"
#include "game_object.h"

Component *Sprite(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dst_rect) {
    Component *sprite = malloc(sizeof(Component));
    SpriteData *data = malloc(sizeof(SpriteData));

    data->texture = texture;
    data->src_rect = src_rect;
    data->dst_rect = dst_rect;

    sprite->data = data;
    sprite->update = NULL;
    sprite->respond = respondSprite;
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

void respondSprite(SDL_Event *event, GameObject *self) {
    if (event->type == MOVEEVENT && event->user.data1 == self) {
        Component *sprite = getComponent(self, SPRITE);
        SpriteData *data = sprite->data;
        data->dst_rect.x = self->x;
        data->dst_rect.y = self->y;
    }
}

void drawSprite(void *sprite) {
    Component *s = sprite;
    SpriteData *data = s->data;
    SDL_RenderCopy(renderer, data->texture, &data->src_rect, &data->dst_rect);
}
