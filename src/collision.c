#include "collision.h"
#include "game_object.h"

Component *Collision(SDL_Rect bounds, SDL_bool fixed) {
    Component *collision = malloc(sizeof(Component));
    CollisionData *data = malloc(sizeof(CollisionData));

    data->bounds = bounds;
    data->fixed = fixed;
    data->collision = NONE;

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
    c1->collision = NONE;
    Component *temp;
    CollisionData *c2;
    GameObject *obj;
    for (int i = 0; i < objects->count; i++) {
        obj = vGet(objects, i);
        if (obj != self) {
            temp = getComponent(obj, COLLISION);
            if (temp) {
                c2 = temp->data;
                if (c2) {
                    Direction collision = checkCollision(c1->bounds, c2->bounds);
                    if (collision) {
                        SDL_Event event;
                        event.type = COLLISIONEVENT;
                        event.user.code = collision;
                        event.user.data1 = self;
                        event.user.data2 = obj;
                        SDL_PushEvent(&event);
                        c1->collision = collision;
                    }
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
            case UP:
                move(self, 0, c2->bounds.y - c1->bounds.y - c1->bounds.h);
                break;
            case LEFT:
                move(self, c2->bounds.x - c1->bounds.x - c1->bounds.w, 0);
                break;
            case DOWN:
                move(self, 0, c2->bounds.y + c2->bounds.h - c1->bounds.y);
                break;
            case RIGHT:
                move(self, c2->bounds.x + c2->bounds.w - c1->bounds.x, 0);
                break;
            }
        }
    }
}

Direction checkCollision(SDL_Rect r1, SDL_Rect r2) {
    if (r1.x < r2.x + r2.w && r1.x + r1.w > r2.x && r1.y < r2.y + r2.h && r1.y + r1.h > r2.y) {
        int tc = r1.y + r1.h - r2.y;
        int bc = r2.y + r2.h - r1.y;
        int lc = r1.x + r1.w - r2.x;
        int rc = r2.x + r2.w - r1.x;
        if (tc < bc && tc < lc && tc < rc) {
            return UP;
        }
        if (bc < tc && bc < lc && bc < rc) {
            return DOWN;
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
