#include "collision.h"
#include "game_object.h"

Component *Collision(SDL_Rect bounds, SDL_bool fixed) {
    Component *collision = malloc(sizeof(Component));
    CollisionData *data = malloc(sizeof(CollisionData));

    data->bounds = bounds;
    data->fixed = fixed;

    collision->data = data;
    collision->update = updateCollision;
    collision->respond = respondCollision;
    collision->type = COLLISION;

    return collision;
}

void deleteCollision(Component *collision) {
    if (collision->type == COLLISION) {
       free(collision->data);
       free(collision);
       collision = NULL;
    }
    #ifdef DEBUG
    else {
        printf("Error: Component at %p is not a collision\n", collision);
    }
    #endif
}

void updateCollision(Uint32 interval, GameObject *self) {
    CollisionData *c1 = getComponent(self, COLLISION)->data;
    CollisionData *c2;
    GameObject *obj;
    for (int i = 0; i < objects->count; ++i) {
        obj = get(objects, i);
        if (obj != self) {
            c2 = getComponent(obj, COLLISION)->data;
            if (c2) {
                collision_t collision = checkCollision(c1->bounds, c2->bounds);
                if (collision) {
                    SDL_Event event;
                    event.type = COLLISIONEVENT;
                    event.user.code = collision;
                    event.user.data1 = self;
                    event.user.data2 = obj;
                    SDL_PushEvent(&event);
                }
            }
        }
    }
}

void respondCollision(SDL_Event *event, GameObject *self) {
    if (event->type == MOVEEVENT && event->user.data1 == self) {
        CollisionData *data = getComponent(self, COLLISION)->data;
        data->bounds.x = self->x;
        data->bounds.y = self->y;
    }
    else if (event->type == COLLISIONEVENT && event->user.data1 == self) {
        CollisionData *c1 = getComponent(self, COLLISION)->data;
        CollisionData *c2 = getComponent(event->user.data2, COLLISION)->data;
        if (!c1->fixed) {
            switch (event->user.code) {
            case TOP:
                move(self, 0, c2->bounds.y - c1->bounds.y - c1->bounds.h);
                break;
            case LEFT:
                move(self, c2->bounds.x - c1->bounds.x - c1->bounds.w, 0);
                break;
            case BOTTOM:
                move(self, 0, c2->bounds.y + c2->bounds.h - c1->bounds.y);
                break;
            case RIGHT:
                move(self, c2->bounds.x + c2->bounds.w - c1->bounds.x, 0);
                break;
            }
        }
    }
}

collision_t checkCollision(SDL_Rect r1, SDL_Rect r2) {
    if (r1.x < r2.x + r2.w && r1.x + r1.w > r2.x && r1.y < r2.y + r2.h && r1.y + r1.h > r2.y) {
        int tc = r1.y + r1.h - r2.y;
        int bc = r2.y + r2.h - r1.y;
        int lc = r1.x + r1.w - r2.x;
        int rc = r2.x + r2.w - r1.x;
        if (tc < bc && tc < lc && tc < rc) {
            return TOP;
        }
        if (bc < tc && bc < lc && bc < rc) {
            return BOTTOM;
        }
        if (lc < tc && lc < bc && lc < rc) {
            return LEFT;
        }
        if (rc < tc && rc < bc && rc < lc) {
            return RIGHT;
        }
    }
    return NONE;
}
