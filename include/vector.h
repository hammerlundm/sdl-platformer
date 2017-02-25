#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    void **data;
    int count;
    int _limit;
} vector;

vector *newVector();
void deleteVector(vector* v);

void vInsert(vector* v, void *data);
void vRemove(vector *v, int index);
void *vGet(vector *v, int index);
int vFind(vector *v, void *data);

void map(vector* v, void (*func)(void *data));

#endif /*VECTOR_H*/
