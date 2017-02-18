#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    void **data;
    int count;
    int _limit;
} vector;

vector newVector();
void deleteVector(vector* v);

void insert(vector* v, void *data);
void *get(vector* v, int index);

void map(vector* v, void (*func)(void *data));

#endif /*VECTOR_H*/
