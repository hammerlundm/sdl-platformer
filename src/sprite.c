#include "sprite.h"
#include "game_object.h"

Component *Sprite(SDL_Texture *texture, int frames, SDL_Rect *src_rects, SDL_Rect *dst_rect, Uint32 time) {
    Component *sprite = malloc(sizeof(Component));
    SpriteData *data = malloc(sizeof(SpriteData));

    data->texture = texture;
    data->src_rects = newVector();
    data->time = time;
    data->time_left = time;
    data->frame = 0;
    for (int i = 0; i < frames; ++i) {
        insert(data->src_rects, src_rects+i);
    }
    data->dst_rect = dst_rect;

    sprite->data = data;
    sprite->update = updateSprite;
    sprite->respond = respondSprite;
    sprite->type = SPRITE;

    insert(sprites, sprite);
    return sprite;
}

void deleteSprite(Component *sprite) {
    if (sprite->type == SPRITE) {
        deleteVector(((SpriteData *)sprite->data)->src_rects);
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

void updateSprite(Uint32 interval, GameObject *self) {
    SpriteData *data = getComponent(self, SPRITE)->data;
    if (data->time && data->time_left <= interval) {
        data->time_left = data->time;
        data->frame++;
        if (data->frame >= data->src_rects->count) {
            data->frame = 0;
        }
    }
    data->time_left -= interval;
}

void respondSprite(SDL_Event *evt, GameObject *self) {
    if (evt->type == MOVEEVENT && evt->user.data1 == self) {
        Component *sprite = getComponent(self, SPRITE);
        SpriteData *data = sprite->data;
        data->dst_rect->x = self->x;
        data->dst_rect->y = self->y;
    }
}

void drawSprite(void *sprite) {
    Component *s = sprite;
    SpriteData *data = s->data;
    SDL_RenderCopy(renderer, data->texture, get(data->src_rects, data->frame), data->dst_rect);
}
