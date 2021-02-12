#include "vector.h"
#include <stdlib.h>
#include <string.h>

#define VECTOR_INCREMENT 32
#define MAX_LATERAL_SIZE 512 // in bytes

void* _vec_create(size_t data_size, size_t size, void* values) {
    charVector vector = malloc(sizeof(*vector));
    if (vector) {
        size_t alloc_size = size;
        vector->size = size;
        vector->internal.offset = 0;
        vector->internal.alloc = alloc_size;
        vector->internal.dsize = data_size;

        vector->internal.begin = vector->at = size ? calloc(alloc_size, data_size) : NULL;

        if (values) 
            memcpy(vector->at, values, size*data_size);
    }
    return (void*)vector;
}

void vector_delete(void* v) {
    free(((charVector)v)->internal.begin);
    free(v);
}

static void resize_right(void* vector, signed long change) {
    charVector v = vector;
    v->internal.alloc += change;
    v->internal.begin = realloc(v->internal.begin, v->internal.alloc * v->internal.dsize);
    v->at = v->internal.begin + v->internal.offset * v->internal.dsize;
}

static void resize_left(void* vector, signed long change) {
    charVector v = vector;
    v->internal.alloc += change;

    if (change > 0) {
        v->internal.begin = realloc(v->internal.begin, v->internal.alloc * v->internal.dsize);
        v->at = v->internal.begin + v->internal.offset * v->internal.dsize;
        memmove(v->at + change*(signed)v->internal.dsize, v->at, v->size * v->internal.dsize);
    } else {
        memmove(v->at + change*(signed)v->internal.dsize, v->at, v->size * v->internal.dsize);
        v->internal.begin = realloc(v->internal.begin, v->internal.alloc * v->internal.dsize);
    }
    v->internal.offset += change;
    v->at = v->internal.begin + v->internal.offset * v->internal.dsize;
}

void _vec_pushback(void* vector, size_t n, void* data) {
    charVector v = vector;
    size_t avaliable = v->internal.alloc - v->internal.offset - v->size;

    if (v->size + n >= avaliable) {
        size_t increment = (n/VECTOR_INCREMENT + 1)*VECTOR_INCREMENT;
        resize_right(v, increment);
    }

    void* dest = v->at + v->internal.dsize * v->size;
    memcpy(dest, data, n*v->internal.dsize);
    v->size += n;
    
}

void _vec_pushfront(void* vector, size_t n, void* data) {
    charVector v = vector;

    if (n >= v->internal.offset) {
        size_t increment = (n/VECTOR_INCREMENT + 1)*VECTOR_INCREMENT;
        resize_left(vector, increment);
    }
    
    v->size += n;
    v->internal.offset -= n;
    v->at -= n*v->internal.dsize;
    memcpy(v->at, data, n*v->internal.dsize);
}

void* _vec_popback(void* vector) {
    charVector v = vector;
    size_t right = v->internal.alloc - (v->internal.offset + v->size);
    size_t limit = MAX_LATERAL_SIZE / v->internal.dsize;
    
    if (right > limit)
        resize_right(vector, -(signed)(limit*2/3));
    
    v->size--;
    return v->at + v->internal.dsize * v->size;
}

void* _vec_popfront(void* vector) {
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