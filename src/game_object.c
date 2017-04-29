#include "game_object.h"
#include "sprite.h"
#include "collision.h"
#include "control.h"

GameObject *newGameObject() {
    GameObject *obj = malloc(sizeof(GameObject));
    obj->x = 0;
    obj->y = 0;
    obj->components = newVector();

    vInsert(objects, obj);
    return obj;
}

void deleteGameObject(GameObject *obj) {
    for (int i = 0; i < obj->components->count; i++) {
        deleteComponent(vGet(obj->components, i));
    }
    deleteVector(obj->components);
    free(obj);
    obj = NULL;
}

void deleteComponent(Component *c) {
    if (c->type == SPRITE) {
        deleteSprite(c);
    }
    else if (c->type == COLLISION) {
        deleteCollision(c);
    }
    else if (c->type == CONTROL) {
        deleteControl(c);
    }
}

void update(Uint32 interval, GameObject *obj) {
    Component *c;
    for (int i = 0; i < obj->components->count; i++) {
        c = vGet(obj->components, i);
        if (c->update) {
            c->update(interval, obj);
        }
    }
}

void respond(SDL_Event *evt, GameObject *obj) {
    Component *c;
    for (int i = 0; i < obj->components->count; i++) {
        c = vGet(obj->components, i);
        if (c->respond) {
            c->respond(evt, obj);
        }
    }
}

Component *getComponent(GameObject *obj, Type t) {
    Component *c;
    for (int i = 0; i < obj->components->count; i++) {
        c = vGet(obj->components, i);
        if (c->type == t) {
            return c;
        }
    }
    return NULL;
}

void move(GameObject *obj, float x, float y) {
    obj->x += x;
    obj->y += y;
    SDL_Event event;
    event.type = MOVEEVENT;
    event.user.code = 0;
    event.user.data1 = obj;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
}
