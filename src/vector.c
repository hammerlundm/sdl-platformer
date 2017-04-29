#include "vector.h"
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

vector *newVector() {
    vector *v = malloc(sizeof(vector));
    v->data = malloc(sizeof(void *)*8);
    v->count = 0;
    v->_limit = 8;
    return v;
}

void deleteVector(vector* v) {
    free(v->data);
    free(v);
    v = NULL;
}

void vInsert(vector* v, void *data) {
    if (v->count >= v->_limit-1) {
        v->_limit *= 2;
        v->data = realloc(v->data, sizeof(void *)*v->_limit);
    }
    v->data[v->count] = data;
    v->count += 1;
}

void vRemove(vector *v, int index) {
    if (index >= 0 && index < v->count) {
        v->count -= 1;
        v->data[index] = v->data[v->count];
        v->data[v->count] = NULL;
    }
    #ifdef DEBUG
    else {
        printf("Error: index %d of %p is out of bounds", index, v);
    }
    #endif
}

void *vGet(vector* v, int index) {
    if (index >= 0 && index < v->count) {
        return v->data[index];
    }
    #ifdef DEBUG
    else {
        printf("Error: index %d of %p is out of bounds", index, v);
    }
    #endif
    return NULL;
}

int vFind(vector *v, void *data) {
    for (int i = 0; i < v->count; i++) {
        if (v->data[i] == data) {
            return i;
        }
    }
    return -1;
}

void map(vector* v, void (*func)(void *data)) {
    for (int i = 0; i < v->count; i++) {
        func(v->data[i]);
    }
}
