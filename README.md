# gdata
A C generic type data library.

### Implemented

- **List**: Doubly linked list with tools to handle it

- **Stack**: Singly linked list with push/pop operations

- **Array**: Simple array with lenght implementation

- **Vector**: Dinamic size vector, with push/pop operations

- **Heap**: Fixed size heap struture, with push/pop operations

- **Dict**: Fixed size hash table struture, with set/get operations

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

### Cmake
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Tests
To test, build with:
    
    $ cmake .. -DBUILD_TESTING=ON