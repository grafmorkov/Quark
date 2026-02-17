#include <stdlib.h>
#include <string.h>

#include "../include/vector.h"

#define VEC_INIT_CAP 16

int vector_init(Vector *vec, size_t elem_size) {
    vec->size = 0;
    vec->elem_size = elem_size;
    vec->capacity = VEC_INIT_CAP;

    vec->data = malloc(vec->capacity * elem_size);

    if (!vec->data)
        return 0;
    return 1;
}
int vector_push_back(Vector *vec, void *element) {
    if (vec->size >= vec->capacity) {
        size_t new_capacity = vec->capacity * 2;
        void *tmp = realloc(vec->data, new_capacity * vec->elem_size);

        if (!tmp)
            return 0;

        vec->data = tmp;
        vec->capacity = new_capacity;
  }

  void *target = (char *)vec->data + vec->size * vec->elem_size;

  memcpy(target, element, vec->elem_size);

  vec->size++;
  return 1;
}
void *vector_get(Vector *vec, size_t index) {
    if (index >= vec->size)
        return NULL;

    return (char *)vec->data + index * vec->elem_size;
}
void vector_free(Vector *vec) {
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    vec->elem_size = 0;
}
