#include "game_object.h"

GameObject *newGameObject() {
    GameObject *obj = malloc(sizeof(GameObject));
    obj->x = 0;
    obj->y = 0;
    obj->components = newVector();

    insert(objects, obj);
    return obj;
}

void deleteGameObject(GameObject *obj) {
    deleteVector(obj->components);
    free(obj);
    obj = NULL;
}

void update(Uint32 interval, GameObject *obj) {
    Component *c;
    for (int i = 0; i < obj->components->count; ++i) {
        c = get(obj->components, i);
        if (c->update) {
            c->update(interval, obj);
        }
    }
}

void respond(SDL_Event *evt, GameObject *obj) {
    Component *c;
    for (int i = 0; i < obj->components->count; ++i) {
        c = get(obj->components, i);
        if (c->respond) {
            c->respond(evt, obj);
        }
    }
}

Component *getComponent(GameObject *obj, Type t) {
    Component *c;
    for (int i = 0; i < obj->components->count; ++i) {
        c = get(obj->components, i);
        if (c->type == t) {
            return c;
        }
    }
    return NULL;
}
