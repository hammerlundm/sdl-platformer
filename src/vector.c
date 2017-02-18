#include "vector.h"
#include <stdlib.h>

vector *newVector() {
    vector *v = malloc(sizeof(vector));
    v->data = NULL;
    v->count = 0;
    v->_limit = 0;
    return v;
}

void deleteVector(vector* v) {
    free(v->data);
    free(v);
    v = NULL;
}

void insert(vector* v, void *data) {
    if (v->count == 0) {
        v->data = malloc(sizeof(void *)*8);
        v->_limit = 8;
    }
    else if (v->count >= v->_limit-1) {
        v->_limit *= 2;
        v->data = realloc(v->data, sizeof(void *)*v->_limit);
    }
    v->data[v->count] = data;
    v->count += 1;
}

void *get(vector* v, int index) {
    return v->data[index];
}

void map(vector* v, void (*func)(void *data)) {
    for (int i = 0; i < v->count; ++i) {
        func(v->data[i]);
    }
}
