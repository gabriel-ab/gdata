#include "gdata/vector.h"
#include <stdlib.h>
#include <string.h>

#define VECTOR_INCREMENT 32
#define MAX_LATERAL_SIZE 64

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

AnyVector vector_create(size_t dsize, size_t initial_size, void* initial_values) {
    charVector vector = malloc(sizeof(*vector));
    if (vector) {
        size_t alloc_size = initial_size;
        vector->size = initial_size;
        vector->internal.offset = 0;
        vector->internal.alloc = alloc_size;
        vector->internal.dsize = dsize;

        vector->internal.begin = vector->at = initial_size ? calloc(alloc_size, dsize) : NULL;

        if (initial_values) 
            memcpy(vector->at, initial_values, initial_size*dsize);
    }
    return (void*)vector;
}

void vector_pushback(AnyVector vector, size_t num_elements, void* data) {
    charVector v = vector;
    size_t avaliable = v->internal.alloc - v->internal.offset - v->size;

    if (v->size + num_elements >= avaliable) {
        size_t increment = (num_elements/VECTOR_INCREMENT + 1)*VECTOR_INCREMENT;
        resize_right(v, increment);
    }

    memcpy(vector_at(v, v->size), data, num_elements*v->internal.dsize);
    v->size += num_elements;
}

void vector_pushfront(AnyVector vector, size_t num_elements, void* data) {
    charVector v = vector;

    if (num_elements >= v->internal.offset) {
        size_t increment = (num_elements/VECTOR_INCREMENT + 1)*VECTOR_INCREMENT;
        resize_left(vector, increment);
    }
    
    v->size += num_elements;
    v->internal.offset -= num_elements;
    v->at = v->internal.begin + v->internal.offset*v->internal.dsize;
    memcpy(v->at, data, num_elements*v->internal.dsize);
}

void* vector_popback(AnyVector vector) {
    charVector v = vector;
    size_t right = v->internal.alloc - (v->internal.offset + v->size);
    
    if (right > MAX_LATERAL_SIZE)
        resize_right(vector, -VECTOR_INCREMENT);
    
    v->size--;
    return vector_at(v, v->size);
}

void* vector_popfront(AnyVector vector) {
    charVector v = vector;
    size_t left = v->internal.offset;

    if (left > MAX_LATERAL_SIZE)
        resize_left(vector, -VECTOR_INCREMENT);
    
    v->size--;
    v->internal.offset++;
    v->at = vector_at(v, 1);
    return v->at - v->internal.dsize;
}

void vector_delete(AnyVector v) {
    free(((charVector)v)->internal.begin);
    free(v);
}

void vector_remove(AnyVector vector, size_t index) {
    charVector v = vector;
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

void* vector_at(AnyVector vector, size_t index) {
    return ((charVector)vector)->at + index*((charVector)vector)->internal.dsize;
}