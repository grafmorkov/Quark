#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

typedef struct{
    void *data;
    size_t capacity;
    size_t size;
    size_t elem_size;
}Vector;

int vector_init(Vector *vec, size_t elem_size);
int vector_push_back(Vector *vec, void *element);
void *vector_get(Vector *vec, size_t index);
void vector_free(Vector *vec);
#endif
