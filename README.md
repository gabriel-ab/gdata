# gdata
A C generic type data library.

### Implemented
- [x] List
- [x] Stack 
- [x] Array
- [x] Vector
- [x] Heap
- [ ] Tree

**List**: Doubly linked list with tools to handle it

**Stack**: Singly linked list with push/pop operations

**Array**: Simple array with lenght implementation

**Vector**: Dinamic size vector, with push/pop operations

**Heap**: Fixed size heap struture, with push/pop operations

## Syntax style

All functions use snake case notation, stating by the name of the type:
ex: `list_create()`

All function macros use the uppercase version of the equivalent funcion:
ex: `LIST_CREATE()`

The macros provide a easy systax to use typed values in some functions like: `create`, `append`, `pop`.
ex:
```c
VECTOR_CREATE(int, 1,2,3)
```
is equivalent to:
```c
vector_create(sizeof(int), 3, (int[]){1,2,3})
```

## Build

### with cmake
    $ mkdir build
    $ cd build
    $ cmake ../
    $ make

## Tests
[Check](https://github.com/libcheck/check) was used to test this library.