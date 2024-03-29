#include "heap.h"

static void heap_upheapify(Heap heap) {
    size_t k = heap->size;
    size_t s = heap->internal.dsize;

    // MaxHeap: father > kid
    // MinHeap: father < kid
    while(k >= 2) {
        void *a = heap->at + s*(k/2); // father
        void *b = heap->at + s*k;   // kid
        int cmp = heap->internal.cmp(a, b);
        
        if ((heap->order == MIN_HEAP && cmp > 0) ||
            (heap->order == MAX_HEAP && cmp < 0)) {
            char temp[s];
            memcpy(temp, a, s);
            memcpy(a, b, s);
            memcpy(b, temp, s);
            k /= 2;
        }
        else break;
    }
}

static void heap_downheapify(Heap heap) {
    size_t s = heap->internal.dsize;
    size_t p = 1, f = 2;
    size_t m = heap->size;
    
    int cmp;

    char x[s];
    memcpy(x, heap->at + s, s); // parrent = heap[1];

    
    while (f <= m) {
        void *child = heap->at + s*f; // left child
        if (f < m) {
            void *brother = child + s; // right child
            cmp = heap->internal.cmp(child, brother);

            if ((heap->order == MIN_HEAP && cmp > 0) ||
                (heap->order == MAX_HEAP && cmp < 0)) {
                child = brother;
                f++;
            }
        }
        cmp = heap->internal.cmp(x, child);
        if ((heap->order == MIN_HEAP && cmp < 0) || 
            (heap->order == MAX_HEAP && cmp > 0)) 
            break;
        memcpy(heap->at + s*p, child, s);
        p = f, f = 2*p;
    }
    memcpy(heap->at + s*p, x, s);
}

void* heap_create(size_t dsize, size_t max_size, comparator cmp, 
                  enum HeapOrder order) {
    Heap heap = calloc(1, sizeof(struct heap) + dsize*(max_size+1));
    heap->size = 0;
    *(enum HeapOrder*)&heap->order = order;
    *(comparator*)&heap->internal.cmp = cmp;
    *(size_t*)&heap->internal.dsize = dsize;
    *(size_t*)&heap->internal.alloc = max_size;
    return heap;
}

void heap_push(void *heap, void *data) {
    Heap H = heap;
    H->size++;
    memcpy(H->at + H->internal.dsize*H->size, data, H->internal.dsize);
    heap_upheapify(H);
}

// get the most relevant value
void* heap_pop(void* heap) {
    Heap H = heap;
    void* array = H->at;
    size_t dsize = H->internal.dsize;

    memcpy(array, array + dsize, dsize);                        // [0] = [1]
    memcpy(array + dsize, array + dsize*H->size, dsize);    // [1] = [used]
    memset(array + dsize*H->size, 0, dsize);                // [used] = 0 
    
    H->size--;
    heap_downheapify(H);
    return array;
}

void* heap_root(void* heap) {
    return ((Heap)heap)->at + ((Heap)heap)->internal.dsize;
}

int intcmp(void* a, void* b) {
    return *(int*)a - *(int*)b;
}
int floatcmp(void* a, void* b) {
    return *(float*)a - *(float*)b;
}
int doublecmp(void* a, void* b) {
    return *(double*)a - *(double*)b;
}