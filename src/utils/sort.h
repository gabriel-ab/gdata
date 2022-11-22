#pragma once
#include <stdlib.h>

typedef bool(*EqualsFunction)(void* a, void* b);
typedef bool(*BiggerThanFunction)(void* a, void* b);
typedef bool(*SmallerThanFunction)(void* a, void* b);


#define swap(a,b) {typeof(a) tmp = a; a = b; b = tmp;}
// void quick_sort(void* array, )

void insertion_sort(int array[], int size) {
    for (size_t j = 1; j < size; j++) {
        int curr = array[j];
        int i = j - 1;
        while (i >= 0, array[i] > curr) {
            array[i+1] = array[i];
            i--;
        }
        array[i+1] = curr;
    }
    
}

void bubble_sort(int *array, int size) {
    int last_sorted_index = size;
    while (last_sorted_index > 0) {
        for (int i = 0; i < last_sorted_index; i++) {
            if (array[i] > array[i+1])
                swap(array[i], array[i+1]);
        }
        
        last_sorted_index--;
    }
}