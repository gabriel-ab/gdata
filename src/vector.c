#include "vector.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

VECTOR_TYPEDEF(uint8_t);

#define VECTOR_INCREMENT 32
#define MAX_LATERAL_SIZE 64

static void resize_right(void* vector, signed long change) {
    uint8_tVector v = vector;
    v->internal.alloc += change;
    v->internal.begin = realloc(v->internal.begin, v->internal.alloc * v->internal.dsize);
    v->at = v->internal.begin + v->internal.offset * v->internal.dsize;
}

static void resize_left(void* vector, signed long change) {
    uint8_tVector v = vector;
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

void* vector_create(size_t dsize, size_t initial_size, void* initial_values) {
    uint8_tVector vector = malloc(sizeof(*vector));
    if (vector) {
        void* ptr = initial_size ? calloc(initial_size, dsize) : NULL;
        *vector = (struct uint8_t_vector){
            .size = initial_size,
            .at = ptr,
            .internal.begin = ptr,
            .internal.offset = 0, 
            .internal.alloc = initial_size, 
            .internal.dsize = dsize
        };
        if (initial_values) 
            memcpy(vector->at, initial_values, initial_size*dsize);
    }
    return (void*)vector;
}

void vector_pushback(void* vector, size_t num_elements, void* data) {
    uint8_tVector v = vector;
    size_t avaliable = v->internal.alloc - v->internal.offset - v->size;

    if (v->size + num_elements >= avaliable) {
        size_t increment = (num_elements/VECTOR_INCREMENT + 1)*VECTOR_INCREMENT;
        resize_right(v, increment);
    }
    if (data) memcpy(vector_at(v, v->size), data, num_elements*v->internal.dsize);
    v->size += num_elements;
}

void vector_pushfront(void* vector, size_t num_elements, void* data) {
    uint8_tVector v = vector;

    if (num_elements >= v->internal.offset) {
        size_t increment = (num_elements/VECTOR_INCREMENT + 1)*VECTOR_INCREMENT;
        resize_left(vector, increment);
    }
    
    v->size += num_elements;
    v->internal.offset -= num_elements;
    v->at = v->internal.begin + v->internal.offset*v->internal.dsize;
    if (data) memcpy(v->at, data, num_elements*v->internal.dsize);
}

void* vector_popback(void* vector) {
    uint8_tVector v = vector;
    size_t right = v->internal.alloc - (v->internal.offset + v->size);
    
    if (right > MAX_LATERAL_SIZE)
        resize_right(vector, -VECTOR_INCREMENT);
    
    v->size--;
    return vector_at(v, v->size);
}

void* vector_popfront(void* vector) {
    uint8_tVector v = vector;
    size_t left = v->internal.offset;

    if (left > MAX_LATERAL_SIZE)
        resize_left(vector, -VECTOR_INCREMENT);
    
    v->size--;
    v->internal.offset++;
    v->at = vector_at(v, 1);
    return v->at - v->internal.dsize;
}

void vector_delete(void* v) {
    free(((uint8_tVector)v)->internal.begin);
    free(v);
}

void vector_remove(void* vector, size_t index) {
    uint8_tVector v = vector;
    if (index > v->size/2) {
        memmove(vector_at(v, index), vector_at(v, index + 1),
            (v->size - index -1) * v->internal.dsize);
    } else {
        memmove(vector_at(v, 1), v->at,
            index * v->internal.dsize);
        
        v->at = vector_at(v, 1);
        v->internal.offset++;
    }
    v->size--;
}

void* vector_at(const void* vector, size_t index) {
    const struct uint8_t_vector *V = vector;
    return V->at + index*V->internal.dsize;
}

void* vector_copy(void* input) {
    uint8_tVector vec = input;
    return vector_create(vec->internal.dsize, vec->size, vec->at);
}

void* vector_slice(const void* vector, unsigned int begin, unsigned int end) {
    const struct uint8_t_vector *vec = vector;
    size_t size = end - begin;
    void* initial_values = vec->at + begin*vec->internal.dsize;
    return vector_create(vec->internal.dsize, size, initial_values);
}

bool vector_equals(const void* a, const void* b) {
    const struct uint8_t_vector *A = a, *B = b;
    if (A->size != B->size || A->internal.dsize != B->internal.dsize)
        return false;
    size_t dsize = A->internal.dsize;
    return memcmp(A->at, B->at, dsize*A->size) == 0;
}