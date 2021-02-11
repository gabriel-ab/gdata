#include "vector.h"
#include <stdlib.h>
#include <string.h>

#define VECTOR_INCREMENT 32
#define MAX_LATERAL_SIZE 512 // in bytes

void* _vector_create(size_t data_size, size_t size, void* values) {
    charVector vector = malloc(sizeof(*vector));
    if (vector) {
        size_t alloc_size = size;
        vector->size = size;
        vector->internal.offset = 0;
        vector->internal.alloc = alloc_size;
        vector->internal.dsize = data_size;
        vector->internal.start = vector->at = calloc(alloc_size, data_size);

        if (values) 
            memcpy(vector->at, values, size*data_size);
    }
    return (void*)vector;
}

void vector_delete(void* v) {
    free(((charVector)v)->at);
    free(v);
}

static void resize_right(void* vector, signed long change) {
    charVector v = vector;
    v->internal.alloc += change;
    v->internal.start = realloc(v->internal.start, v->internal.alloc * v->internal.dsize);
    v->at = v->internal.start + v->internal.offset * v->internal.dsize;
}

static void resize_left(void* vector, signed long change) {
    charVector v = vector;
    v->internal.alloc += change;

    if (change > 0) {
        v->internal.start = realloc(v->internal.start, v->internal.alloc * v->internal.dsize);
        v->at = v->internal.start + v->internal.offset * v->internal.dsize;
        memmove(v->at + change*(signed)v->internal.dsize, v->at, v->size * v->internal.dsize);
    } else {
        memmove(v->at + change*(signed)v->internal.dsize, v->at, v->size * v->internal.dsize);
        v->internal.start = realloc(v->internal.start, v->internal.alloc * v->internal.dsize);
    }
    v->internal.offset += change;
    v->at = v->internal.start + v->internal.offset * v->internal.dsize;
}

void vec_push_back(void* vector, void* data) {
    charVector v = vector;
    if (v->at + v->internal.dsize * v->size == v->internal.start + v->internal.dsize * v->internal.alloc) // is on the right border
        resize_right(v, VECTOR_INCREMENT);

    void* dest = v->at + v->internal.dsize * v->size++;
    memcpy(dest, data, v->internal.dsize);
}

void vec_push_front(void* vector, void* data) {
    charVector v = vector;
    if (v->at == v->internal.start)
        resize_left(vector, VECTOR_INCREMENT);
    
    v->size++;
    v->internal.offset--;
    v->at -= v->internal.dsize;
    memcpy(v->at, data, v->internal.dsize);
}

void* vec_pop_back(void* vector) {
    charVector v = vector;
    size_t right = v->internal.alloc - (v->internal.offset + v->size);
    size_t limit = MAX_LATERAL_SIZE / v->internal.dsize;
    
    if (right > limit)
        resize_right(vector, -(signed)(limit*2/3));
    
    v->size--;
    return v->at + v->internal.dsize * v->size;
}

void* vec_pop_front(void* vector) {
    charVector v = vector;
    size_t left = v->internal.offset;
    size_t limit = MAX_LATERAL_SIZE / v->internal.dsize;

    if (left > limit)
        resize_left(vector, -(signed)(limit*2/3));
    
    v->size--;
    v->internal.offset++;
    v->at += v->internal.dsize;
    return v->at - v->internal.dsize;
}